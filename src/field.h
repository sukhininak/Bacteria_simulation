# pragma once
#include <iostream>
#include <vector>

struct FieldUpdate{
    //какая еда съедена где умер жук 
};

class Field{
public:

  Field(int Nx, int Ny);
  
  ~Field();

  void add_bac(int x, int y);

  void delete_bac(int x, int y);
  void move(int x_past, int y_past, int x, int y); //перемещение бактерии из точки (x_past y_past) в точку (x y)
 
  
  void relative_move(int x_past, int y_past, int dx, int dy); //перемещение бактерии из точки (x_past y_past) в точку (x_past+dx y_past+dy)
 

  void symbol_plot();  // поле символами в терминале
  

  void Update(FieldUpdate res);
  
  private:
  int Nx;
  int Ny;

  std::vector <bool> matrix_field;
    
};