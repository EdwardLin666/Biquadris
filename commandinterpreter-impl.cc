module CommandInterpreter;

import Command;
import <iostream>;
import <vector>;
import <string>;
import <sstream>;

using namespace std;

CommandInterpreter::CommandInterpreter() {}

string CommandInterpreter::readNextCommand() {
    string cmd;
    if (getline(cin, cmd)) {
        return cmd;
    }
    return "";  // EOF
}

void CommandInterpreter::parseMultiplier(const string& input, int& multiplier, string& cmd) {
    multiplier = 1;
    cmd = input;
    
    // Check if starts with digit
    size_t i = 0;
    while (i < input.length() && input[i] >= '0' && input[i] <= '9') {
        ++i;
    }
    
    if (i > 0) {
        multiplier = 0;
        for (size_t j = 0; j < i; ++j) {
            multiplier = multiplier * 10 + (input[j] - '0');
        }
        cmd = input.substr(i);
    }
}

string CommandInterpreter::findCommandByPrefix(const string& prefix) {
    if (prefix.empty()) return "";
    
    // Try exact matches first for single-letter block commands
    if (prefix == "I" || prefix == "J" || prefix == "L" || prefix == "O" || 
        prefix == "S" || prefix == "Z" || prefix == "T") {
        return prefix;
    }
    
    // Check each command to see if prefix matches
    string match = "";
    int matchCount = 0;
    
    // Movement commands
    if (string("left").find(prefix) == 0) { match = "left"; ++matchCount; }
    if (string("right").find(prefix) == 0) { match = "right"; ++matchCount; }
    if (string("down").find(prefix) == 0) { match = "down"; ++matchCount; }
    if (string("drop").find(prefix) == 0) { match = "drop"; ++matchCount; }
    
    // Rotation commands
    if (string("clockwise").find(prefix) == 0) { match = "clockwise"; ++matchCount; }
    if (string("counterclockwise").find(prefix) == 0) { match = "counterclockwise"; ++matchCount; }
    
    // Level commands
    if (string("levelup").find(prefix) == 0) { match = "levelup"; ++matchCount; }
    if (string("leveldown").find(prefix) == 0) { match = "leveldown"; ++matchCount; }
    
    // Other commands
    if (string("restart").find(prefix) == 0) { match = "restart"; ++matchCount; }
    if (string("norandom").find(prefix) == 0) { match = "norandom"; ++matchCount; }
    if (string("random").find(prefix) == 0) { match = "random"; ++matchCount; }
    if (string("sequence").find(prefix) == 0) { match = "sequence"; ++matchCount; }
    
    // Only return if unambiguous (exactly one match)
    if (matchCount == 1) {
        return match;
    }
    
    return "";  // Not found
}

Command* CommandInterpreter::parse(const string& cmdStr) {
    if (cmdStr.empty()) {
        return nullptr;
    }
    
    string cmd = findCommandByPrefix(cmdStr);
    
    if (cmd.empty()) {
        return nullptr;
    }
    
    // Basic movement
    if (cmd == "left") return new MoveLeftCmd();
    if (cmd == "right") return new MoveRightCmd();
    if (cmd == "down") return new MoveDownCmd();
    if (cmd == "drop") return new DropCmd();
    
    // Rotation
    if (cmd == "clockwise") return new RotateCwCmd();
    if (cmd == "counterclockwise") return new RotateCcwCmd();
    
    // Level
    if (cmd == "levelup") return new LevelUpCmd();
    if (cmd == "leveldown") return new LevelDownCmd();
    
    // Restart
    if (cmd == "restart") return new RestartCmd();
    
    // Random (no argument)
    if (cmd == "random") return new RandomCmd();
    
    // Block replacement
    if (cmd == "I" || cmd == "J" || cmd == "L" || cmd == "O" || 
        cmd == "S" || cmd == "Z" || cmd == "T") {
        return new ReplaceBlockCmd(cmd[0]);
    }
    
    return nullptr;
}

vector<Command*> CommandInterpreter::parseWithMultiplier(const string& input) {
    vector<Command*> commands;
    
    if (input.empty()) {
        return commands;
    }
    
    // Split input into words to handle commands with arguments
    istringstream iss(input);
    string firstWord;
    iss >> firstWord;
    
    if (firstWord.empty()) {
        return commands;
    }
    
    // Parse multiplier from first word
    int multiplier;
    string cmdStr;
    parseMultiplier(firstWord, multiplier, cmdStr);
    
    // Find which command this is
    string baseCmd = findCommandByPrefix(cmdStr);
    
    // Commands that don't support multipliers
    if (baseCmd == "restart" || baseCmd == "norandom" || 
        baseCmd == "random" || baseCmd == "sequence") {
        multiplier = 1;
    }
    
    // Handle norandom command (needs filename argument)
    if (baseCmd == "norandom") {
        string filename;
        iss >> filename;  // Read the filename argument
        if (!filename.empty()) {
            commands.push_back(new NoRandomCmd(filename));
        }
        return commands;
    }
    
    // Handle sequence command (needs filename argument)
    if (baseCmd == "sequence") {
        string filename;
        iss >> filename;  // Read the filename argument
        if (!filename.empty()) {
            commands.push_back(new SequenceCmd(filename));
        }
        return commands;
    }
    
    // Handle random command (no argument)
    if (baseCmd == "random") {
        commands.push_back(new RandomCmd());
        return commands;
    }
    
    // Generate multiplied commands for all other commands
    for (int i = 0; i < multiplier; ++i) {
        Command* cmd = parse(cmdStr);
        if (cmd) {
            commands.push_back(cmd);
        }
    }
    
    return commands;
}
