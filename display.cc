export module IDisplay;

import Board;
import Player;
import Block;
import <iostream>;
import <string>;
import <vector>;

using namespace std;

export class IDisplay{
    public:
    virtual void message(const string &s) = 0;
    virtual void render(Player& p1, Player& p2) = 0;
    virtual ~IDisplay() = default;
};
