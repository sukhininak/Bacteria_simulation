#pragma once
 #include "src/bacteria.h"
#include "src/field.h"

#include <vector>


struct Context {
  Field* field;
  Bacterium* bacterium;//state 
};

class VirtualMachine{
public:
    VirtualMachine();
    void Start();
};

class Command{
public:
    Command() = default;
    virtual ~Command() = default;
    virtual bool Exec(Context& ctx) = 0; // returns true if command is a terminal
};

class Move: public Command {
    public:
    Move() = default;
    bool Exec(Context& ctx) override;// обновляет поле
};

class Look: public Command {
    Look();
    bool Exec(Context& ctx) override;// обновляет поле
};


class Simulator {
    public:
    Simulator(Field* field);
    unsigned int GetStep() const;
    void SimulateStep();

    private:
    Field* field;
    unsigned int step;
};