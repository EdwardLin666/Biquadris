export module Level0;

import Level;
import Block;
import Blocks;
import <memory>;
import <string>;
import <vector>;
import <fstream>;
import <sstream>;

export class Level0: public Level{
    std::string filename;
    std::vector<char> seq;
    size_t idx;

    void loadFile(){
        seq.clear();

        std::ifstream ifs(filename);

        if (!ifs) return;

        std::string token;

        while (ifs >> token){
            if(!token.empty()) seq.push_back(token[0]);
        }

        if (seq.empty()){
            // default sequence
            seq ={'I','J','L','O','S','Z','T'};
        }
        idx = 0;
    }

    public:
    Level0(const std::string &file = "sequence.txt") :Level{0}, filename{file}, idx(0) {loadFile();}

    std::unique_ptr<Block> nextBlock() override {
        if (seq.empty()) loadFile();
        if(seq.empty()) return std::make_unique<TBlock>(); // just for safety

        char ch = seq[idx++];

        if (idx >= seq.size()){
            idx = 0;    // rewind
        }

        switch (ch){
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
