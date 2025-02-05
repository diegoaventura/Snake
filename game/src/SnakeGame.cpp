#include "SnakeGame.h"
#include <ctime>
#include <cstdlib>

SnakeGame::SnakeGame(int2 WindowSize, int2 boardSize)
{
    m_LastTickTime = 0.f;
    m_BoardWidth = boardSize.x;
    m_BoardHeight = boardSize.y;
    m_BoardScale = WindowSize.x / (boardSize.x + 2);
    m_WindowSize.x = WindowSize.x;
    m_WindowSize.y = WindowSize.y;

    m_Snake = new int2[m_BoardWidth * m_BoardHeight];
    InitTextures();

    Reset();
}

SnakeGame::~SnakeGame()
{
    delete[] m_Snake;
}

SnakeGame::ReturnState SnakeGame::Update()
{
    ReturnState retState = ReturnState::Nothing;

    if (m_gameState == GameState::Pause)
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            m_gameState = GameState::Playing;
        }

        if (IsKeyPressed(KEY_M))
        {
            retState = ReturnState::Exit;
        }

        return retState;
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        m_gameState = GameState::Pause;
    }

    // Check SnakeMovement
    if (IsKeyPressed(KEY_RIGHT) && (m_SnakeDirection.x != -1))
    {
        m_SnakeDirection.x = 1;
        m_SnakeDirection.y = 0;
    }
    if (IsKeyPressed(KEY_LEFT) && (m_SnakeDirection.x != 1))
    {
        m_SnakeDirection.x = -1;
        m_SnakeDirection.y = 0;
    }
    if (IsKeyPressed(KEY_DOWN) && (m_SnakeDirection.y != -1))
    {
        m_SnakeDirection.y = 1;
        m_SnakeDirection.x = 0;
    }
    if (IsKeyPressed(KEY_UP) && (m_SnakeDirection.y != 1))
    {
        m_SnakeDirection.y = -1;
        m_SnakeDirection.x = 0;
    }

    // Get current time
    double currentTime = GetTime();
    // If first time (LAST_TICK_TIME == 0) then just init last tick time
    if (m_LastTickTime == 0.0)
    {
        m_LastTickTime = currentTime;
    }
    // If has passed m_CurrentLapseTime since LAST_TICK_TIME 
    else if ((currentTime - m_LastTickTime) >= m_CurrentLapseTime)
    {
        // New LAST_TICK_TIME is currentTime 
        m_LastTickTime = currentTime;
        // Tick 
        retState = Tick();
    }
    
    return retState;
}

void SnakeGame::Draw()
{
    ClearBackground(GRAY);

    DrawBoard();
    DrawApple();
    DrawSnake();
    switch (m_gameState)
    {
    case GameState::Pause:
        DrawPause();
        break;
    case GameState::GameOver:
        DrawGameOver();
        break;
    }
    // DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
}

SnakeGame::ReturnState SnakeGame::Tick()
{
    bool validPosition = CheckPosition(m_SnakeDirection.x, m_SnakeDirection.y);
    if (validPosition)
    {
        // Update snake position including its trail
        SnakeMovement(m_SnakeDirection.x, m_SnakeDirection.y);
        return ReturnState::Nothing;
    }
    else
    {
        return ReturnState::Exit;
    }
}

bool SnakeGame::CheckPosition(int movx, int movy)
{
    bool validPosition = true;

    int newSnakePosX = GetFixedPositionX(m_Snake[0].x, movx);
    int newSnakePosY = GetFixedPositionY(m_Snake[0].y, movy);

    // Apple
    if ((newSnakePosX == m_ApplePosition.x) && (newSnakePosY == m_ApplePosition.y))
    {
        m_SnakeLength += 1;
        if (m_SnakeLength > 20)
        {
            m_CurrentLapseTime -= 0.001;
        }
        else
        {
            m_CurrentLapseTime -= 0.005;
        }
        UpdateApplePosition();
    }
    // Check colision with the trail
    for (int i = 1; i < m_SnakeLength - 1; i++)
    {
        if ((newSnakePosX == m_Snake[i].x) && (newSnakePosY == m_Snake[i].y))
        {
            //Reset();
            validPosition = false;
        }
    }

    return validPosition;
}

