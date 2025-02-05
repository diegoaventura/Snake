#include "raylib.h"
#include <cassert>
#include <cstdlib>
#include <ctime>
#include "MainMenu.h"
#include "SnakeGame.h"

/// --- Defs ---

constexpr int MARGIN_X = 1;
constexpr int MARGIN_Y = 1;
constexpr int BOARD_WIDTH = 20;
constexpr int BOARD_HEIGHT = 20;
constexpr float BOARD_SCALE = 40.0f;

enum class AppState
{
    MainMenu,
    Game
};

/// -- Main ---

int main(void)
{
    std::srand(std::time(nullptr));
    InitWindow((BOARD_WIDTH + 2) * BOARD_SCALE, (BOARD_HEIGHT + 2) * BOARD_SCALE, "SNAKE");

    MainMenu mm((BOARD_WIDTH + 2) * BOARD_SCALE, (BOARD_HEIGHT + 2) * BOARD_SCALE);
    SnakeGame game(
        int2((BOARD_WIDTH + 2) * BOARD_SCALE, (BOARD_HEIGHT + 2) * BOARD_SCALE),
        int2(BOARD_WIDTH, BOARD_HEIGHT));

    AppState appState = AppState::MainMenu;
    
    double lastTime = GetTime();
    bool shouldExit = false;
    while (!WindowShouldClose() && !shouldExit)
    {
        double currentTime = GetTime();
        double elapsedTime = currentTime - lastTime;

        if (elapsedTime > 1.0 / 60)
        {
            lastTime = currentTime;

            switch (appState)
            {
                case AppState::MainMenu:
                {
                    MainMenu::ReturnState menuState = mm.Update(elapsedTime);
                    if (menuState == MainMenu::ReturnState::Start)
                    {
                        appState = AppState::Game;
                        game.Reset();
                    }
                    else if (menuState == MainMenu::ReturnState::Exit)
                    {
                        shouldExit = true;
                    }
                }
                break;

                case AppState::Game:
                {
                    SnakeGame::ReturnState SnakeReturnState = game.Update();
                    if (SnakeReturnState == SnakeGame::ReturnState::Exit)
                    {
                        appState = AppState::MainMenu;
                    }
                }
                break;
            }

            BeginDrawing();
            switch (appState)
            {
            case AppState::MainMenu:
                mm.Draw();
                break;
            case AppState::Game:
                game.Draw();
                break;
            }

            EndDrawing();

        }
    }

    CloseWindow();
    return 0;
}