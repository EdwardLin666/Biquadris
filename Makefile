CXX = g++-14
CXXFLAGS = -std=c++20 -fmodules-ts -Wall -g
CXXHEADER = -std=c++20 -fmodules-ts -c -x c++-system-header

# Object files (ORDER MATTERS for modules!)
# Dependency chain: Command(fwd decl GC) -> CommandInterpreter -> GameController
OBJS = block.o board.o blocks.o level.o \
       level0.o level1.o level2.o level3.o level4.o \
       levelfactory.o player.o player-impl.o \
       display.o textdisplay.o graphicdisplay.o command.o commandinterpreter.o gamecontroller.o \
       command-impl.o commandinterpreter-impl.o gamecontroller-impl.o \
       main.o

TARGET = biquadris

all: header $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)  -L/usr/lib/x86_64-linux-gnu  -lX11

# Compile standard headers (string must be last!)
header:
	$(CXX) $(CXXHEADER) iostream
	$(CXX) $(CXXHEADER) vector
	$(CXX) $(CXXHEADER) algorithm
	$(CXX) $(CXXHEADER) memory
	$(CXX) $(CXXHEADER) fstream
	$(CXX) $(CXXHEADER) sstream
	$(CXX) $(CXXHEADER) cstdlib
	$(CXX) $(CXXHEADER) utility
	$(CXX) $(CXXHEADER) X11/Xlib.h
	$(CXX) $(CXXHEADER) X11/Xutil.h
	$(CXX) $(CXXHEADER) cstring
	$(CXX) $(CXXHEADER) string

# === Base modules ===
block.o: block.cc
	$(CXX) $(CXXFLAGS) -c block.cc

board.o: board.cc
	$(CXX) $(CXXFLAGS) -c board.cc

blocks.o: IJLOSTB-blocks.cc
	$(CXX) $(CXXFLAGS) -c IJLOSTB-blocks.cc -o blocks.o

level.o: level.cc
	$(CXX) $(CXXFLAGS) -c level.cc

level0.o: level0.cc
	$(CXX) $(CXXFLAGS) -c level0.cc

level1.o: level1.cc
	$(CXX) $(CXXFLAGS) -c level1.cc

level2.o: level2.cc
	$(CXX) $(CXXFLAGS) -c level2.cc

level3.o: level3.cc
	$(CXX) $(CXXFLAGS) -c level3.cc

level4.o: level4.cc
	$(CXX) $(CXXFLAGS) -c level4.cc

levelfactory.o: levelfactory.cc
	$(CXX) $(CXXFLAGS) -c levelfactory.cc

# === Player ===
player.o: player.cc
	$(CXX) $(CXXFLAGS) -c player.cc

player-impl.o: player-impl.cc
	$(CXX) $(CXXFLAGS) -c player-impl.cc

# === Display ===
display.o: display.cc
	$(CXX) $(CXXFLAGS) -c display.cc

textdisplay.o: textdisplay.cc
	$(CXX) $(CXXFLAGS) -c textdisplay.cc

graphicdisplay.o: graphicdisplay.cc
	$(CXX) $(CXXFLAGS) -c graphicdisplay.cc

# === Command/Interpreter/Controller chain ===
# Command uses forward declaration of GameController (no import)
command.o: command.cc
	$(CXX) $(CXXFLAGS) -c command.cc

# CommandInterpreter imports Command
commandinterpreter.o: commandinterpreter.cc
	$(CXX) $(CXXFLAGS) -c commandinterpreter.cc

# GameController imports CommandInterpreter
gamecontroller.o: gamecontroller.cc
	$(CXX) $(CXXFLAGS) -c gamecontroller.cc

# === Implementation files ===
# command-impl is a regular file (not module impl), imports Command and GameController
command-impl.o: command-impl.cc
	$(CXX) $(CXXFLAGS) -c command-impl.cc

commandinterpreter-impl.o: commandinterpreter-impl.cc
	$(CXX) $(CXXFLAGS) -c commandinterpreter-impl.cc

gamecontroller-impl.o: gamecontroller-impl.cc
	$(CXX) $(CXXFLAGS) -c gamecontroller-impl.cc

# === Main ===
main.o: main.cc
	$(CXX) $(CXXFLAGS) -c main.cc

clean:
	rm -f $(TARGET) *.o
	rm -rf gcm.cache

.PHONY: all clean header
