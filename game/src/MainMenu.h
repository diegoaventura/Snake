#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "raylib.h"

struct Extents
{
	int left;	
	int top;
	int width;
	int height;
};

class MainMenu
{
public:

	enum class ReturnState
	{
		Nothing,
		Start,
		Exit
	};

	MainMenu(int windowWidth, int windowHeight);
	~MainMenu();
	
	void Draw();
	ReturnState Update(float elapsed_sec);

	void Exit();
	void EnterGame();

private:

	static constexpr int titleFontSize = 80;
	static constexpr int optionsFontSize = 60;

	Extents GetOptionTexttExtents(int optionIdx);
	static bool CheckBound(const Vector2* p, const Extents* e);

	// -1 = none
	// 0 = New Game
	int m_OptionSelected;

	int m_WindowWidth;
	int m_WindowHeight;

	static int numMMs;
};

#endif
