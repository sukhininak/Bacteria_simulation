#include "field.h"

Field::Field(int Nx, int Ny)
{
  matrix_field.resize(Nx);
  for (auto& column: matrix_field) {
    column.resize(Ny);
  }
}

Field::~Field(){};

void Field::add_bac(int x, int y, size_t id)
{ 
  if (x < matrix_field.size() && y < matrix_field[x].size())
    matrix_field[x][y] = Bacterium(x, y, id);
  else
    std::cout << "incorrect number of line or rows" << std::endl;
}

void Field::delete_bac(int x, int y)
{
  if (x < matrix_field.size() && y < matrix_field[x].size())
    matrix_field[x][y] = std::monostate{};
  else
    std::cout << "incorrect number of line or rows" << std::endl;
}

int Field::get_size_W() const{
  return matrix_field.size();
}
int Field::get_size_H() const{
  return  matrix_field[0].size();
}

const FieldCell& Field::get_cell(int x, int y) const {
  return matrix_field[x][y];
}

FieldCell& Field::get_cell(int x, int y) {
  return matrix_field[x][y];
}



