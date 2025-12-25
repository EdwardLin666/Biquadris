module Player;

import <cstdlib>;
import <memory>;
import <string>;
import <vector>;
import Board;
import Block;
import Blocks;
import Level;
import LevelFactory;

Player::Player()
    : playerScore{0}
    , playerLevel{0}
    , theirBoard{nullptr}
    , currentBlock{nullptr}
    , nextBlock{nullptr}
    , levelObj{nullptr}
    , lastLockedBlockType{' '}
    , currentBlockLevel{0}
    , lastLockedPositions{}
    , heavyEffect{false}
    , blindEffect{false}
    , sequenceFile{""}
{
    theirBoard = new Board();
    levelObj = makeLevel(0, "");
}

// Constructor now accepts and stores sequence file
Player::Player(int startLevel, const std::string& seqFile) 
    : playerScore{0}
    , playerLevel{startLevel}
    , theirBoard{nullptr}
    , currentBlock{nullptr}
    , nextBlock{nullptr}
    , levelObj{nullptr}
    , lastLockedBlockType{' '}
    , currentBlockLevel{0}
    , lastLockedPositions{}
    , heavyEffect{false}
    , blindEffect{false}
    , sequenceFile{seqFile}
{
    theirBoard = new Board();
    // Pass sequence file to makeLevel
    levelObj = makeLevel(startLevel, sequenceFile);
}


Player::~Player() {
    delete theirBoard;
}

// Getters
int Player::getLevel() const {
    return playerLevel;
}

int Player::getScore() const {
    return playerScore;
}

Block* Player::getCurrentBlock() {
    return currentBlock.get();
}

Block* Player::getNextBlock() {
    return nextBlock.get();
}

Board& Player::getBoard() {
    return *theirBoard;
}

bool Player::hasHeavyEffect() const {
    return heavyEffect;
}

bool Player::hasBlindEffect() const {
    return blindEffect;
}

// Setters
// setLevel now preserves sequence file
void Player::setLevel(int level) {
    if (level >= 0 && level <= 4) {
        playerLevel = level;
        levelObj = makeLevel(level, sequenceFile);
    }
}

// Block generation using Level objects
std::unique_ptr<Block> Player::generateBlock() {
    if (levelObj) {
        return levelObj->nextBlock();
    }
    return std::make_unique<TBlock>();
}

void Player::spawnInitialBlocks() {
    currentBlock = generateBlock();
    currentBlockLevel = playerLevel;  // Remember level when block was generated
    nextBlock = generateBlock();
}

void Player::spawnNextBlock() {
    currentBlock = std::move(nextBlock);
    currentBlockLevel = playerLevel;  // The next block becomes current, track its level
    nextBlock = generateBlock();
    
    // Clear effects after dropping a block
    clearEffects();
}

/*
 * lockCurrentBlock() - Locks the current block onto the board
 * 
 * Steps:
 * 1. Get the absolute cell positions of the current block
 * 2. Save these positions (needed for checkAndScoreCompletedBlocks)
 * 3. Lock the block onto the board grid
 * 4. Clear any full rows
 * 5. Notify level object (for Level 4 star block logic)
 * 6. Return number of rows cleared
 */
int Player::lockCurrentBlock() {
    if (!currentBlock) return 0;
    
    // Save block type BEFORE any changes
    char lockedBlockType = currentBlock->type;
    lastLockedPositions = currentBlock->getAbsoluteCells();
    
    theirBoard->lockBlock(*currentBlock);
    int rowsCleared = theirBoard->clearFullRows();
    
    if (levelObj) {
        levelObj->onBlockLocked(rowsCleared);
        
        // Check if Level 4 needs to drop a star block
        if (levelObj->shouldDropStar()) {
            dropStarBlock();
            levelObj->clearStarPending();
        }
    }
    
    // Store for scoring check
    lastLockedBlockType = lockedBlockType;
    
    return rowsCleared;
}

// For Level 4
void Player::dropStarBlock() {
    StarBlock star;  // Starts at row 3, col 5 (center)
    
    // Drop to bottom
    while (theirBoard->canPlace(star)) {
        star.moveDown();
    }
    star.moveUp();
    
    // Lock it
    theirBoard->lockBlock(star);
    
    // Clear any full rows from star block
    int clearedRows = theirBoard->clearFullRows();
    if (clearedRows > 0) {
        updateScore(clearedRows);
    }
}

