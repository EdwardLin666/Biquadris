export module Block;

import <vector>;
import <algorithm>;
import <utility>;


export class Position {
    public:
    int row, col;
};


export class Block {
public:
    std::vector<Position> cells;  // to store LLLLLLL
    int row, col; //top left position
    char type;
    int countCCW = 0;

    Block(const std::vector<Position> &shape, int startR = 6, int startC = 0, char t = ' '):
        cells{shape}, row{startR}, col{startC}, type{t} {}
    
    virtual ~Block(){}

    virtual char getSymbol() const = 0;

    void moveDown() {++row;}
    void moveUp() {--row;}
    void moveLeft() {--col;}
    void moveRight() {++col;}

    virtual void rotateCCW() {
        for (auto &p : cells) {
            int r = p.row, c = p.col;
            p.row = c;
            p.col = -r;
        }
        normalize();
    }

    virtual void rotateCW() {
        int oldMinR=9999, oldMaxR=-9999, oldMinC=9999, oldMaxC=-9999;
        for (auto &p : cells) {
            oldMinR = std::min(oldMinR, p.row);
            oldMaxR = std::max(oldMaxR, p.row);
            oldMinC = std::min(oldMinC, p.col);
            oldMaxC = std::max(oldMaxC, p.col);
        }

        for (auto &p : cells) {
            int r = p.row, c = p.col;
            p.row = c;
            p.col = -r;
        }

        int newMinR=9999, newMaxR=-9999, newMinC=9999, newMaxC=-9999;
        for (auto &p : cells) {
            newMinR = std::min(newMinR, p.row);
            newMaxR = std::max(newMaxR, p.row);
            newMinC = std::min(newMinC, p.col);
            newMaxC = std::max(newMaxC, p.col);
        }

        int oldLLrow = row + oldMaxR;
        int oldLLcol = col + oldMinC;

        int newLRrow = row + newMaxR;
        int newLRcol = col + newMaxC;

        row += (oldLLrow - newLRrow);
        col += (oldLLcol - newLRcol);

        normalize();
    }


    std::vector<Position> getAbsoluteCells() const{
        std::vector<Position> ans;

        for (auto &p : cells){
            ans.emplace_back(Position{row + p.row, col +p.col});
        }

        return ans;
    }


    private:

    void normalize(){
        int minR = 9999;
        int minC = 9999;
        
        for (auto &p : cells){
            if (p.row < minR) minR = p.row;
            if (p.col < minC) minC = p.col;
        }

        for (auto &p : cells){
            p.row -= minR;
            p.col -= minC;
        }
    }
};
