export module GraphicDisplay;

import IDisplay;
import Player;
import Board;
import Block;

import <X11/Xlib.h>;
import <X11/Xutil.h>;
import <iostream>;
import <vector>;
import <string>;
import <memory>;
import <cstring>;

using namespace std;
using XDisplay = ::Display;

export class GraphicDisplay : public IDisplay {
    XDisplay* dpy;
    Window win;
    GC gc;

    const int cellSize = 20;

    // colors
    unsigned long black, white;
    unsigned long colors[256];

    // Store current message to display
    string currentMessage;

public:
    GraphicDisplay() {
        dpy = XOpenDisplay(nullptr);
        if (!dpy) {
            cerr << "Could not open X11 display.\n";
            exit(1);
        }

        int screen = DefaultScreen(dpy);
        black = BlackPixel(dpy, screen);
        white = WhitePixel(dpy, screen);

        win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
                                  10, 10,
                                  600, 500,
                                  1, black, white);

        XSelectInput(dpy, win, ExposureMask | KeyPressMask);
        XMapWindow(dpy, win);

        gc = XCreateGC(dpy, win, 0, nullptr);

        initColors();
    }

    ~GraphicDisplay() {
        XFreeGC(dpy, gc);
        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
    }

    void message(const string& s) override {
        currentMessage = s;
        // Also print to console for debugging
        cout << "[Graphic] " << s << endl;
        // Redraw message area
        drawMessage();
        XFlush(dpy);
    }

    void render(Player& p1, Player& p2) override {
        XClearWindow(dpy, win);

        drawBoard(50, p1);      // left board at x=50
        drawBoard(350, p2);     // right board at x=350

        // Draw next blocks
        drawNextBlock(50, 400, p1.getNextBlock());
        drawNextBlock(350, 400, p2.getNextBlock());

        // Draw message at the bottom
        drawMessage();

        XFlush(dpy);
    }

private:
    unsigned long colorFor(char ch) {
        switch (ch) {
            case 'I': return colors[1];
            case 'J': return colors[2];
            case 'L': return colors[3];
            case 'O': return colors[4];
            case 'S': return colors[5];
            case 'Z': return colors[6];
            case 'T': return colors[7];
            case '*': return colors[8];
            default: return white;
        }
    }

    void initColors() {
        Colormap cmap = DefaultColormap(dpy, 0);

        XColor xc;
        auto set = [&](int idx, const char* name){
            if (!XParseColor(dpy, cmap, name, &xc)) return;
            if (!XAllocColor(dpy, cmap, &xc)) return;
            colors[idx] = xc.pixel;
        };

        set(1, "cyan");
        set(2, "blue");
        set(3, "orange");
        set(4, "yellow");
        set(5, "green");
        set(6, "red");
        set(7, "purple");
        set(8, "gray");
    }

    void drawBoard(int xOffset, Player& p) {
        Board& b = p.getBoard();

        // Copy grid and overlay current block
        vector<vector<char>> grid = b.getGrid();
        Block* curr = p.getCurrentBlock();
        if (curr) {
            for (auto& pos : curr->getAbsoluteCells()) {
                if (pos.row >= 0 && pos.row < b.numRows() &&
                    pos.col >= 0 && pos.col < b.numCols()) {
                    grid[pos.row][pos.col] = curr->type;
                }
            }
        }

        for (int r = 3; r < b.numRows(); ++r) {  // skip top 3 rows
            for (int c = 0; c < b.numCols(); ++c) {
                
		char ch = grid[r][c];
		if (!(p.hasBlindEffect())){
                drawCell(xOffset + c * cellSize,
                         (r - 3) * cellSize,
                         colorFor(ch));
		} else {
		       drawCell(xOffset + c * cellSize,
                         (r - 3) * cellSize,white);
		     XSetForeground(dpy, gc, black);
		    XDrawString(dpy,win,gc,xOffset + c *cellSize,(r - 3) * cellSize, "?", 1);
                }
            }
        }

        drawText(xOffset, 360, ("Level: " + to_string(p.getLevel())).c_str());
        drawText(xOffset, 380, ("Score: " + to_string(p.getScore())).c_str());
    }

    void drawNextBlock(int xOffset, int yOffset, Block* nextBlock) {
        drawText(xOffset, yOffset, "Next:");

        if (!nextBlock) return;

        char type = nextBlock->getSymbol();
        int startY = yOffset + 5;  // Start drawing cells below "Next:" label

        // Draw the next block pattern based on type (similar to textdisplay.cc)
        // Each block is drawn in its canonical orientation
        switch (type) {
            case 'I':
                // IIII (1 row, 4 cols)
                for (int c = 0; c < 4; ++c) {
                    drawCell(xOffset + c * cellSize, startY, colorFor('I'));
                }
                break;
            case 'J':
                // J
                // JJJ
                drawCell(xOffset, startY, colorFor('J'));
                for (int c = 0; c < 3; ++c) {
                    drawCell(xOffset + c * cellSize, startY + cellSize, colorFor('J'));
                }
                break;
            case 'L':
                //   L
                // LLL
                drawCell(xOffset + 2 * cellSize, startY, colorFor('L'));
                for (int c = 0; c < 3; ++c) {
                    drawCell(xOffset + c * cellSize, startY + cellSize, colorFor('L'));
                }
                break;
            case 'O':
                // OO
                // OO
                for (int r = 0; r < 2; ++r) {
                    for (int c = 0; c < 2; ++c) {
                        drawCell(xOffset + c * cellSize, startY + r * cellSize, colorFor('O'));
                    }
                }
                break;
            case 'S':
                //  SS
                // SS
                drawCell(xOffset + cellSize, startY, colorFor('S'));
                drawCell(xOffset + 2 * cellSize, startY, colorFor('S'));
                drawCell(xOffset, startY + cellSize, colorFor('S'));
                drawCell(xOffset + cellSize, startY + cellSize, colorFor('S'));
                break;
            case 'Z':
                // ZZ
                //  ZZ
                drawCell(xOffset, startY, colorFor('Z'));
                drawCell(xOffset + cellSize, startY, colorFor('Z'));
                drawCell(xOffset + cellSize, startY + cellSize, colorFor('Z'));
                drawCell(xOffset + 2 * cellSize, startY + cellSize, colorFor('Z'));
                break;
            case 'T':
                // TTT
                //  T
                for (int c = 0; c < 3; ++c) {
                    drawCell(xOffset + c * cellSize, startY, colorFor('T'));
                }
                drawCell(xOffset + cellSize, startY + cellSize, colorFor('T'));
                break;
            case '*':
                // * (single cell)
                drawCell(xOffset, startY, colorFor('*'));
                break;
            default:
                break;
        }
    }

    void drawMessage() {
        if (!currentMessage.empty()) {
            // Draw message at the bottom of the window
            drawText(50, 480, currentMessage.c_str());
        }
    }

    void drawCell(int x, int y, unsigned long color) {
        XSetForeground(dpy, gc, black);
        XDrawRectangle(dpy, win, gc, x, y, cellSize, cellSize);
        XSetForeground(dpy, gc, color);
        XFillRectangle(dpy, win, gc, x + 1, y + 1, cellSize - 1, cellSize - 1);
    }

    void drawText(int x, int y, const char* text) {
        XSetForeground(dpy, gc, black);
        XDrawString(dpy, win, gc, x, y, text, strlen(text));
    }
};
