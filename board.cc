export module Board;

import Block;
import <vector>;
import <algorithm>;

export class Board {
    int rows, cols;
    std::vector<std::vector<char>> grid;

public:
    Board(int r = 21, int c = 11) : rows(r), cols(c), grid(r, std::vector<char>(c, ' ')) {}

    // Check if a block can be placed at its current position
    bool canPlace(const Block& b) const {
        auto absCells = b.getAbsoluteCells();
        for (const auto& p : absCells) {
            int r = p.row;
            int c = p.col;
            
            // Check bounds
            if (r < 0 || r >= rows || c < 0 || c >= cols) {
                return false;
            }
            
            // Check if cell is occupied
            if (grid[r][c] != ' ') {
                return false;
            }
        }
        return true;
    }

    // Get the grid for display purposes
    std::vector<std::vector<char>>& getGrid() {
        return grid;
    }

    const std::vector<std::vector<char>>& getGrid() const {
        return grid;
    }

    // Drop block to the bottom (for hard drop)
    void dropToBottom(Block& b) {
        while (true) {
            b.moveDown();
            if (!canPlace(b)) {
                b.moveUp();
                break;
            }
        }
    }

    // Lock a block into the grid
    void lockBlock(const Block& b) {
        auto absCells = b.getAbsoluteCells();
        for (const auto& p : absCells) {
            if (p.row >= 0 && p.row < rows && p.col >= 0 && p.col < cols) {
                grid[p.row][p.col] = b.type;
            }
        }
    }

    // Clear full rows and return the number of rows cleared
    int clearFullRows() {
        int cleared = 0;

        for (int r = rows - 1; r >= 0; --r) {
            bool full = true;
            for (int c = 0; c < cols; ++c) {
                if (grid[r][c] == ' ') {
                    full = false;
                    break;
                }
            }

            if (full) {
                cleared++;
                
                // Move all rows above down by one
                for (int rr = r; rr > 0; --rr) {
                    grid[rr] = grid[rr - 1];
                }
                
                // Clear the top row
                grid[0] = std::vector<char>(cols, ' ');
                
                // Check the same row again since rows shifted down
                r++;
            }
        }
        return cleared;
    }

    // Check if a specific cell is empty
    bool isCellEmpty(int r, int c) const {
        if (r >= 0 && r < rows && c >= 0 && c < cols) {
            return grid[r][c] == ' ';
        }
        return true;  // Out of bounds treated as empty
    }

    // Check if game is over (new block can't be placed)
    bool isGameOver(const Block& newBlock) const {
        return !canPlace(newBlock);
    }

    // Reset the board
    void reset() {
        for (auto& row : grid) {
            std::fill(row.begin(), row.end(), ' ');
        }
    }

    int numRows() const { return rows; }
    int numCols() const { return cols; }

    // Get a specific cell
    char getCell(int r, int c) const {
        if (r >= 0 && r < rows && c >= 0 && c < cols) {
            return grid[r][c];
        }
        return ' ';
    }

    // Set a specific cell (for testing or special blocks)
    void setCell(int r, int c, char ch) {
        if (r >= 0 && r < rows && c >= 0 && c < cols) {
            grid[r][c] = ch;
        }
    }
};
