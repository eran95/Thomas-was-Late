#include "Engine.h"


bool Engine::detectCollisions(PlayableCharacter& character)
{
	bool reachedGoal = false;
	// Make a rect for all his parts
	FloatRect detectionZone = character.getPosition();
	// Make a FloatRect to test each block
	FloatRect block;
	block.width = TILE_SIZE;
	block.height = TILE_SIZE;
	// Build a zone around thomas to detect collisions
	int startX = (int)(detectionZone.left / TILE_SIZE) - 1;/*This gives us the horizontal index of the block that comes before 
														   the block the player currently stands*/
	int startY = (int)(detectionZone.top / TILE_SIZE) - 1;/*This gives us the vertical index of the block that comes before 
														   the block the player currently stands*/
	int endX = (int)(detectionZone.left / TILE_SIZE) + 2;//Checking up to 2 blocks ahead horizontally
	// Thomas is quite tall so check a few tiles vertically
	int endY = (int)(detectionZone.top / TILE_SIZE) + 3;//Checking up to 3 blocks ahead vertically
	// Make sure we don't test positions lower than zero
	// Or higher than the end of the array
	if (startX < 0)startX = 0;
	if (startY < 0)startY = 0;
	if (endX >= m_LM.getLevelSize().x)
		endX = m_LM.getLevelSize().x;
	if (endY >= m_LM.getLevelSize().y)
		endY = m_LM.getLevelSize().y;

	// Has the character fallen out of the map?
	FloatRect level(0.0f, 0.0f,
		m_LM.getLevelSize().x * TILE_SIZE,
		m_LM.getLevelSize().y * TILE_SIZE);
	if (!character.getPosition().intersects(level))
	{
		// respawn the character
		character.spawn(m_LM.getStartPosition(), GRAVITY);
	}
	// Loop through all the local blocks
	for (int x = startX; x < endX; x++)
	{
		for (int y = startY; y < endY; y++)
		{
			// Initialize the starting position of the current block
			block.left = x * TILE_SIZE;//The x coordinate of the left edge of the block the player is currently standing on
			block.top = y * TILE_SIZE;/*The y coordinate of the bottom edge of the block the player 
									  is currently standing on(in SFML the positive Y direction is down)*/
			// Has character been burnt or drowned?
			// Use head as this allows him to sink a bit
			if (m_ArrayLevel[y][x] == 2 || m_ArrayLevel[y][x] == 3)
			{
				if (character.getHead().intersects(block))
				{
					character.spawn(m_LM.getStartPosition(), GRAVITY);
					// Which sound should be played?
					if (m_ArrayLevel[y][x] == 2)// Fire, ouch!
					{
						// Play a sound
						m_SM.playFallInFire();
					}
					else // Water
					{
						// Play a sound
						m_SM.playFallInWater();
					}
				}
			}
			// Is character colliding with a regular block
			if (m_ArrayLevel[y][x] == 1)
			{
				if (character.getRight().intersects(block))
				{
					character.stopRight(block.left);
				}
				else if (character.getLeft().intersects(block))
				{
					character.stopLeft(block.left);
				}
				if (character.getFeet().intersects(block))
				{
					character.stopFalling(block.top);
				}
				else if (character.getHead().intersects(block))
				{
					character.stopJump();
				}
			}
			// More collision detection here once we have
			// learned about particle effects
			// Has the character reached the goal?
			if (m_ArrayLevel[y][x] == 4)
			{
				// Character has reached the goal
				reachedGoal = true;
			}
		}
	}
	// All done, return, whether or not a new level might be required
	return reachedGoal;
}
