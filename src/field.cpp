#include "field.h"

class Field{
  public:

  Field(int Nx, int Ny){ 
    Nx = Nx;
    Ny = Ny;
    matrix_field.resize(Nx*Ny,0); 
  }
  
  ~Field(){};

  void add_bac(int x, int y){
    if (x < Nx && y < Ny)
      matrix_field[Nx*x+y] = 1;
    else
      std::cout << "incorrect number of line or rows" << std::endl;
  } 

  void delete_bac(int x, int y){
    if (x < Nx && y < Ny)
      matrix_field[Nx*x+y] = 0;
    else
      std::cout << "incorrect number of line or rows" << std::endl;
  } 

  void move(int x_past, int y_past, int x, int y) //перемещение бактерии из точки (x_past y_past) в точку (x y)
  {
    delete_bac(x_past, y_past);
    add_bac(x,y);
  } 
  
  void relative_move(int x_past, int y_past, int dx, int dy) //перемещение бактерии из точки (x_past y_past) в точку (x_past+dx y_past+dy)
  {
    delete_bac(x_past, y_past);
    add_bac(x_past+dx, y_past+dy);
  } 

  void symbol_plot()
  {
    for (int i=0; i<Nx; i++) 
    {
      for (int j=0; j<Ny; j++)
        std::cout << matrix_field[Nx*i+j];

      std::cout << std::endl;
    }
  }

  private:
  int Nx;
  int Ny;

  std::vector <bool> matrix_field;
};


