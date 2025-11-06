#include "GameUI.h"
#include <iostream>
#include <cmath>

GameUI::GameUI() 
    : window(nullptr), renderer(nullptr), font(nullptr), 
      hoveredColumn(-1), showWinMessage(false), 
      uiState(UIState::MODE_SELECTION),
      serverIP("127.0.0.1"),
      isNetworkHost(false),
      selectedGameMode(GameMode::PLAYER_VS_PLAYER),
      selectedAIDifficulty(AIDifficulty::MEDIUM),
      selectedMinimaxDepth(4) {}

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
                if (uiState == UIState::PLAYING) {
                    hoveredColumn = getColumnFromMouseX(event.motion.x);
                }
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    
                    if (uiState == UIState::MODE_SELECTION) {
                        handleModeSelectionClick(mouseX, mouseY);
                    } else if (uiState == UIState::JOIN_GAME) {
                        // Handle connect button
                        if (isMouseOverConnectButton(mouseX, mouseY)) {
                            gameClient = std::make_unique<GameClient>();
                            if (gameClient->connect(serverIP)) {
                                uiState = UIState::PLAYING;
                            } else {
                                std::cerr << "Failed to connect: " << gameClient->getLastError() << std::endl;
                                gameClient.reset();
                            }
                        }
                        // Handle back button
                        if (isMouseOverBackButton(mouseX, mouseY)) {
                            uiState = UIState::MODE_SELECTION;
                        }
                    } else if (uiState == UIState::HOST_GAME || uiState == UIState::WAITING_FOR_OPPONENT) {
                        // Handle cancel button
                        if (isMouseOverBackButton(mouseX, mouseY)) {
                            if (gameServer) {
                                gameServer->stop();
                                gameServer.reset();
                            }
                            uiState = UIState::MODE_SELECTION;
                        }
                    } else {
                        // Check if clicked on Back button
                        if (isMouseOverBackButton(mouseX, mouseY)) {
                            // Clean up network connections
                            if (gameServer) {
                                gameServer->stop();
                                gameServer.reset();
                            }
                            if (gameClient) {
                                gameClient->disconnect();
                                gameClient.reset();
                            }
                            uiState = UIState::MODE_SELECTION;
                            game.reset();
                            showWinMessage = false;
                            break;
                        }
                        
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
                        if (uiState == UIState::PLAYING && !game.isGameOver() && !game.isAITurn()) {
                            int column = getColumnFromMouseX(mouseX);
                            if (column >= 0 && column < Board::COLS) {
                                bool moveSucceeded = false;
                                
                                // Handle network moves
                                if (gameClient && gameClient->isConnected()) {
                                    moveSucceeded = gameClient->sendMove(column);
                                } else if (gameServer && gameServer->isGameStarted()) {
                                    // For now, allow host to make moves directly
                                    moveSucceeded = game.makeMove(column);
                                } else {
                                    // Local game
                                    moveSucceeded = game.makeMove(column);
                                }
                                
                                if (moveSucceeded && game.isGameOver()) {
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
    // Handle AI moves in AI mode
    if (uiState == UIState::PLAYING && !gameServer && !gameClient && game.isAITurn() && !game.isGameOver()) {
        // Add a small delay so AI moves are visible
        SDL_Delay(500);
        game.makeAIMove();
        if (game.isGameOver()) {
            showWinMessage = true;
        }
    }
    
    // Update network game state
    if (gameServer && gameServer->isGameStarted()) {
        // Check if game is over
        const Game& serverGame = gameServer->getGame();
        if (serverGame.isGameOver() && !showWinMessage) {
            showWinMessage = true;
        }
    } else if (gameClient && gameClient->isConnected()) {
        // Check if game is over
        const Game& clientGame = gameClient->getGame();
        if (clientGame.isGameOver() && !showWinMessage) {
            showWinMessage = true;
        }
    }
}

void GameUI::render() {
    // Clear screen with background color
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderClear(renderer);
    
    if (uiState == UIState::MODE_SELECTION) {
        renderModeSelection();
    } else if (uiState == UIState::HOST_GAME) {
        renderHostGameScreen();
    } else if (uiState == UIState::JOIN_GAME) {
        renderJoinGameScreen();
    } else if (uiState == UIState::WAITING_FOR_OPPONENT) {
        renderWaitingScreen();
    } else {
        // Render game elements
        renderPlayerTurn();
        renderBoard();
        renderPieces();
        renderButtons();
        
        if (showWinMessage) {
            renderWinMessage();
        }
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
    const Game& currentGame = getCurrentGame();
    const Board& board = currentGame.getBoard();
    
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
    
    const Game& currentGame = getCurrentGame();
    
    char turnText[100];
    if (currentGame.isGameOver()) {
        char winner = currentGame.getWinner();
        if (winner == ' ') {
            snprintf(turnText, sizeof(turnText), "It's a Draw!");
        } else {
            snprintf(turnText, sizeof(turnText), "Player %c Wins!", winner);
        }
    } else {
        snprintf(turnText, sizeof(turnText), "Player %c's Turn", currentGame.getCurrentPlayer());
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
    SDL_Color backColor = {150, 150, 150, 255};
    
    // Back button (left side)
    drawButton(50, WINDOW_HEIGHT - 80, BUTTON_WIDTH, BUTTON_HEIGHT, "Back", backColor);
    
    // New Game button (center)
    drawButton(WINDOW_WIDTH / 2 - BUTTON_WIDTH / 2, WINDOW_HEIGHT - 80, BUTTON_WIDTH, BUTTON_HEIGHT, "New Game", buttonColor);
    
    // Quit button (right side)
    drawButton(WINDOW_WIDTH - BUTTON_WIDTH - 50, WINDOW_HEIGHT - 80, BUTTON_WIDTH, BUTTON_HEIGHT, "Quit", quitColor);
}

int GameUI::getColumnFromMouseX(int mouseX) {
    if (mouseX < BOARD_OFFSET_X || mouseX >= BOARD_OFFSET_X + Board::COLS * CELL_SIZE) {
        return -1;
    }
    return (mouseX - BOARD_OFFSET_X) / CELL_SIZE;
}

bool GameUI::isMouseOverNewGameButton(int mouseX, int mouseY) {
    return mouseX >= WINDOW_WIDTH / 2 - BUTTON_WIDTH / 2 && 
           mouseX <= WINDOW_WIDTH / 2 + BUTTON_WIDTH / 2 &&
           mouseY >= WINDOW_HEIGHT - 80 && 
           mouseY <= WINDOW_HEIGHT - 80 + BUTTON_HEIGHT;
}

bool GameUI::isMouseOverQuitButton(int mouseX, int mouseY) {
    return mouseX >= WINDOW_WIDTH - BUTTON_WIDTH - 50 && 
           mouseX <= WINDOW_WIDTH - 50 &&
           mouseY >= WINDOW_HEIGHT - 80 && 
           mouseY <= WINDOW_HEIGHT - 80 + BUTTON_HEIGHT;
}

bool GameUI::isMouseOverBackButton(int mouseX, int mouseY) {
    return mouseX >= 50 && mouseX <= 50 + BUTTON_WIDTH &&
           mouseY >= WINDOW_HEIGHT - 80 && mouseY <= WINDOW_HEIGHT - 80 + BUTTON_HEIGHT;
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

void GameUI::renderModeSelection() {
    SDL_Color titleColor = {0, 0, 0, 255};
    SDL_Color buttonColor = {100, 150, 255, 255};
    SDL_Color networkColor = {150, 100, 255, 255};
    SDL_Color selectedColor = {0, 200, 0, 255};
    SDL_Color startColor = {0, 150, 0, 255};
    
    // Title
    renderText("Connect 4 - Game Mode Selection", WINDOW_WIDTH / 2 - 200, 30, titleColor);
    
    // Game Mode buttons
    renderText("Select Game Mode:", 100, 100, titleColor);
    
    SDL_Color pvpColor = (selectedGameMode == GameMode::PLAYER_VS_PLAYER) ? selectedColor : buttonColor;
    SDL_Color pvaiColor = (selectedGameMode == GameMode::PLAYER_VS_AI) ? selectedColor : buttonColor;
    
    drawButton(100, 140, 200, 50, "Player vs Player", pvpColor);
    drawButton(320, 140, 200, 50, "Player vs AI", pvaiColor);
    
    // Network multiplayer buttons
    renderText("Network Multiplayer:", 100, 220, titleColor);
    drawButton(100, 260, 200, 50, "Host Game", networkColor);
    drawButton(320, 260, 200, 50, "Join Game", networkColor);
    
    // AI Difficulty selection (only if Player vs AI is selected)
    if (selectedGameMode == GameMode::PLAYER_VS_AI) {
        renderText("Select AI Difficulty:", 100, 340, titleColor);
        
        SDL_Color easyColor = (selectedAIDifficulty == AIDifficulty::EASY) ? selectedColor : buttonColor;
        SDL_Color medColor = (selectedAIDifficulty == AIDifficulty::MEDIUM) ? selectedColor : buttonColor;
        SDL_Color hardColor = (selectedAIDifficulty == AIDifficulty::HARD) ? selectedColor : buttonColor;
        
        drawButton(100, 380, 150, 50, "Easy", easyColor);
        drawButton(270, 380, 150, 50, "Medium", medColor);
        drawButton(440, 380, 150, 50, "Hard", hardColor);
        
        // Show depth slider for Hard mode
        if (selectedAIDifficulty == AIDifficulty::HARD) {
            renderText("Minimax Depth:", 100, 460, titleColor);
            
            char depthText[32];
            snprintf(depthText, sizeof(depthText), "%d", selectedMinimaxDepth);
            renderText(depthText, 250, 460, titleColor);
            
            // Draw slider
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_Rect sliderBg = {100, 500, 400, 10};
            SDL_RenderFillRect(renderer, &sliderBg);
            
            // Draw slider handle
            int handleX = 100 + (selectedMinimaxDepth - 1) * 400 / 7;
            SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
            SDL_Rect handle = {handleX - 5, 490, 10, 30};
            SDL_RenderFillRect(renderer, &handle);
            
            renderText("1", 90, 515, titleColor);
            renderText("8", 505, 515, titleColor);
        }
    }
    
    // Start button (only for local modes)
    if (selectedGameMode != GameMode::NETWORK_HOST && selectedGameMode != GameMode::NETWORK_CLIENT) {
        drawButton(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT - 150, 200, 60, "Start Game", startColor);
    }
}

void GameUI::handleModeSelectionClick(int mouseX, int mouseY) {
    // Check game mode buttons
    if (isMouseOverPvPButton(mouseX, mouseY)) {
        selectedGameMode = GameMode::PLAYER_VS_PLAYER;
    } else if (isMouseOverPvAIButton(mouseX, mouseY)) {
        selectedGameMode = GameMode::PLAYER_VS_AI;
    }
    
    // Check network buttons
    if (isMouseOverHostGameButton(mouseX, mouseY)) {
        uiState = UIState::HOST_GAME;
        isNetworkHost = true;
        // Initialize server
        gameServer = std::make_unique<GameServer>();
        if (gameServer->start()) {
            uiState = UIState::WAITING_FOR_OPPONENT;
        } else {
            std::cerr << "Failed to start server: " << gameServer->getLastError() << std::endl;
            gameServer.reset();
            uiState = UIState::MODE_SELECTION;
        }
        return;
    } else if (isMouseOverJoinGameButton(mouseX, mouseY)) {
        uiState = UIState::JOIN_GAME;
        isNetworkHost = false;
        return;
    }
    
    // Check AI difficulty buttons (only if PvAI is selected)
    if (selectedGameMode == GameMode::PLAYER_VS_AI) {
        if (isMouseOverEasyButton(mouseX, mouseY)) {
            selectedAIDifficulty = AIDifficulty::EASY;
        } else if (isMouseOverMediumButton(mouseX, mouseY)) {
            selectedAIDifficulty = AIDifficulty::MEDIUM;
        } else if (isMouseOverHardButton(mouseX, mouseY)) {
            selectedAIDifficulty = AIDifficulty::HARD;
        }
        
        // Check depth slider (only for Hard mode)
        if (selectedAIDifficulty == AIDifficulty::HARD && isMouseOverDepthSlider(mouseX, mouseY)) {
            updateDepthFromMouse(mouseX);
        }
    }
    
    // Check start button
    if (isMouseOverStartButton(mouseX, mouseY)) {
        game.setGameMode(selectedGameMode);
        if (selectedGameMode == GameMode::PLAYER_VS_AI) {
            game.setAIDifficulty(selectedAIDifficulty);
            if (selectedAIDifficulty == AIDifficulty::HARD) {
                game.setMinimaxDepth(selectedMinimaxDepth);
            }
        }
        game.reset();
        uiState = UIState::PLAYING;
    }
}

bool GameUI::isMouseOverPvPButton(int mouseX, int mouseY) {
    return mouseX >= 100 && mouseX <= 300 && mouseY >= 140 && mouseY <= 190;
}

bool GameUI::isMouseOverPvAIButton(int mouseX, int mouseY) {
    return mouseX >= 320 && mouseX <= 520 && mouseY >= 140 && mouseY <= 190;
}

bool GameUI::isMouseOverHostGameButton(int mouseX, int mouseY) {
    return mouseX >= 100 && mouseX <= 300 && mouseY >= 260 && mouseY <= 310;
}

bool GameUI::isMouseOverJoinGameButton(int mouseX, int mouseY) {
    return mouseX >= 320 && mouseX <= 520 && mouseY >= 260 && mouseY <= 310;
}

bool GameUI::isMouseOverEasyButton(int mouseX, int mouseY) {
    return mouseX >= 100 && mouseX <= 250 && mouseY >= 380 && mouseY <= 430;
}

bool GameUI::isMouseOverMediumButton(int mouseX, int mouseY) {
    return mouseX >= 270 && mouseX <= 420 && mouseY >= 380 && mouseY <= 430;
}

bool GameUI::isMouseOverHardButton(int mouseX, int mouseY) {
    return mouseX >= 440 && mouseX <= 590 && mouseY >= 380 && mouseY <= 430;
}

bool GameUI::isMouseOverStartButton(int mouseX, int mouseY) {
    return mouseX >= WINDOW_WIDTH / 2 - 100 && mouseX <= WINDOW_WIDTH / 2 + 100 &&
           mouseY >= WINDOW_HEIGHT - 150 && mouseY <= WINDOW_HEIGHT - 90;
}

bool GameUI::isMouseOverConnectButton(int mouseX, int mouseY) {
    return mouseX >= WINDOW_WIDTH / 2 - 100 && mouseX <= WINDOW_WIDTH / 2 + 100 &&
           mouseY >= 350 && mouseY <= 410;
}

bool GameUI::isMouseOverDepthSlider(int mouseX, int mouseY) {
    return mouseX >= 100 && mouseX <= 500 && mouseY >= 490 && mouseY <= 520;
}

void GameUI::updateDepthFromMouse(int mouseX) {
    int relativeX = mouseX - 100;
    if (relativeX < 0) relativeX = 0;
    if (relativeX > 400) relativeX = 400;
    
    selectedMinimaxDepth = 1 + (relativeX * 7) / 400;
    if (selectedMinimaxDepth < 1) selectedMinimaxDepth = 1;
    if (selectedMinimaxDepth > 8) selectedMinimaxDepth = 8;
}

void GameUI::renderHostGameScreen() {
    SDL_Color titleColor = {0, 0, 0, 255};
    SDL_Color buttonColor = {150, 50, 50, 255};
    
    renderText("Host Game", WINDOW_WIDTH / 2 - 60, 50, titleColor);
    renderText("Waiting for opponent to connect...", WINDOW_WIDTH / 2 - 180, 150, titleColor);
    
    if (gameServer) {
        std::string ipText = "Server IP: " + gameServer->getIPAddress();
        renderText(ipText.c_str(), WINDOW_WIDTH / 2 - 150, 250, titleColor);
        
        char portText[64];
        snprintf(portText, sizeof(portText), "Port: %d", gameServer->getPort());
        renderText(portText, WINDOW_WIDTH / 2 - 50, 300, titleColor);
        
        char playersText[64];
        snprintf(playersText, sizeof(playersText), "Players connected: %d/2", gameServer->getConnectedPlayerCount());
        renderText(playersText, WINDOW_WIDTH / 2 - 120, 350, titleColor);
    }
    
    drawButton(WINDOW_WIDTH / 2 - 75, WINDOW_HEIGHT - 150, 150, 50, "Cancel", buttonColor);
}

void GameUI::renderJoinGameScreen() {
    SDL_Color titleColor = {0, 0, 0, 255};
    SDL_Color buttonColor = {100, 150, 255, 255};
    SDL_Color backColor = {150, 50, 50, 255};
    
    renderText("Join Game", WINDOW_WIDTH / 2 - 60, 50, titleColor);
    renderText("Enter server IP address:", 100, 150, titleColor);
    
    // Simple IP display (not editable in this simple version)
    char ipText[128];
    snprintf(ipText, sizeof(ipText), "Server IP: %s", serverIP.c_str());
    renderText(ipText, 100, 200, titleColor);
    renderText("(Using default: 127.0.0.1)", 100, 250, titleColor);
    
    drawButton(WINDOW_WIDTH / 2 - 100, 350, 200, 60, "Connect", buttonColor);
    drawButton(WINDOW_WIDTH / 2 - 75, WINDOW_HEIGHT - 150, 150, 50, "Back", backColor);
}

void GameUI::renderWaitingScreen() {
    SDL_Color titleColor = {0, 0, 0, 255};
    
    renderText("Waiting for opponent...", WINDOW_WIDTH / 2 - 140, WINDOW_HEIGHT / 2 - 20, titleColor);
    
    if (gameServer && gameServer->getConnectedPlayerCount() >= 2) {
        // Transition to game
        uiState = UIState::PLAYING;
    }
}

const Game& GameUI::getCurrentGame() const {
    if (gameServer && gameServer->isGameStarted()) {
        return gameServer->getGame();
    } else if (gameClient && gameClient->isConnected()) {
        return gameClient->getGame();
    }
    return game;
}

