module GameController;

import CommandInterpreter;
import Command;
import Player;
import Block;
import Board;
import IDisplay;
import Level;
import <iostream>;
import <vector>;
import <fstream>;
import <cstdlib>;
import <string>;

using namespace std;

GameController::GameController(Player* p1, Player* p2, CommandInterpreter* ci, int seed, IDisplay* display)
    : p1{p1}, p2{p2}, current{p1}, ci{ci}, display{display},hiScore{0}, gameOver{false}, randomSeed{seed} {
    if (seed != 0) {
        srand(seed);
    }
}

Player* GameController::getOpponent() {
    return (current == p1) ? p2 : p1;
}

void GameController::run() {
    bool endedByEOF = false;
    int count = 0;

    // Main game loop
    while (!gameOver) {
        // Display the game state
        if (count == 0) display->render(*p1, *p2);
        
        // Read and process command
        string cmdStr = ci->readNextCommand();
        
        if (cmdStr.empty()) {
            // EOF received, exit game gracefully
            endedByEOF = true;
            break;
        }
        
        processCommand(cmdStr);
        ++count;
    }

    // Game End Message
    string summary = "Final scores - Player 1: " + to_string(p1->getScore()) +
                     ", Player 2: " + to_string(p2->getScore());

    if (gameOver) {
        display->message("Game over! " + summary);
    } else if (endedByEOF) {
        display->message("Input ended. " + summary);
    } else {
        display->message("Game ended. " + summary);
    }
}

void GameController::processCommand(const string& cmdStr) {
    // Parse command with multiplier support
    vector<Command*> commands = ci->parseWithMultiplier(cmdStr);
    
    // Execute each command
    for (size_t i = 0; i < commands.size(); ++i) {
        if (commands[i]) {
            commands[i]->execute(*this);
            delete commands[i];  // Clean up
            
            // Check if game is over after each command
            if (gameOver) {
                // Clean up remaining commands
                for (size_t j = i + 1; j < commands.size(); ++j) {
                    delete commands[j];
                }
                break;
            }
        }
    }
    
    // Redraw after all commands executed
    display->render(*p1, *p2);
}

void GameController::startNewGame(int startLevel) {
    gameOver = false;
    
    // Reset both players
    p1->reset(startLevel);
    p2->reset(startLevel);
    
    // Spawn initial blocks for both players
    p1->spawnInitialBlocks();
    p2->spawnInitialBlocks();
    
    // Start with player 1
    current = p1;
    
    display->message("New game started at level " + to_string(startLevel) + ". Player 1 moves first.");
}

void GameController::restart() {
    startNewGame(0);  // Restart at level 0
    // Note: hiScore does not reset
    display->message("Game restarted at level 0. Hi score is preserved: " + to_string(hiScore) + ".");

}

void GameController::switchTurn() {
    current = getOpponent();
}

void GameController::onBlockLocked(int rowsCleared) {
    // Update score based on rows cleared
    if (rowsCleared > 0) {
        current->updateScore(rowsCleared);
    }
    
    // Check if any blocks were fully removed from board
    current->checkAndScoreCompletedBlocks();
    
    // Update hi score if needed
    if (current->getScore() > hiScore) {
        hiScore = current->getScore();
        display->message("New high score: " + to_string(hiScore) + "!");
    }
    
    // If rows cleared
    if (rowsCleared > 0) {
        display->message("Block locked: cleared " + to_string(rowsCleared) + " line(s).");
    } else {
        display->message("Block locked: no lines cleared.");
    }

    // Trigger special action if 2+ rows cleared
    if (rowsCleared >= 2) {
        triggerSpecialAction(current, getOpponent(), rowsCleared);
    }
    
    // Spawn next block for current player
    current->spawnNextBlock();
    
    // Check if current block can be placed (game over check)
    if (!current->canPlaceCurrentBlock()) {
        gameOver = true;
        string winner = (current == p1) ? "Player 2" : "Player 1";
        display->message(winner + " wins! No space for the next block.");
        return;
    }
    
    // Switch turns
    switchTurn();
}

