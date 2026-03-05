#pragma once
#include "src/bacteria.h"

struct Context {
 Field* field;
 Bacteria* bacteria;//state 
};


class Command{
public:
    Command();

    virtual bool Exec(Context& ctx) = 0; // returns true if command is a terminal

};

class Move: public Command {
    Move();
    bool Exec(Context& ctx) override;// обновляет поле
};

class Look: public Command {
    Look();
};