#pragma once
#include <vector>

template<typename T>
struct Grid {
    int r, c;
    std::vector<std::vector<T>> data;

    Grid(int rows, int cols) : r(rows), c(cols),
        data(rows, std::vector<T>(cols)) {}

    int rows() const { return r; }
    int cols() const { return c; }

    T& get(int i, int j) { return data[i][j]; }
    void set(int i, int j, T val) { data[i][j] = val; }
};