#pragma once
#include "src/bacteria.h"

class Command{
public:
    Command();

    virtual bool Exec(...) = 0; // returns true if command is a terminal

};

class Move: public Command {
    Move(BacteriumState state, int coord);
};

class Look: public Command {
    Look();
};