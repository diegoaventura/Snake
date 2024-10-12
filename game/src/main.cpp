#include "raylib.h"
#include <cassert>
#include <cstdlib>
#include <ctime>

/// --- Defs ---

constexpr int BOARD_WIDTH = 20;
constexpr int BOARD_HEIGHT = 20;
constexpr float BOARD_SCALE = 40.0f;
constexpr int MARGIN_X = 1;
constexpr int MARGIN_Y = 1;

// ---Types ---

struct int2
{
    int x;
    int y;
};

// --- Globals ---

int2 SNAKE_DIRECTION;
int2 SNAKE[BOARD_WIDTH * BOARD_HEIGHT];
int SNAKE_LENGTH = 1;
int2 APPLE_POSITION;
double CURRENT_LAPSE_TIME = 0.25;
double LAST_TICK_TIME = 0.0;

// --- Helper funcs ---


int GetFixedPositionX(int posx, int movx)
{
    int newposX = posx + movx;
    if (newposX >= BOARD_WIDTH)
    {
        return 0;
    }
    if (newposX < 0)
    {
        return (BOARD_WIDTH - 1);
    }
    return newposX;
}

int GetFixedPositionY(int posy, int movy)
{
    int newposY = posy + movy;
    if (newposY >= BOARD_HEIGHT)
    {
        return 0;
    }
    if (newposY < 0)
    {
        return (BOARD_HEIGHT - 1);
    }
    return newposY;
}

void UpdateApplePosition()
{
    APPLE_POSITION.x = (rand() % BOARD_WIDTH);
    APPLE_POSITION.y = (rand() % BOARD_HEIGHT);
}

void Reset()
{
    // Set snake lenght
    SNAKE_LENGTH = 19;
    SNAKE_DIRECTION = { 0, 0 };
    CURRENT_LAPSE_TIME = 0.25;
    // Set snake pos
    SNAKE[0].x = (rand() % BOARD_WIDTH);
    SNAKE[0].y = (rand() % BOARD_HEIGHT);
    // Set snake mov
    int x = (rand() % 4);
    switch (x)
    {

    case 0:
        SNAKE_DIRECTION.x = -1;
        break;
    case 1:
        SNAKE_DIRECTION.x = 1;
        break;
    case 2:
        SNAKE_DIRECTION.y = -1;
        break;
    case 3:
        SNAKE_DIRECTION.y = 1;
        break;
    }
    // Set apple to random pos
    UpdateApplePosition();

    //...
}

// Check the position that the head of the snake is
void CheckPosition(int movx, int movy)
{
    int newSnakePosX = GetFixedPositionX(SNAKE[0].x, movx);
    int newSnakePosY = GetFixedPositionY(SNAKE[0].y, movy);

    // Apple
    if ((newSnakePosX == APPLE_POSITION.x) && (newSnakePosY == APPLE_POSITION.y))
    {
        SNAKE_LENGTH += 1;
        if (SNAKE_LENGTH > 20)
        {
            CURRENT_LAPSE_TIME -= 0.001;
        }
        else
        {
            CURRENT_LAPSE_TIME -= 0.005;
        }
        UpdateApplePosition();
    }
    // Check colision with the trail
    for (int i = 1; i < SNAKE_LENGTH - 1; i++)
    {
        if ((newSnakePosX == SNAKE[i].x) && (newSnakePosY == SNAKE[i].y))
        {
            Reset();
        }
    }
}

Color ResolveBoardColor(int pieceType)
{
    switch (pieceType)
    {

    case 1:
        return DARKGREEN;
    case 2:
        return RED;
    default:
        return BLACK;
    }
}

/// --- Init funcs ---

void DrawSubPiece(int x, int y, int c)
{
    DrawRectangle(
        static_cast<int>((1 + x) * BOARD_SCALE),    // posX
        static_cast<int>((1 + y) * BOARD_SCALE),    // posY
        static_cast<int>(BOARD_SCALE),              // width
        static_cast<int>(BOARD_SCALE),              // height
        ResolveBoardColor(c));                      // color
}

