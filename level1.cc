export module Level1;

import Level;
import Block;
import Blocks;
import <memory>;
import <vector>;
import <cstdlib>;

export class Level1 : public Level {
    std::vector<char> pool;

    public:
    Level1() : Level(1) {
        pool = {
            'S','Z',
            'I','I','J','J','L','L','O','O','T','T'
        };
    }

    std::unique_ptr<Block> nextBlock() override {
        int idx = rand() % static_cast<int>(pool.size());
        char ch = pool[idx];
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
};