/*
 * checkAndScoreCompletedBlocks() - Check if the just-dropped block was completely cleared
 * 
 * Improved logic explanation
 * 
 * After lockCurrentBlock():
 * 1. lastLockedPositions contains the absolute positions where block cells were placed
 * 2. Block was locked into grid
 * 3. Full rows were cleared and cells shifted down
 * 
 * To check if block was completely cleared:
 * - We check if all positions where we placed the block now have the block's type char
 * - If they don't (either empty or different char from shifting), the cells were cleared
 * 
 * Note: This is a simplified check. A more accurate approach would track which rows
 * were cleared and verify all block cells were in those rows. The current approach
 * works because:
 * - If cells shifted into the position, they'll have different chars (wrong type)
 * - If position is now empty, the row was cleared
 * - We just need to verify none of the original block cells remain
 */
void Player::checkAndScoreCompletedBlocks() {
    if (lastLockedPositions.empty()) return;
    
    // Use saved block type instead of currentBlock->type
    char blockType = lastLockedBlockType;
    bool allCleared = true;
    
    for (const auto& pos : lastLockedPositions) {
        char cellContent = theirBoard->getCell(pos.row, pos.col);
        if (cellContent == blockType) {
            allCleared = false;
            break;
        }
    }
    
    if (allCleared) {
        // Score = (level when block was generated + 1)²
        int bonus = (currentBlockLevel + 1) * (currentBlockLevel + 1);
        playerScore += bonus;
    }
    
    lastLockedPositions.clear();
}


void Player::replaceCurrentBlock(char type) {
    switch (type) {
        case 'I': currentBlock = std::make_unique<IBlock>(); break;
        case 'J': currentBlock = std::make_unique<JBlock>(); break;
        case 'L': currentBlock = std::make_unique<LBlock>(); break;
        case 'O': currentBlock = std::make_unique<OBlock>(); break;
        case 'S': currentBlock = std::make_unique<SBlock>(); break;
        case 'Z': currentBlock = std::make_unique<ZBlock>(); break;
        case 'T': currentBlock = std::make_unique<TBlock>(); break;
        default: break;
    }
    // Note: Don't update currentBlockLevel here - testing commands don't change the level tracking
}

bool Player::canPlaceCurrentBlock() const {
    if (!currentBlock) return true;
    return theirBoard->canPlace(*currentBlock);
}

// Scoring for cleared rows: (level + rows)²
void Player::updateScore(int rows) {
    int score = (playerLevel + rows) * (playerLevel + rows);
    playerScore += score;
}

// Reset
// FIX: Preserve sequence file on reset
void Player::reset(int startLevel) {
    delete theirBoard;
    theirBoard = new Board();
    
    playerLevel = startLevel;
    playerScore = 0;
    currentBlockLevel = 0;
    
    currentBlock = nullptr;
    nextBlock = nullptr;
    
    // FIX: Use stored sequence file
    levelObj = makeLevel(startLevel, sequenceFile);
    
    lastLockedPositions.clear();
    
    heavyEffect = false;
    blindEffect = false;
}

// Effects
void Player::applyHeavyEffect() {
    heavyEffect = true;
}

void Player::applyBlindEffect() {
    blindEffect = true;
}

void Player::applyForceEffect(char type) {
    switch (type) {
        case 'I': currentBlock = std::make_unique<IBlock>(); break;
        case 'J': currentBlock = std::make_unique<JBlock>(); break;
        case 'L': currentBlock = std::make_unique<LBlock>(); break;
        case 'O': currentBlock = std::make_unique<OBlock>(); break;
        case 'S': currentBlock = std::make_unique<SBlock>(); break;
        case 'Z': currentBlock = std::make_unique<ZBlock>(); break;
        case 'T': currentBlock = std::make_unique<TBlock>(); break;
        default: break;
    }
}

void Player::clearEffects() {
    heavyEffect = false;
    blindEffect = false;
}

// NoRandom support
void Player::setNoRandom(const std::string& filename) {
    if (levelObj) {
        levelObj->setNoRandom(filename);
    }
}

void Player::setRandom() {
    if (levelObj) {
        levelObj->setRandom();
    }
}