void SnakeGame::SnakeMovement(int MoveX, int MoveY)
{
    // Update train position
    for (int i = m_SnakeLength - 1; i >= 1; i--)
    {
        m_Snake[i] = m_Snake[i - 1];
    }
    // Update head positionSNAK
    m_Snake[0].x += MoveX;
    m_Snake[0].y += MoveY;

    // left
    if (m_Snake[0].x < 0)
        m_Snake[0].x = m_BoardWidth - 1;
    // right
    if (m_Snake[0].x >= m_BoardWidth)
        m_Snake[0].x = 0;
    // bottom
    if (m_Snake[0].y >= m_BoardHeight)
        m_Snake[0].y = 0;
    // up
    if (m_Snake[0].y < 0)
        m_Snake[0].y = m_BoardHeight - 1;
    //apple
}

int SnakeGame::GetFixedPositionX(int posx, int movx)
{
    int newposX = posx + movx;
    if (newposX >= m_BoardWidth)
    {
        return 0;
    }
    if (newposX < 0)
    {
        return (m_BoardWidth - 1);
    }
    return newposX;
}

int SnakeGame::GetFixedPositionY(int posy, int movy)
{
    int newposY = posy + movy;
    if (newposY >= m_BoardHeight)
    {
        return 0;
    }
    if (newposY < 0)
    {
        return (m_BoardHeight - 1);
    }
    return newposY;
}

void SnakeGame::UpdateApplePosition()
{
    m_ApplePosition.x = (rand() % m_BoardWidth);
    m_ApplePosition.y = (rand() % m_BoardHeight);
}

void SnakeGame::Reset()
{
    // Set snake lenght
    m_SnakeLength = 2;
    m_SnakeDirection = { 0, 0 };
    m_CurrentLapseTime = 0.25;
    // Set snake pos
    m_Snake[0].x = (rand() % m_BoardWidth);
    m_Snake[0].y = (rand() % m_BoardHeight);
    // Set snake mov
    int x = (rand() % 4);
    switch (x)
    {

    case 0:
        m_SnakeDirection.x = -1;
        break;
    case 1:
        m_SnakeDirection.x = 1;
        break;
    case 2:
        m_SnakeDirection.y = -1;
        break;
    case 3:
        m_SnakeDirection.y = 1;
        break;
    }
    // Set apple to random pos
    UpdateApplePosition();

    m_gameState = GameState::Playing;
}

void SnakeGame::DrawBoard()
{
    // Draw background
    DrawRectangle(1 * m_BoardScale, 1 * m_BoardScale, m_BoardWidth * m_BoardScale, m_BoardHeight * m_BoardScale, BLACK);
}

void SnakeGame::DrawApple()
{
    DrawSprite({ 0, 192 }, m_ApplePosition);
}

void SnakeGame::DrawSprite(int2 spriteCoordsInPixels, int2 drawPosInTableCoords)
{
    Rectangle srcRect;
    srcRect.x = spriteCoordsInPixels.x;
    srcRect.y = spriteCoordsInPixels.y;
    srcRect.width = SpriteScale;
    srcRect.height = SpriteScale;

    Rectangle dstRect;
    dstRect.x = static_cast<int>((1 + drawPosInTableCoords.x) * m_BoardScale);
    dstRect.y = static_cast<int>((1 + drawPosInTableCoords.y) * m_BoardScale);
    dstRect.width = m_BoardScale;
    dstRect.height = m_BoardScale;

    DrawTexturePro(m_SnakeGraphics, srcRect, dstRect, { 0.0f, 0.0f }, 0.f, WHITE); // Draw a part of a texture defined by a rectangle with 'pro' parameters
}

int2 operator-(const int2& l, const int2& r)
{
    return int2{ l.x - r.x, l.y - r.y };
}

int abs(int x)
{
       if (x < 0)
    {
        return -x;
    }
    else
    {
        return x;
    }
}

