#include "GameUI.h"
#include <iostream>

int main(int argc, char* argv[]) {
    // Suppress unused parameter warnings
    (void)argc;
    (void)argv;
    
    GameUI gameUI;
    
    if (!gameUI.init()) {
        std::cerr << "Failed to initialize game UI!" << std::endl;
        return 1;
    }
    
    gameUI.run();
    gameUI.cleanup();
    
    return 0;
}
