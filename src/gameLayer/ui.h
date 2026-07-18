#pragma once
#include <raylib.h>
#include <assetManager.h>

extern AssetManager assetManager;

enum class GameState {
    MainMenu,
    Playing,
    Paused
};

extern GameState currentGameState;

// Function to draw the main menu
void DrawMainMenu();

// Function to draw the pause menu
void DrawPauseMenu();