void SnakeGame::DrawSnake()
{
    for (int i = 0; i < m_SnakeLength; i++)
    {
        // Draw Head
        if (i == 0)
        {
            const int2 prevPos = m_Snake[i + 1];
            int2 difPrevPos = m_Snake[i] - prevPos;

            if (abs(difPrevPos.x) > 1)
            {
                difPrevPos.x = -difPrevPos.x;
            }
            if (abs(difPrevPos.y) > 1)
            {
                difPrevPos.y = -difPrevPos.y;
            }

            if (difPrevPos.x > 0) // Right
            {
                DrawSprite({ 256, 0 }, m_Snake[i]);
            }
            else if (difPrevPos.x < 0) // Left
            {
                DrawSprite({ 192, 64 }, m_Snake[i]);
            }
            else if (difPrevPos.y < 0) // Up
            {
                DrawSprite({ 192, 0 }, m_Snake[i]);
            }
            else if (difPrevPos.y > 0) // Down
            {
                DrawSprite({ 256, 64 }, m_Snake[i]);
            }

        }
        // Draw Tail
        else if (i == m_SnakeLength - 1)
        {
            const int2 nextPos = m_Snake[i - 1];

            int2 difNextPos = nextPos - m_Snake[i];

            if (abs(difNextPos.x) > 1)
            {
                difNextPos.x = -difNextPos.x;
            }
            if (abs(difNextPos.y) > 1)
            {
                difNextPos.y = -difNextPos.y;
            }

            if (difNextPos.x < 0) // Left
            {
                DrawSprite({ 192, 192 }, m_Snake[i]);
            }
            else if (difNextPos.x > 0) // Right
            {
                DrawSprite({ 256, 128 }, m_Snake[i]);
            }
            else if (difNextPos.y < 0) // Up
            {
                DrawSprite({ 192, 128 }, m_Snake[i]);
            }
            else if (difNextPos.y > 0) // Down
            {
                DrawSprite({ 256, 192 }, m_Snake[i]);
            }
        }
        // Draw Body
        else
        {
            const int2 prevPos = m_Snake[i + 1];
            const int2 nextPos = m_Snake[i - 1];

            int2 difPrevPos = m_Snake[i] - prevPos;

            int2 difNextPos = nextPos - m_Snake[i];

            // Vertical an Horizontal
            if (m_Snake[i - 1].x == m_Snake[i + 1].x)
            {
                DrawSprite({ 128, 64 }, m_Snake[i]);
            }
            else if (m_Snake[i - 1].y == m_Snake[i + 1].y)
            {
                DrawSprite({ 64, 0 }, m_Snake[i]);
            }

            // Turns
            if (abs(difPrevPos.x) > 1)
            {
                difPrevPos.x = -difPrevPos.x;
            }
            if (abs(difPrevPos.y) > 1)
            {
                difPrevPos.y = -difPrevPos.y;
            }

            if (abs(difNextPos.x) > 1)
            {
                difNextPos.x = -difNextPos.x;
            }
            if (abs(difNextPos.y) > 1)
            {
                difNextPos.y = -difNextPos.y;
            }

            else if (difPrevPos.x > 0 && difNextPos.y > 0) // Turn Left-Down
            {
                DrawSprite({ 128, 0 }, m_Snake[i]);
            }
            else if (difPrevPos.x > 0 && difNextPos.y < 0)// Turn Left-uP
            {
                DrawSprite({ 128, 128 }, m_Snake[i]);
            }
            else if (difPrevPos.x < 0 && difNextPos.y > 0)// Turn Right-Down
            {
                DrawSprite({ 0, 0 }, m_Snake[i]);
            }
            else if (difPrevPos.x < 0 && difNextPos.y < 0)// Turn Right-Up
            {
                DrawSprite({ 0, 64 }, m_Snake[i]);
            }
            else if (difPrevPos.y < 0 && difNextPos.x < 0)// Turn Up-Left
            {
                DrawSprite({ 128, 0 }, m_Snake[i]);
            }
            else if (difPrevPos.y < 0 && difNextPos.x > 0)// Turn Up-Right
            {
                DrawSprite({ 0, 0 }, m_Snake[i]);
            }
            else if (difPrevPos.y > 0 && difNextPos.x < 0)// Turn Down-Left
            {
                DrawSprite({ 128, 128 }, m_Snake[i]);
            }
            else if (difPrevPos.y > 0 && difNextPos.x > 0)// Turn Down-Right
            {
                DrawSprite({ 0, 64 }, m_Snake[i]);
            }
        }
        // DrawSubPiece(SNAKE[i], 1);
    }
}

void SnakeGame::InitTextures()
{
    m_SnakeGraphics = LoadTexture(m_SpriteFilename);
}

void SnakeGame::DrawPause()
{
    constexpr int textFontSize = 80;

    const int textWidth = MeasureText("Pause", textFontSize);
    const int textX = (m_WindowSize.x - textWidth) / 2;
    DrawText("Pause", textX, 50, textFontSize, textColor);
    DrawText("Main Menu", textX / 4, 300, textFontSize - 20, textColor);
    DrawText("(M)", textX / 2, 300 + textFontSize, textFontSize - 20, textColor);
    DrawText("Resume", textX * 1.8, 300, textFontSize - 20, textColor);
    DrawText("(Space)", textX * 1.8, 300 + textFontSize, textFontSize - 20, textColor);
    constexpr int titleBottom = 50 + textFontSize;
}

void SnakeGame::DrawGameOver()
{

}