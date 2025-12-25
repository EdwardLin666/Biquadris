import GameController;
import CommandInterpreter;
import Player;
import IDisplay;
import TextDisplay;
import GraphicDisplay;
import <iostream>;
import <string>;
import <cstdlib>;

using namespace std;

int main(int argc, char* argv[]) {
    bool textOnly = false;
    int seed = 0;
    string scriptFile1 = "biquadris_sequence1.txt";
    
    string scriptFile2 = "biquadris_sequence2.txt";
    int startLevel = 0;

    for (int i = 1; i < argc; ++i) {
        string args = argv[i];

        if (args == "-text") {
            textOnly = true;
        } else if (args == "-seed") {
            if (i + 1 < argc) {
                seed = stoi(argv[++i]);
            }
        } else if (args == "-scriptfile1") {
            if (i + 1 < argc) {
                scriptFile1 = argv[++i];
            }
        } else if (args == "-scriptfile2") {
            if (i + 1 < argc) {
                scriptFile2 = argv[++i];
            }
        } else if (args == "-startlevel") {
            if (i + 1 < argc) {
                startLevel = stoi(argv[++i]);
                if (startLevel < 0) startLevel = 0;
                if (startLevel > 4) startLevel = 4;
            }
        }
    }

    // Set random seed if provided
    if (seed != 0) {
        srand(seed);
    }

    // Pass script files to players
    Player* p1 = new Player(startLevel, scriptFile1);
    Player* p2 = new Player(startLevel, scriptFile2);

    CommandInterpreter* ci = new CommandInterpreter();

    IDisplay *display;

    if (textOnly) {
        display = new TextDisplay();
    } else {
        display = new GraphicDisplay();
    }


    GameController* gc = new GameController(p1, p2, ci, seed, display);

    gc->startNewGame(startLevel);

    gc->run();

    delete gc;
    delete ci;
    delete p1;
    delete p2;
    delete display;

    return 0;
}
