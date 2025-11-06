#ifndef GAMEUI_H
#define GAMEUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Game.h"

class GameUI {
public:
    GameUI();
    ~GameUI();
    
    bool init();
    void run();
    void cleanup();
    
private:
    // SDL components
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    
    // Game state
    Game game;
    int hoveredColumn;
    bool showWinMessage;
    
    // UI constants
    static const int WINDOW_WIDTH = 800;
    static const int WINDOW_HEIGHT = 700;
    static const int BOARD_OFFSET_X = 50;
    static const int BOARD_OFFSET_Y = 150;
    static const int CELL_SIZE = 90;
    static const int PIECE_RADIUS = 35;
    static const int BUTTON_WIDTH = 150;
    static const int BUTTON_HEIGHT = 50;
    
    // Helper methods
    void handleEvents(bool& running);
    void update();
    void render();
    
    void renderBoard();
    void renderPieces();
    void renderPlayerTurn();
    void renderWinMessage();
    void renderButtons();
    
    int getColumnFromMouseX(int mouseX);
    bool isMouseOverNewGameButton(int mouseX, int mouseY);
    bool isMouseOverQuitButton(int mouseX, int mouseY);
    
    void drawFilledCircle(int centerX, int centerY, int radius, SDL_Color color);
    void drawButton(int x, int y, int width, int height, const char* text, SDL_Color bgColor);
    void renderText(const char* text, int x, int y, SDL_Color color);
};

#endif // GAMEUI_H
