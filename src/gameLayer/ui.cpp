#include "ui.h"
#include <gameMain.h>
#include <saveGame.h>
#include <iostream>

GameState currentGameState = GameState::MainMenu;

bool DrawButton(const char* text, Rectangle bounds) {
    bool clicked = false;
    Vector2 mousePoint = GetMousePosition();
    
    if (CheckCollisionPointRec(mousePoint, bounds)) {
        DrawRectangleRec(bounds, LIGHTGRAY);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            clicked = true;
        }
    } else {
        DrawRectangleRec(bounds, DARKGRAY);
    }
    
    DrawRectangleLinesEx(bounds, 2, BLACK);
    
    int textWidth = MeasureText(text, 20);
    DrawText(text, bounds.x + bounds.width / 2 - textWidth / 2, bounds.y + bounds.height / 2 - 10, 20, WHITE);
    
    return clicked;
}

void DrawMainMenu() {
    // Draw background
    DrawTexturePro(
        assetManager.uiBg,
        { 0, 0, (float)assetManager.uiBg.width, (float)assetManager.uiBg.height },
        { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        { 0, 0 },
        0.0f,
        WHITE
    );
    
    float screenWidth = GetScreenWidth();
    float screenHeight = GetScreenHeight();
    
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;
    float startX = screenWidth / 2 - buttonWidth / 2;
    float startY = screenHeight / 2 - 100.0f;
    float spacing = 20.0f;
    
    int titleWidth = MeasureText("Terraria Clone", 40);
    DrawText("Terraria Clone", screenWidth / 2 - titleWidth / 2, startY - 80, 40, WHITE);
    
    if (DrawButton("New Game", { startX, startY, buttonWidth, buttonHeight })) {
        resetGame();
        currentGameState = GameState::Playing;
    }
    
    if (DrawButton("Load Game", { startX, startY + buttonHeight + spacing, buttonWidth, buttonHeight })) {
        loadGame();
        currentGameState = GameState::Playing;
    }
    
    if (DrawButton("Top Scores", { startX, startY + 2 * (buttonHeight + spacing), buttonWidth, buttonHeight })) {
        // Top Scores logic here
    }
    
    if (DrawButton("Help", { startX, startY + 3 * (buttonHeight + spacing), buttonWidth, buttonHeight })) {
        // Help logic here
    }
}

void DrawPauseMenu() {
    // Draw background (could be slightly transparent, but instructions said UI_bg.png)
    DrawTexturePro(
        assetManager.uiBg,
        { 0, 0, (float)assetManager.uiBg.width, (float)assetManager.uiBg.height },
        { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        { 0, 0 },
        0.0f,
        WHITE
    );
    
    float screenWidth = GetScreenWidth();
    float screenHeight = GetScreenHeight();
    
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;
    float startX = screenWidth / 2 - buttonWidth / 2;
    float startY = screenHeight / 2 - 120.0f;
    float spacing = 20.0f;
    
    int titleWidth = MeasureText("Paused", 40);
    DrawText("Paused", screenWidth / 2 - titleWidth / 2, startY - 80, 40, WHITE);
    
    if (DrawButton("Resume", { startX, startY, buttonWidth, buttonHeight })) {
        currentGameState = GameState::Playing;
    }
    
    if (DrawButton("New Game", { startX, startY + buttonHeight + spacing, buttonWidth, buttonHeight })) {
        resetGame();
        currentGameState = GameState::Playing;
    }
    
    if (DrawButton("Save & Exit", { startX, startY + 2 * (buttonHeight + spacing), buttonWidth, buttonHeight })) {
        saveGame();
        currentGameState = GameState::MainMenu;
    }
    
    if (DrawButton("Top Scores", { startX, startY + 3 * (buttonHeight + spacing), buttonWidth, buttonHeight })) {
        // Top Scores logic here
    }
    
    if (DrawButton("Help", { startX, startY + 4 * (buttonHeight + spacing), buttonWidth, buttonHeight })) {
        // Help logic here
    }
}
