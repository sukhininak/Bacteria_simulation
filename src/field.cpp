#include "field.h"

Field::Field(int Nx, int Ny) {
    matrix_field.resize(Nx);
    for (auto& column : matrix_field) {
        column.resize(Ny);
    }
}

void Field::add_bac(int x, int y, size_t id, BacteriumType type, int bSize) {
    if (x >= 0 && x < (int)matrix_field.size() &&
        y >= 0 && y < (int)matrix_field[0].size())
    {
        matrix_field[x][y] = Bacterium(x, y, id, type, bSize);
    } else {
        std::cout << "add_bac: incorrect coordinates (" << x << ", " << y << ")" << std::endl;
    }
}

void Field::set_cell(int x, int y, Bacterium bac) {
    if (x >= 0 && x < (int)matrix_field.size() &&
        y >= 0 && y < (int)matrix_field[0].size())
    {
        matrix_field[x][y] = FieldCell(std::move(bac));
    }
}

void Field::delete_bac(int x, int y) {
    if (x >= 0 && x < (int)matrix_field.size() &&
        y >= 0 && y < (int)matrix_field[0].size())
    {
        matrix_field[x][y] = std::monostate{};
    }
}

int Field::get_size_W() const {
    return static_cast<int>(matrix_field.size());
}

int Field::get_size_H() const {
    return static_cast<int>(matrix_field[0].size());
}

const FieldCell& Field::get_cell(int x, int y) const {
    return matrix_field[x][y];
}

FieldCell& Field::get_cell(int x, int y) {
    return matrix_field[x][y];
}
