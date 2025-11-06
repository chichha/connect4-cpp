#include "GameUI.h"
#include <iostream>
#include <cmath>

GameUI::GameUI() 
    : window(nullptr), renderer(nullptr), font(nullptr), 
      hoveredColumn(-1), showWinMessage(false) {}

GameUI::~GameUI() {
    cleanup();
}

bool GameUI::init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Initialize SDL_ttf
    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    
    // Create window
    window = SDL_CreateWindow(
        "Connect 4",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return false;
    }
    
    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }
    
    // Load font - try to use a system font
    font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 24);
    if (!font) {
        // Try alternative font path for different systems
        font = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSans-Bold.ttf", 24);
    }
    if (!font) {
        // Try Windows font path
        font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);
    }
    if (!font) {
        std::cerr << "Font loading failed: " << TTF_GetError() << std::endl;
        std::cerr << "Continuing without text rendering..." << std::endl;
    }
    
    return true;
}

void GameUI::run() {
    bool running = true;
    
    while (running) {
        handleEvents(running);
        update();
        render();
        
        SDL_Delay(16); // Cap at ~60 FPS
    }
}

void GameUI::cleanup() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    TTF_Quit();
    SDL_Quit();
}

void GameUI::handleEvents(bool& running) {
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
                
            case SDL_MOUSEMOTION:
                hoveredColumn = getColumnFromMouseX(event.motion.x);
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    
                    // Check if clicked on New Game button
                    if (isMouseOverNewGameButton(mouseX, mouseY)) {
                        game.reset();
                        showWinMessage = false;
                        break;
                    }
                    
                    // Check if clicked on Quit button
                    if (isMouseOverQuitButton(mouseX, mouseY)) {
                        running = false;
                        break;
                    }
                    
                    // Check if clicked on board to make a move
                    if (!game.isGameOver()) {
                        int column = getColumnFromMouseX(mouseX);
                        if (column >= 0 && column < Board::COLS) {
                            if (game.makeMove(column)) {
                                if (game.isGameOver()) {
                                    showWinMessage = true;
                                }
                            }
                        }
                    }
                }
                break;
        }
    }
}

void GameUI::update() {
    // Update logic here if needed
}

void GameUI::render() {
    // Clear screen with background color
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderClear(renderer);
    
    // Render game elements
    renderPlayerTurn();
    renderBoard();
    renderPieces();
    renderButtons();
    
    if (showWinMessage) {
        renderWinMessage();
    }
    
    // Present
    SDL_RenderPresent(renderer);
}

void GameUI::renderBoard() {
    // Draw board background (blue)
    SDL_SetRenderDrawColor(renderer, 0, 102, 204, 255);
    SDL_Rect boardRect = {
        BOARD_OFFSET_X - 10,
        BOARD_OFFSET_Y - 10,
        Board::COLS * CELL_SIZE + 20,
        Board::ROWS * CELL_SIZE + 20
    };
    SDL_RenderFillRect(renderer, &boardRect);
    
    // Draw column highlight if hovering
    if (hoveredColumn >= 0 && hoveredColumn < Board::COLS && !game.isGameOver()) {
        SDL_SetRenderDrawColor(renderer, 100, 150, 255, 100);
        SDL_Rect highlightRect = {
            BOARD_OFFSET_X + hoveredColumn * CELL_SIZE,
            BOARD_OFFSET_Y,
            CELL_SIZE,
            Board::ROWS * CELL_SIZE
        };
        SDL_RenderFillRect(renderer, &highlightRect);
    }
    
    // Draw grid lines and empty cells
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int row = 0; row < Board::ROWS; row++) {
        for (int col = 0; col < Board::COLS; col++) {
            int centerX = BOARD_OFFSET_X + col * CELL_SIZE + CELL_SIZE / 2;
            int centerY = BOARD_OFFSET_Y + row * CELL_SIZE + CELL_SIZE / 2;
            
            // Draw empty cell (white circle)
            SDL_Color whiteColor = {255, 255, 255, 255};
            drawFilledCircle(centerX, centerY, PIECE_RADIUS, whiteColor);
        }
    }
}

