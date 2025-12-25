export module TextDisplay;

import Board;
import Player;
import Block;
import IDisplay;
import <iostream>;
import <string>;
import <vector>;

using namespace std;

export class TextDisplay : public IDisplay{
public:
    void message(const string& s) override {
        cout << s << endl;
    }
    void render(Player& p1, Player& p2) override {
        Board& b1 = p1.getBoard();
        Board& b2 = p2.getBoard();

        // Copy grids to overlay current blocks
        vector<vector<char>> grid1 = b1.getGrid();
        vector<vector<char>> grid2 = b2.getGrid();

        // Overlay current blocks
        Block* curr1 = p1.getCurrentBlock();
        if (curr1) {
            for (const auto& pos : curr1->getAbsoluteCells()) {
                if (pos.row >= 0 && pos.row < b1.numRows() &&
                    pos.col >= 0 && pos.col < b1.numCols()) {
                    grid1[pos.row][pos.col] = curr1->type;
                }
            }
        }

        Block* curr2 = p2.getCurrentBlock();
        if (curr2) {
            for (const auto& pos : curr2->getAbsoluteCells()) {
                if (pos.row >= 0 && pos.row < b2.numRows() &&
                    pos.col >= 0 && pos.col < b2.numCols()) {
                    grid2[pos.row][pos.col] = curr2->type;
                }
            }
        }

        // Print header
        cout << "Level:    " << p1.getLevel() << "    Level:    " << p2.getLevel() << endl;
        cout << "Score:    " << p1.getScore() << "    Score:    " << p2.getScore() << endl;
        cout << "-----------    -----------" << endl;

        // Print boards (skip 3 reserve rows)
        for (int r = 3; r < b1.numRows(); ++r) {
            for (int c = 0; c < b1.numCols(); ++c) {
		    if (p1.hasBlindEffect() && c>=2 && c<=8 && r>=6 && r <= 18){
			    cout << "?";
		    }
		    else {
		    	cout << grid1[r][c];
		    }
	    }
            cout << "    ";
            for (int c = 0; c < b2.numCols(); ++c){
                    if (p2.hasBlindEffect() &&  c>=2 && c<=8 && r>=6 && r <= 18){
                            cout << "?";
                    }
                    else {
                        cout << grid2[r][c];
                    }
            }	   
            
	    cout << endl;
        }

        cout << "-----------    -----------" << endl;
        cout << "Next:          Next:" << endl;

        // Print next blocks (simplified)
        Block* n1 = p1.getNextBlock();
        Block* n2 = p2.getNextBlock();

        string n1r1, n1r2, n2r1, n2r2;
        getBlockPattern(n1, n1r1, n1r2);
        getBlockPattern(n2, n2r1, n2r2);

        while (n1r1.length() < 11) n1r1 += ' ';
        while (n1r2.length() < 11) n1r2 += ' ';

        cout << n1r1 << "    " << n2r1 << endl;
        if (!n1r2.empty() || !n2r2.empty()) {
            cout << n1r2 << "    " << n2r2 << endl;
        }

        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    }

private:
    void getBlockPattern(Block* b, string& r1, string& r2) {
        r1 = ""; r2 = "";
        if (!b) return;
        switch (b->getSymbol()) {
            case 'I': r1 = "IIII"; break;
            case 'J': r1 = "J"; r2 = "JJJ"; break;
            case 'L': r1 = "  L"; r2 = "LLL"; break;
            case 'O': r1 = "OO"; r2 = "OO"; break;
            case 'S': r1 = " SS"; r2 = "SS"; break;
            case 'Z': r1 = "ZZ"; r2 = " ZZ"; break;
            case 'T': r1 = "TTT"; r2 = " T"; break;
            case '*': r1 = "*"; break;
            default: r1 = "?"; break;
        }
    }
};
