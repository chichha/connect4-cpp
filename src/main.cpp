#include "GameUI.h"
#include <iostream>

int main() {
    GameUI gameUI;
    
    if (!gameUI.init()) {
        std::cerr << "Failed to initialize game UI!" << std::endl;
        return 1;
    }
    
    gameUI.run();
    gameUI.cleanup();
    
    return 0;
}
