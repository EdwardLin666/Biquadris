module Command;

import <string>;

// Basic movement commands
void MoveLeftCmd::execute(IGameController &gc) {
    gc.moveLeft();
}

void MoveRightCmd::execute(IGameController &gc) {
    gc.moveRight();
}

void MoveDownCmd::execute(IGameController &gc) {
    gc.moveDown();
}

void DropCmd::execute(IGameController &gc) {
    gc.drop();
}

void RotateCwCmd::execute(IGameController &gc) {
    gc.rotateCW();
}

void RotateCcwCmd::execute(IGameController &gc) {
    gc.rotateCCW();
}

void LevelUpCmd::execute(IGameController &gc) {
    gc.levelUp();
}

void LevelDownCmd::execute(IGameController &gc) {
    gc.levelDown();
}

void RestartCmd::execute(IGameController &gc) {
    gc.restart();
}

SequenceCmd::SequenceCmd(std::string file)
    : filename{file} {}

void SequenceCmd::execute(IGameController &gc) {
    gc.executeSequence(filename);
}

ReplaceBlockCmd::ReplaceBlockCmd(char type)
    : blockType{type} {}

void ReplaceBlockCmd::execute(IGameController &gc) {
    gc.replaceCurrentBlock(blockType);
}

// NoRandom command
NoRandomCmd::NoRandomCmd(std::string file)
    : filename{file} {}

void NoRandomCmd::execute(IGameController &gc) {
    gc.setNoRandom(filename);
}

// Random command
void RandomCmd::execute(IGameController &gc) {
    gc.setRandom();
}
