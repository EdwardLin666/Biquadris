export module Level;

import Block;
import <memory>;
import <string>;

export class Level {
protected:
    int levelNum;

public:
    Level(int n = 0) : levelNum{n} {}

    virtual ~Level() = default;

    virtual std::unique_ptr<Block> nextBlock() = 0;

    virtual void onBlockLocked(int rowsCleared) {
        (void)rowsCleared;  // Default: do nothing
    }

    virtual bool isHeavy() const { return false; }

    int getLevelNum() const { return levelNum; }
    
    // For norandom/random commands (only relevant for levels 3 and 4)
    virtual void setNoRandom(const std::string& filename) {
        (void)filename;  // Default: do nothing
    }
    
    virtual void setRandom() {
        // Default: do nothing
    }

    virtual bool shouldDropStar() { return false; }
    virtual void clearStarPending() {}
};
