export module Level2;

import Level;
import Block;
import Blocks;
import <memory>;
import <cstdlib>;

export class Level2 : public Level {
public:
    Level2() : Level(2) {}

    std::unique_ptr<Block> nextBlock() override {
        int r = rand() % 7;
        switch (r) {
            case 0: return std::make_unique<IBlock>();
            case 1: return std::make_unique<JBlock>();
            case 2: return std::make_unique<LBlock>();
            case 3: return std::make_unique<OBlock>();
            case 4: return std::make_unique<SBlock>();
            case 5: return std::make_unique<ZBlock>();
            default:return std::make_unique<TBlock>();
        }
    }
};