void GameController::triggerSpecialAction(Player* attacker, Player* defender, int rows) {
    display->message(
    std::string("Special action! (cleared ") + to_string(rows) + " row(s)). Choose action: blind / heavy / force <block>.");

    string action;
    if (!getline(cin, action)) {
        display->message("No special action is chosen. Skiped");
        return;
    }
    
    if (action == "blind") {
        defender->applyBlindEffect();
        display->message("Special: BLIND applied to opponent's board.");
    } 
    else if (action == "heavy") {
        defender->applyHeavyEffect();
        display->message("Special: HEAVY applied. Opponent's blocks will fall faster.");
    } 
    else if (action.find("force") == 0) {
        // Extract block type: "force Z" -> 'Z'
        if (action.length() >= 7) {
            char blockType = action[6];
            defender->applyForceEffect(blockType);
            display->message(std::string("Special: FORCE applied. Opponent's next block is ") + blockType +".");

            
            // Check if forced block can be placed
            if (!defender->canPlaceCurrentBlock()) {
                gameOver = true;
                string winner = (defender == p1) ? "Player 2" : "Player 1";
                display->message(winner + " wins! Forced block could not be placed.");
            }
        } else {
            display->message("Invalid force command. Use: force <block_type>.");
        }
    } else {
        display->message("Invalid special action: '" + action + "'. No special effect applied.");
    }
}

// Helper function to handle heavy block logic (level 3+ or special action heavy)
void GameController::moveLeft() {
    Block* block = current->getCurrentBlock();
    if (!block) return;
    
    Board& board = current->getBoard();
    block->moveLeft();    
    if (!board.canPlace(*block)) {
        block->moveRight();  // Undo if invalid
        display->message("Invalid move: cannot move current block further left.");
        return;
    }
    
    // Heavy effect from special action: drop 2 rows
    if (current->hasHeavyEffect()) {
        for (int i = 0; i < 2; ++i) {
            block->moveDown();
            if (!board.canPlace(*block)) {
                block->moveUp();
                // Block is locked if it can't move down
                int rows = current->lockCurrentBlock();
                display->message("Heavy effect: block locked after falling.");
                onBlockLocked(rows);
                return;
            }
        }
    }
    
    // Heavy blocks from level 3+: automatically fall 1 row after movement
    if (current->getLevel() >= 3) {
        block->moveDown();
        if (!board.canPlace(*block)) {
            block->moveUp();
            int rows = current->lockCurrentBlock();
            display->message("Auto-drop: block locked after falling.");
            onBlockLocked(rows);
        }
    }
}

void GameController::moveRight() {
    Block* block = current->getCurrentBlock();
    if (!block) return;
    
    Board& board = current->getBoard();
    
    block->moveRight();
    if (!board.canPlace(*block)) {
        block->moveLeft();  // Undo if invalid
        display->message("Invalid move: cannot move current block further right.");
        return;
    }
    
    // Heavy effect from special action: drop 2 rows
    if (current->hasHeavyEffect()) {
        for (int i = 0; i < 2; ++i) {
            block->moveDown();
            if (!board.canPlace(*block)) {
                block->moveUp();
                int rows = current->lockCurrentBlock();
                display->message("Heavy effect: block locked after falling.");
                onBlockLocked(rows);
                return;
            }
        }
    }
    
    // Heavy blocks from level 3+: automatically fall 1 row after movement
    if (current->getLevel() >= 3) {
        block->moveDown();
        if (!board.canPlace(*block)) {
            block->moveUp();
            int rows = current->lockCurrentBlock();
            display->message("Auto-drop: block locked after falling.");
            onBlockLocked(rows);
        }
    }
}

void GameController::rotateCW() {
    Block* block = current->getCurrentBlock();
    if (!block) return;
    
    Board& board = current->getBoard();
    
    block->rotateCW();
    if (!board.canPlace(*block)) {
        block->rotateCCW();  // Undo if invalid
        display->message("Invalid rotation: cannot rotate block clockwise here.");
        return;
    }
    
    // Heavy blocks from level 3+: automatically fall 1 row after rotation
    if (current->getLevel() >= 3) {
        block->moveDown();
        if (!board.canPlace(*block)) {
            block->moveUp();
            int rows = current->lockCurrentBlock();
            display->message("Auto-drop: block locked after rotation.");
            onBlockLocked(rows);
        }
    }
}