void DrawBoard()
{
    // Draw background
    DrawRectangle(1 * BOARD_SCALE, 1 * BOARD_SCALE, BOARD_WIDTH * BOARD_SCALE, BOARD_HEIGHT * BOARD_SCALE, BLACK);
}

// Draws the apple
void DrawApple()
{
    DrawSubPiece(APPLE_POSITION.x, APPLE_POSITION.y, 2);
}

// Draws the Full Snake
void DrawSnake()
{
    for (int i = 0; i < SNAKE_LENGTH; i ++)
    {
        DrawSubPiece(SNAKE[i].x, SNAKE[i].y, 1);
    }
}

void Draw()
{
    BeginDrawing();
    ClearBackground(GRAY);

    DrawBoard();
    DrawApple();
    DrawSnake();
    // DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
    EndDrawing();
}

void SnakeMovement(int MoveX,int MoveY)
{
    // Update train position
    for (int i = SNAKE_LENGTH - 1; i >= 1; i--)
    {
        SNAKE[i] = SNAKE[i - 1];
    }
    // Update head positionSNAK
    SNAKE[0].x += MoveX;
    SNAKE[0].y += MoveY;

    // left
    if (SNAKE[0].x < 0)
        SNAKE[0].x = BOARD_WIDTH - 1;
    // right
    if (SNAKE[0].x >= BOARD_WIDTH)
        SNAKE[0].x = 0;
    // bottom
    if (SNAKE[0].y >= BOARD_HEIGHT)
        SNAKE[0].y = 0;
    // up
    if (SNAKE[0].y < 0)
        SNAKE[0].y = BOARD_HEIGHT - 1;
    //apple
}

// It is called each tick time
// Updates the snake position
void Tick()
{
    CheckPosition(SNAKE_DIRECTION.x, SNAKE_DIRECTION.y);
    // Update snake position including its trail
    SnakeMovement(SNAKE_DIRECTION.x, SNAKE_DIRECTION.y);
}

void Update()
{

    if (IsKeyPressed(KEY_RIGHT) && (SNAKE_DIRECTION.x != -1))
    {
        SNAKE_DIRECTION.x = 1;
        SNAKE_DIRECTION.y = 0;
    }
    if (IsKeyPressed(KEY_LEFT) && (SNAKE_DIRECTION.x != 1))
    {
        SNAKE_DIRECTION.x = -1;
        SNAKE_DIRECTION.y = 0;
    }
    if (IsKeyPressed(KEY_DOWN) && (SNAKE_DIRECTION.y != -1))
    {
        SNAKE_DIRECTION.y = 1;
        SNAKE_DIRECTION.x = 0;
    }
    if (IsKeyPressed(KEY_UP) && (SNAKE_DIRECTION.y != 1))
    {
        SNAKE_DIRECTION.y = -1;
        SNAKE_DIRECTION.x = 0;
    }

    // Get current time
    double currentTime = GetTime();
    // If first time (LAST_TICK_TIME == 0) then just init last tick time
    if (LAST_TICK_TIME == 0.0)
    {
        LAST_TICK_TIME = currentTime;
    }
    // If has passed CURRENT_LAPSE_TIME since LAST_TICK_TIME 
    else if ((currentTime - LAST_TICK_TIME) >= CURRENT_LAPSE_TIME)
    {
        // New LAST_TICK_TIME is currentTime 
        LAST_TICK_TIME = currentTime;
        // Tick 
        Tick();
    }
}
/// -- Main ---

int main(void)
{
    std::srand(std::time(nullptr));
    InitWindow((BOARD_WIDTH + 2) * BOARD_SCALE, (BOARD_HEIGHT + 2) * BOARD_SCALE, "SNAKE");
    Reset();

    while (!WindowShouldClose())
    {
        Update();
        Draw();
    }

    CloseWindow();

    return 0;
}