export module Blocks;

import Block;
import <vector>;

// I-Block:  IIII (horizontal line)
export class IBlock : public Block {
public:
    IBlock(int startR = 6, int startC = 0)
        : Block({Position{0, 0}, Position{0, 1}, Position{0, 2}, Position{0, 3}}, 
                startR, startC, 'I') {}
    
    char getSymbol() const override { return 'I'; }
};

// J-Block:  J
//           JJJ
export class JBlock : public Block {
public:
    JBlock(int startR = 6, int startC = 0)
        : Block({Position{0, 0}, Position{1, 0}, Position{1, 1}, Position{1, 2}}, 
                startR, startC, 'J') {}
    
    char getSymbol() const override { return 'J'; }
};

// L-Block:    L
//           LLL
export class LBlock : public Block {
public:
    LBlock(int startR = 6, int startC = 0)
        : Block({Position{0, 2}, Position{1, 0}, Position{1, 1}, Position{1, 2}}, 
                startR, startC, 'L') {}
    
    char getSymbol() const override { return 'L'; }
};

// O-Block:  OO
//           OO
export class OBlock : public Block {
public:
    OBlock(int startR = 6, int startC = 0)
        : Block({Position{0, 0}, Position{0, 1}, Position{1, 0}, Position{1, 1}}, 
                startR, startC, 'O') {}
    
    char getSymbol() const override { return 'O'; }
};

// S-Block:   SS
//           SS
export class SBlock : public Block {
public:
    SBlock(int startR = 6, int startC = 0)
        : Block({Position{0, 1}, Position{0, 2}, Position{1, 0}, Position{1, 1}}, 
                startR, startC, 'S') {}
    
    char getSymbol() const override { return 'S'; }
};

// Z-Block:  ZZ
//            ZZ
export class ZBlock : public Block {
public:
    ZBlock(int startR = 6, int startC = 0)
        : Block({Position{0, 0}, Position{0, 1}, Position{1, 1}, Position{1, 2}}, 
                startR, startC, 'Z') {}
    
    char getSymbol() const override { return 'Z'; }
};

// T-Block:  TTT
//            T
export class TBlock : public Block {
public:
    TBlock(int startR = 6, int startC = 0)
        : Block({Position{0, 0}, Position{0, 1}, Position{0, 2}, Position{1, 1}}, 
                startR, startC, 'T') {}
    
    char getSymbol() const override { return 'T'; }
};

// Star Block (1x1) for Level 4
export class StarBlock : public Block {
public:
    StarBlock(int startR = 6, int startC = 5)  // Center column (column 5 of 11)
        : Block({Position{0, 0}}, startR, startC, '*') {}
    
    char getSymbol() const override { return '*'; }
};
