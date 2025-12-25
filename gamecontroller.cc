export module GameController;

import CommandInterpreter;
import Command;
import Player;
import Block;
import Board;
import IDisplay;
import Level;
import <iostream>;
import <string>;

using namespace std;


export class GameController : public IGameController{
    Player* p1;
    Player* p2;
    Player* current;
    CommandInterpreter* ci;
    IDisplay* display;
    
    int hiScore;  // Persists across restarts
    bool gameOver;
    int randomSeed;  // For -seed command line option
    
    // Helper to get opponent of current player
    Player* getOpponent();

public:
    GameController(Player* p1, Player* p2, CommandInterpreter* ci, int seed, IDisplay* display);
    ~GameController() = default;
    
    // Main game loop
    void run();
    
    // Process a single command string
    void processCommand(const string& cmd);
    
    // Initialize/restart game
    void startNewGame(int startLevel = 0);
    void restart() override;
    
    // Turn management
    void switchTurn();
    
    // Called after a block is locked
    void onBlockLocked(int rowsCleared);
    
    // Trigger special action
    void triggerSpecialAction(Player* attacker, Player* defender, int rows);
    
    // Movement commands (called by Command objects)
    void moveLeft() override;
    void moveRight() override;
    void rotateCW() override;
    void rotateCCW() override;
    void moveDown() override;
    void drop() override;
    
    // Level commands
    void levelUp() override;
    void levelDown() override;
    
    // Special commands
    void executeSequence(const string& filename) override;
    void replaceCurrentBlock(char blockType) override;
    void setNoRandom(const string& filename) override;
    void setRandom() override;
    
    // Set random seed
    void setSeed(int seed);
    
    // Getters
    Player* getCurrentPlayer() { return current; }
    int getHiScore() const { return hiScore; }
    bool isGameOver() const { return gameOver; }
};
