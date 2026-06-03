#pragma once
#include <iostream>
#include <vector>
#include <variant>
#include <type_traits>
#include "bacteria.h"

struct FieldCell {
    FieldCell() = default;
    FieldCell(const FieldCell&) = default;
    FieldCell(FieldCell&&) = default;
    FieldCell& operator=(const FieldCell&) = default;
    FieldCell& operator=(FieldCell&&) = default;

    template <typename V,
              typename = std::enable_if_t<!std::is_same_v<std::decay_t<V>, FieldCell>>>
    FieldCell(V&& v) : data(std::forward<V>(v)) {}

    template <typename V,
              typename = std::enable_if_t<!std::is_same_v<std::decay_t<V>, FieldCell>>>
    FieldCell& operator=(V&& v) {
        data = std::forward<V>(v);
        return *this;
    }

    int GetCode() const {
        if (std::holds_alternative<std::monostate>(data)) return 0;
        if (std::holds_alternative<Bacterium>(data)) return 1;
        return -1;
    }

    bool IsEmpty() const {
        return GetCode() == 0;
    }

    template<typename V>
    V* as() {
        return std::get_if<V>(&data);
    }

    template<typename V>
    const V* as() const {
        return std::get_if<V>(&data);
    }

    std::variant<std::monostate, Bacterium> data;
};

class Field {
public:
    Field(int Nx, int Ny);
    ~Field() = default;

    void add_bac(int x, int y, size_t id,
                 BacteriumType type = BacteriumType::PHOTOSYNTHESIZER,
                 int bSize = 1);
    void set_cell(int x, int y, Bacterium bac);
    void delete_bac(int x, int y);

    int get_size_W() const;
    int get_size_H() const;

    const FieldCell& get_cell(int x, int y) const;
    FieldCell& get_cell(int x, int y);

    size_t getNextId() { return nextId++; }

private:
    std::vector<std::vector<FieldCell>> matrix_field;
    size_t nextId = 0;
};
