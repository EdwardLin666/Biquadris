export module CommandInterpreter;

import Command;
import <vector>;
import <sstream>;
import <string>;

export class CommandInterpreter {
    // Helper function to find command by prefix
    std::string findCommandByPrefix(const std::string& prefix);
    
    // Helper to parse multiplier (e.g., "3ri" -> multiplier=3, cmd="ri")
    void parseMultiplier(const std::string& input, int& multiplier, std::string& cmd);
    
    // Helper to split string by spaces
    std::vector<std::string> split(const std::string& str);

public:
    CommandInterpreter();
    ~CommandInterpreter() = default;
    
    // Read next command from input (stdin)
    std::string readNextCommand();
    
    // Parse command string into Command object
    // Returns nullptr if invalid command
    Command* parse(const std::string& cmd);
    
    // Parse command with arguments (for norandom, sequence)
    Command* parseWithArgs(const std::string& fullCmd);
    
    // Parse with multiplier support
    // Returns vector of commands (for multiplied commands like "3right")
    std::vector<Command*> parseWithMultiplier(const std::string& input);
};
