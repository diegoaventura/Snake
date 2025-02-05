#ifndef __SNAKE_GAME_H
#define __SNAKE_GAME_H

#include "raylib.h"

struct int2
{
	int x;
	int y;
};

class SnakeGame
{
public:

	enum class ReturnState
	{
		Nothing,
		Exit
	};

				SnakeGame(int2 WindowSize, int2 boardSize);
				~SnakeGame();
	
	void		Draw();
	ReturnState	Update();
	void		Reset();

private:

	enum class GameState
	{
		Playing,
		Pause,
		GameOver
	};

	// Init
	void		InitTextures();

	// Update
	ReturnState	Tick();
	void		SnakeMovement(int MoveX, int MoveY);
	void		UpdateApplePosition();

	// Drawing
	void		DrawBoard();
	void		DrawSprite(int2 spriteCoordsInPixels, int2 drawPosInTableCoords);
	void		DrawApple();
	void		DrawSnake();
	void		DrawPause();
	void		DrawGameOver();

	// ...
	bool		CheckPosition(int movx, int movy);
	int			GetFixedPositionX(int posx, int movx);
	int			GetFixedPositionY(int posy, int movy);

	// Konstants
	static constexpr Color bkgColor = { 0x35, 0x5e, 0x3b, 0xff };
	static constexpr Color textColor = { 0x32, 0xcd, 0x32, 0xff };

	int			m_BoardWidth;
	int			m_BoardHeight;
	float		m_BoardScale;
	int2		m_SnakeDirection;
	int2*		m_Snake;
	int2		m_ApplePosition;
	int			m_SnakeLength = 1;
	int			SpriteScale = 64;
	double		m_CurrentLapseTime;
	double		m_LastTickTime;
	Texture2D	m_SnakeGraphics;
	GameState	m_gameState;
	int2		m_WindowSize;
	
	const char* const m_SpriteFilename = "snake-graphics.png";
};

#endif