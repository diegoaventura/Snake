#include "raylib.h"
#include <cassert>

/// --- Defs ---

constexpr int BOARD_WIDTH = 10;
constexpr int BOARD_HEIGHT = 20;
constexpr float BOARD_SCALE = 40.0f;
constexpr int MARGIN_X = 1;
constexpr int MARGIN_Y = 1;

// --- Globals ---

int BOARD[BOARD_WIDTH * BOARD_HEIGHT];
double CURRENT_LAPSE_TIME = 1.0;
double LAST_TICK_TIME = 0.0;

// --- Helper funcs ---

void SetBoardValue(int x, int y, int value)
{
    if ((x >= 0) && (x < BOARD_WIDTH) && (y >= 0) && (y < BOARD_HEIGHT))
    {
        BOARD[(y * BOARD_WIDTH) + x] = value;
    }
}

int GetBoardValue(int x, int y)
{
    if ((x >= 0) && (x < BOARD_WIDTH) && (y >= 0) && (y < BOARD_HEIGHT))
    {
        return BOARD[(y * BOARD_WIDTH) + x];
    }

    return 0;
}

/// --- Init funcs ---

void InitBoard()
{
    for (int i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++)
    {
        BOARD[i] = 0;
    }

    // Debug
    //SetBoardValue(2, 6, 2);
    //SetBoardValue(7, 12, 3);
}

void DrawBoard()
{
    // Draw background
    DrawRectangle(1 * BOARD_SCALE, 1 * BOARD_SCALE, BOARD_WIDTH * BOARD_SCALE, BOARD_HEIGHT * BOARD_SCALE, BLACK);
}

/// -- Main ---

int main(void)
{
    InitWindow((BOARD_WIDTH + 2) * BOARD_SCALE, (BOARD_HEIGHT + 2) * BOARD_SCALE, "TETRIS");
    InitBoard();


    return 0;
}