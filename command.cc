export module Command;
import <string>;

// Use IGameController abstract class instead of GameController
export class IGameController {
public:
    // Movement
    virtual void moveLeft() = 0;
    virtual void moveRight() = 0;
    virtual void moveDown() = 0;
    virtual void drop() = 0;

    // Rotation
    virtual void rotateCW() = 0;
    virtual void rotateCCW() = 0;

    // Level
    virtual void levelUp() = 0;
    virtual void levelDown() = 0;

    // Others
    virtual void restart() = 0;
    virtual void executeSequence(const std::string& filename) = 0;
    virtual void replaceCurrentBlock(char blockType) = 0;
    virtual void setNoRandom(const std::string& filename) = 0;
    virtual void setRandom() = 0;

    virtual ~IGameController() = default;
};

// Abstract class
export class Command {
public:
    virtual void execute(IGameController &gc) = 0;
    virtual ~Command() = default;
};

// Change to IGameController
export class MoveLeftCmd : public Command {
public:
    void execute(IGameController &gc) override;
};

export class MoveRightCmd : public Command {
public:
    void execute(IGameController &gc) override;
};

export class MoveDownCmd : public Command {
public:
    void execute(IGameController &gc) override;
};

export class DropCmd : public Command {
public:
    void execute(IGameController &gc) override;
};

export class RotateCwCmd : public Command {
public:
    void execute(IGameController &gc) override;
};

export class RotateCcwCmd : public Command {
public:
    void execute(IGameController &gc) override;
};

export class LevelUpCmd : public Command {
public:
    void execute(IGameController &gc) override;
};

export class LevelDownCmd : public Command {
public:
    void execute(IGameController &gc) override;
};

export class RestartCmd : public Command {
public:
    void execute(IGameController &gc) override;
};

export class SequenceCmd : public Command {
    std::string filename;
public:
    explicit SequenceCmd(std::string file);
    void execute(IGameController &gc) override;
};

export class ReplaceBlockCmd : public Command {
    char blockType;
public:
    explicit ReplaceBlockCmd(char type);
    void execute(IGameController &gc) override;
};

// For norandom command
export class NoRandomCmd : public Command {
    std::string filename;
public:
    explicit NoRandomCmd(std::string file);
    void execute(IGameController &gc) override;
};

// For random command
export class RandomCmd : public Command {
public:
    void execute(IGameController &gc) override;
};
