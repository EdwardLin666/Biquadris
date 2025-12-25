export module Level4;

import Level;
import Block;
import Blocks;
import <memory>;
import <vector>;
import <cstdlib>;
import <string>;
import <fstream>;

export class Level4 : public Level {
    std::vector<char> pool;
    int blocksSinceClear;
    bool starPending;
    
    // For norandom mode
    bool useNoRandom;
    std::string noRandomFile;
    std::vector<char> fileSequence;
    size_t fileIndex;
    
    void loadSequenceFile() {
        fileSequence.clear();
        fileIndex = 0;
        
        std::ifstream ifs(noRandomFile);
        if (!ifs) return;
        
        std::string token;
        while (ifs >> token) {
            if (!token.empty()) {
                fileSequence.push_back(token[0]);
            }
        }
    }

public:
    Level4() : Level(4), blocksSinceClear{0}, starPending{false}, 
               useNoRandom{false}, fileIndex{0} {
        // S and Z have probability 2/9 each, others have 1/9 each
        pool = {'S', 'S', 'Z', 'Z', 'I', 'J', 'L', 'O', 'T'};
    }

    std::unique_ptr<Block> nextBlock() override {
        
        char ch;
        
        if (useNoRandom && !fileSequence.empty()) {
            ch = fileSequence[fileIndex++];
            if (fileIndex >= fileSequence.size()) {
                fileIndex = 0;  // Wrap around
            }
        } else {
            int idx = rand() % static_cast<int>(pool.size());
            ch = pool[idx];
        }
        
        switch (ch) {
            case 'I': return std::make_unique<IBlock>();
            case 'J': return std::make_unique<JBlock>();
            case 'L': return std::make_unique<LBlock>();
            case 'O': return std::make_unique<OBlock>();
            case 'S': return std::make_unique<SBlock>();
            case 'Z': return std::make_unique<ZBlock>();
            case 'T': return std::make_unique<TBlock>();
            default:  return std::make_unique<TBlock>();
        }
    }

    bool isHeavy() const override { return true; }

    void onBlockLocked(int rowsCleared) override {
        if (rowsCleared > 0) {
            blocksSinceClear = 0;
        } else {
            ++blocksSinceClear;
            // Every 5 blocks without clearing, drop a star block
            if (blocksSinceClear % 5 == 0) {
                starPending = true;
            }
        }
    }
    
    void setNoRandom(const std::string& filename) override {
        useNoRandom = true;
        noRandomFile = filename;
        loadSequenceFile();
    }
    
    void setRandom() override {
        useNoRandom = false;
        fileSequence.clear();
        fileIndex = 0;
    }

    bool shouldDropStar() override { return starPending; }
    void clearStarPending() override { starPending = false; }
};
