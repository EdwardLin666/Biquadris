export module LevelFactory;

import Level;
import Level0;
import Level1;
import Level2;
import Level3;
import Level4;

import <memory>;
import <string>;

// When seqFile is empty and level is 0, we need to use a default.
// But we can't know which player this is for here.
// The caller (Player) should pass the correct sequence file.
// If empty, Level0 will use its own default ("sequence.txt")

export std::unique_ptr<Level> makeLevel(int levelNumber, const std::string& seqFile = "") {
    switch (levelNumber) {
        case 0: 
            // If sequence file provided, use it; otherwise Level0 uses its default
            if (!seqFile.empty()) {
                return std::make_unique<Level0>(seqFile);
            }
            return std::make_unique<Level0>();
        case 1: return std::make_unique<Level1>();
        case 2: return std::make_unique<Level2>();
        case 3: return std::make_unique<Level3>();
        case 4: return std::make_unique<Level4>();
        default: 
            if (!seqFile.empty()) {
                return std::make_unique<Level0>(seqFile);
            }
            return std::make_unique<Level0>();
    }
}
