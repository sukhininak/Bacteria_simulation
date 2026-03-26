# pragma once
#include <iostream>
#include <vector>
#include <variant>
//#include "bacteria.h"
struct Bacteria {};

struct FieldCell {
  FieldCell() = default;

  template <typename V>
  FieldCell(V&& v) : data(std::move(v)) {}

  int GetCode() const {
    if (std::holds_alternative<std::monostate>(data)) {
      return 0;
    }else if (std::holds_alternative<Bacteria>(data)) {
      return 1;
    }
    throw "unknown";
  }

  std::variant<std::monostate, Bacteria> data;
};

class Field{
public:

  Field(int Nx, int Ny);
  
  ~Field();

  void add_bac(int x, int y);
  int get_size_W() const;
  int get_size_H() const;
  void delete_bac(int x, int y);
  Bacteria get_bac(int x, int y) const;
  FieldCell get_cell(int x, int y) const;

  private:
  std::vector<std::vector<FieldCell>> matrix_field;
    
};