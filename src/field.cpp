#include "field.h"

Field::Field(int Nx, int Ny)
{
  matrix_field.resize(Nx);
  for (auto& column: matrix_field) {
    column.resize(Ny);
  }
}

Field::~Field(){};

void Field::add_bac(int x, int y)
{
  if (x < matrix_field.size() && y < matrix_field[x].size())
    matrix_field[x][y] = Bacteria{};
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

  Bacteria Field::get_bac(int x, int y){
    return std::get<Bacteria>(matrix_field[x][y].data);
  };