void GameUI::renderPieces() {
    const Board& board = game.getBoard();
    
    for (int row = 0; row < Board::ROWS; row++) {
        for (int col = 0; col < Board::COLS; col++) {
            char cell = board.getCell(row, col);
            
            if (cell != ' ') {
                int centerX = BOARD_OFFSET_X + col * CELL_SIZE + CELL_SIZE / 2;
                int centerY = BOARD_OFFSET_Y + row * CELL_SIZE + CELL_SIZE / 2;
                
                SDL_Color color;
                if (cell == 'X') {
                    color = {255, 0, 0, 255}; // Red for Player 1
                } else {
                    color = {255, 255, 0, 255}; // Yellow for Player 2
                }
                
                drawFilledCircle(centerX, centerY, PIECE_RADIUS, color);
            }
        }
    }
}

void GameUI::renderPlayerTurn() {
    if (!font) return;
    
    char turnText[100];
    if (game.isGameOver()) {
        char winner = game.getWinner();
        if (winner == ' ') {
            snprintf(turnText, sizeof(turnText), "It's a Draw!");
        } else {
            snprintf(turnText, sizeof(turnText), "Player %c Wins!", winner);
        }
    } else {
        snprintf(turnText, sizeof(turnText), "Player %c's Turn", game.getCurrentPlayer());
    }
    
    SDL_Color textColor = {0, 0, 0, 255};
    renderText(turnText, WINDOW_WIDTH / 2 - 100, 30, textColor);
}

void GameUI::renderWinMessage() {
    if (!font) return;
    
    // Draw semi-transparent overlay
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_Rect overlayRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &overlayRect);
    
    // Draw message box
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect messageBox = {
        WINDOW_WIDTH / 2 - 200,
        WINDOW_HEIGHT / 2 - 100,
        400,
        200
    };
    SDL_RenderFillRect(renderer, &messageBox);
    
    // Draw border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &messageBox);
    
    // Render text
    char winText[100];
    char winner = game.getWinner();
    if (winner == ' ') {
        snprintf(winText, sizeof(winText), "It's a Draw!");
    } else {
        snprintf(winText, sizeof(winText), "Player %c Wins!", winner);
    }
    
    SDL_Color textColor = {0, 0, 0, 255};
    renderText(winText, WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 - 40, textColor);
    renderText("Click 'New Game' to play again", WINDOW_WIDTH / 2 - 130, WINDOW_HEIGHT / 2 + 20, textColor);
}

void GameUI::renderButtons() {
    SDL_Color buttonColor = {100, 200, 100, 255};
    SDL_Color quitColor = {200, 100, 100, 255};
    
    // New Game button
    drawButton(50, WINDOW_HEIGHT - 80, BUTTON_WIDTH, BUTTON_HEIGHT, "New Game", buttonColor);
    
    // Quit button
    drawButton(WINDOW_WIDTH - BUTTON_WIDTH - 50, WINDOW_HEIGHT - 80, BUTTON_WIDTH, BUTTON_HEIGHT, "Quit", quitColor);
}

int GameUI::getColumnFromMouseX(int mouseX) {
    if (mouseX < BOARD_OFFSET_X || mouseX >= BOARD_OFFSET_X + Board::COLS * CELL_SIZE) {
        return -1;
    }
    return (mouseX - BOARD_OFFSET_X) / CELL_SIZE;
}

bool GameUI::isMouseOverNewGameButton(int mouseX, int mouseY) {
    return mouseX >= 50 && mouseX <= 50 + BUTTON_WIDTH &&
           mouseY >= WINDOW_HEIGHT - 80 && mouseY <= WINDOW_HEIGHT - 80 + BUTTON_HEIGHT;
}

bool GameUI::isMouseOverQuitButton(int mouseX, int mouseY) {
    return mouseX >= WINDOW_WIDTH - BUTTON_WIDTH - 50 && 
           mouseX <= WINDOW_WIDTH - 50 &&
           mouseY >= WINDOW_HEIGHT - 80 && 
           mouseY <= WINDOW_HEIGHT - 80 + BUTTON_HEIGHT;
}

void GameUI::drawFilledCircle(int centerX, int centerY, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

void GameUI::drawButton(int x, int y, int width, int height, const char* text, SDL_Color bgColor) {
    // Draw button background
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_Rect buttonRect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &buttonRect);
    
    // Draw button border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &buttonRect);
    
    // Draw button text
    if (font) {
        SDL_Color textColor = {255, 255, 255, 255};
        renderText(text, x + 20, y + 15, textColor);
    }
}

void GameUI::renderText(const char* text, int x, int y, SDL_Color color) {
    if (!font) return;
    
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        return;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }
    
    SDL_Rect destRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