void GameController::rotateCCW() {
    Block* block = current->getCurrentBlock();
    if (!block) return;
    
    Board& board = current->getBoard();
    
    block->rotateCCW();
    if (!board.canPlace(*block)) {
        block->rotateCW();  // Undo if invalid
        display->message("Invalid rotation: cannot rotate block counter-clockwise here.");
        return;
    }
    

    // Heavy blocks from level 3+: automatically fall 1 row after rotation
    if (current->getLevel() >= 3) {
        block->moveDown();
        if (!board.canPlace(*block)) {
            block->moveUp();
            int rows = current->lockCurrentBlock();
            display->message("Auto-drop: block locked after rotation.");
            onBlockLocked(rows);
        }
    }
    ++block->countCCW;
}

void GameController::moveDown() {
    Block* block = current->getCurrentBlock();
    if (!block) return;
    
    Board& board = current->getBoard();
    
    block->moveDown();
    if (!board.canPlace(*block)) {
        block->moveUp();
        // Don't lock on down command, only drop locks
        display->message("Invalid move: block cannot move further down. Use 'drop' to lock it.");
    }
}

void GameController::drop() {
    Block* block = current->getCurrentBlock();
    if (!block) return;
    
    Board& board = current->getBoard();
    
    // Drop until collision
    while (true) {
        block->moveDown();
        if (!board.canPlace(*block)) {
            block->moveUp();  // Undo last invalid move
            break;
        }
    }
    
    // Lock the block
    int rows = current->lockCurrentBlock();
    display->message("Block dropped and locked.");
    onBlockLocked(rows);
}

void GameController::levelUp() {
    int currentLevel = current->getLevel();
    if (currentLevel < 4) {  // Max level is 4
        current->setLevel(currentLevel + 1);
        display->message("Level increased to " + to_string(currentLevel + 1) + ".");
    } else {
        display->message("Level up not possible: already at maximum level 4.");
    }
}

void GameController::levelDown() {
    int currentLevel = current->getLevel();
    if (currentLevel > 0) {  // Min level is 0
        current->setLevel(currentLevel - 1);
        display->message("Level decreased to " + to_string(currentLevel - 1) + ".");
    } else {
        display->message("Level down not possible: already at minimum level 0.");
    }
}

void GameController::executeSequence(const string& filename) {
    ifstream file(filename);
    if (!file) {
        display->message("Error: could not open sequence file " + filename + ".");
        return;
    }

    display->message("Executing command sequence from file " + filename + ".");

    string cmd;
    while (file >> cmd) {
        if (!cmd.empty()) {
            processCommand(cmd);
            
            // Check if game is over
            if (gameOver) {
                break;
            }
        }
    }
    
    if (!gameOver) {
        display->message("Finished executing sequence from " + filename + ".");
    }
    
    file.close();
}

void GameController::replaceCurrentBlock(char blockType) {
    current->replaceCurrentBlock(blockType);
    display->message(string("Current block replaced with ") + blockType + "'.");
}

void GameController::setNoRandom(const string& filename) {
    int level = current->getLevel();
    if (level == 3 || level == 4) {
        current->setNoRandom(filename);
        display->message(std::string("Non-random mode enabled at level ") + to_string(level) + std::string(" using file ") + filename + ".");
    } else {
        display->message(std::string("NoRandom command only works for levels 3 and 4. Current level is") + to_string(level) + ".");
    }
}

void GameController::setRandom() {
    int level = current->getLevel();
    if (level == 3 || level == 4) {
        current->setRandom();
        display->message(std::string("Random mode restored at level ") + to_string(level) + ".");
    } else {
        display->message(std::string("Random command only works for levels 3 and 4. Current level is ") + to_string(level) + ".");
    }
}

void GameController::setSeed(int seed) {
    randomSeed = seed;
    srand(seed);
    display->message(std::string("Random seed set to ") + to_string(seed) + ".");
}
