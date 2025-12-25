export module Player;

import <memory>;
import <string>;
import <vector>;
import Board;
import Block;
import Level;

export class Player {
    int playerScore;
    int playerLevel;
    Board* theirBoard;
    std::unique_ptr<Block> currentBlock;
    std::unique_ptr<Block> nextBlock;
    std::unique_ptr<Level> levelObj;
    char lastLockedBlockType = ' ';
    
    // Track the level when current block was generated (for scoring)
    int currentBlockLevel;
    
    // Store positions of current block after locking (before clearing rows)
    std::vector<Position> lastLockedPositions;
    
    // Effect flags
    bool heavyEffect;
    bool blindEffect;
    
    // Store sequence file for Level 0
    std::string sequenceFile;

public:
    Player();
    // Add sequence file parameter
    Player(int startLevel, const std::string& seqFile = "");
    ~Player();
    
    // Getters
    int getLevel() const;
    int getScore() const;
    Block* getCurrentBlock();
    Block* getNextBlock();
    Board& getBoard();
    bool hasHeavyEffect() const;
    bool hasBlindEffect() const;
    
    // Setters
    void setLevel(int level);
    
    // Block generation
    std::unique_ptr<Block> generateBlock();
    void spawnInitialBlocks();
    void spawnNextBlock();
    
    // Block operations
    int lockCurrentBlock();
    void replaceCurrentBlock(char type);
    bool canPlaceCurrentBlock() const;
    
    // Scoring
    void updateScore(int rows);
    void checkAndScoreCompletedBlocks();
    
    // Reset
    void reset(int startLevel);
    
    // Effects
    void applyHeavyEffect();
    void applyBlindEffect();
    void applyForceEffect(char type);
    void clearEffects();
    
    // NoRandom support for levels 3 and 4
    void setNoRandom(const std::string& filename);
    void setRandom();

    // For level 4
    void dropStarBlock();
};
