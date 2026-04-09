# pragma once
#include <iostream>
#include <vector>
#include <variant>
#include "bacteria.h"



struct FieldCell {
  FieldCell() = default;

  template <typename V>
  FieldCell(V&& v) : data(std::move(v)) {}

  int GetCode() const {
    if (std::holds_alternative<std::monostate>(data)) {
      return 0;
    }else if (std::holds_alternative<Bacterium>(data)) {
      return 1;
    }
    throw "unknown";
  }

  bool IsEmpty() const {
    return GetCode()  == 0;
  }
  
  template<typename V>
  V* as() {
    return std::get_if<V>(&data);
  }

  std::variant<std::monostate, Bacterium> data;
};

class Field{
public:

  Field(int Nx, int Ny);
  
  ~Field();

  void add_bac(int x, int y, size_t id);
  int get_size_W() const;
  int get_size_H() const;
  void delete_bac(int x, int y);
  const FieldCell& get_cell(int x, int y) const;
  FieldCell& get_cell(int x, int y);

  private:
  std::vector<std::vector<FieldCell>> matrix_field;
    
};