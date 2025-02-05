#include "MainMenu.h"
#include "iostream"
#include "cassert"

MainMenu::MainMenu(int windowWidth, int windowHeight)
{
	m_WindowWidth = windowWidth;
	m_WindowHeight = windowHeight;
}


MainMenu::~MainMenu()
{

}


void MainMenu::Draw()
{
	constexpr Color bkgColor = { 0x35, 0x5e, 0x3b, 0xff };
	constexpr Color textColor = { 0x32, 0xcd, 0x32, 0xff };

	// Background
	ClearBackground(bkgColor);
	DrawRectangleRoundedLines(
		{ 20, 20, static_cast<float>(m_WindowWidth - 40), static_cast<float>(m_WindowHeight - 40) },
		0.2f, 50, 1.0f, textColor);

	// Title
	const int titleWidth = MeasureText("SNAKE", titleFontSize);
	const int titleX = (m_WindowWidth - titleWidth) / 2;
	DrawText("SNAKE", titleX, 50, titleFontSize, textColor);
	constexpr int titleBottom = 50 + titleFontSize;

	// Options
	Extents optionExtents = GetOptionTexttExtents(0);
	DrawText("New Game", optionExtents.left, optionExtents.top, optionsFontSize, textColor);
	optionExtents = GetOptionTexttExtents(1);
	DrawText("Options", optionExtents.left, optionExtents.top, optionsFontSize, textColor);
	optionExtents = GetOptionTexttExtents(2);
	DrawText("Credits", optionExtents.left, optionExtents.top, optionsFontSize, textColor);
	optionExtents = GetOptionTexttExtents(3);
	DrawText("Exit", optionExtents.left, optionExtents.top, optionsFontSize, textColor);
	
	// Draw asterisc in selected option
	if (m_OptionSelected != -1)
	{
		optionExtents = GetOptionTexttExtents(m_OptionSelected);
		DrawText("*", optionExtents.left - MeasureText("* ", optionsFontSize), optionExtents.top, optionsFontSize, textColor);
	}
}

MainMenu::ReturnState MainMenu::Update(float elapsed_sec)
{
	m_OptionSelected = -1;

	Vector2 mousePos = GetMousePosition();

	// Check New Game
	if (Extents e = GetOptionTexttExtents(0); CheckBound(&mousePos, &e))
	{
		m_OptionSelected = 0;
	}

	// Check Options
	else if (Extents e = GetOptionTexttExtents(1); CheckBound(&mousePos, &e))
	{
		m_OptionSelected = 1;
	}

	// Check Credits
	else if (Extents e = GetOptionTexttExtents(2); CheckBound(&mousePos, &e))
	{
		m_OptionSelected = 2;
	}

	// Check Exit
	else if (Extents e = GetOptionTexttExtents(3); CheckBound(&mousePos, &e))
	{
		m_OptionSelected = 3;
	}

	// Check Click
	if (IsMouseButtonPressed(0))
	{
		switch (m_OptionSelected)
		{
		case 0:
			return ReturnState::Start;
		case 3:
			return ReturnState::Exit;
		}
	}

	return ReturnState::Nothing;
}

// Returns the extensts of optionIdx option
Extents MainMenu::GetOptionTexttExtents(int optionIdx)
{
	const int titleWidth = MeasureText("SNAKE", titleFontSize);
	const int titleX = (m_WindowWidth - titleWidth) / 2;
	constexpr int titleBottom = 50 + titleFontSize;
	const int optionsY = ((m_WindowHeight - titleBottom) - (titleFontSize * 4)) / 2 + titleFontSize;

	switch (optionIdx)
	{
	case 0: // new game
		return { titleX, optionsY + (optionIdx * optionsFontSize), MeasureText("New Game", optionsFontSize), optionsFontSize };
	case 1: // options
		return { titleX, optionsY + (optionIdx * optionsFontSize), MeasureText("Options", optionsFontSize), optionsFontSize};
	case 2: // credits
		return { titleX, optionsY + (optionIdx * optionsFontSize), MeasureText("Credits", optionsFontSize), optionsFontSize };
	case 3: // Exit
		return { titleX, optionsY + (optionIdx * optionsFontSize), MeasureText("Exit", optionsFontSize), optionsFontSize };
	default:
		assert(false);
		return {};
	}
}

bool MainMenu::CheckBound(const Vector2* p, const Extents* e) 
{
	return(p->x >= e->left && p->x < (e->left + e->width)) && (p->y >= e->top && p->y < (e->top + e->height));
}