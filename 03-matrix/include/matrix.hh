#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

class Matrix {
private:
  class ProxyRow {
  public:
    ProxyRow() = default;
    ProxyRow(size_t column, int32_t standart_value = 0);
    ~ProxyRow();

    int32_t& operator[](size_t j);
    int32_t* data_;
    size_t count_column;
  };

public:
  Matrix(size_t rows = 1, size_t column = 1, int32_t standart_value = 0);
  Matrix(const Matrix& matrix);
  Matrix(const std::vector<std::vector<int32_t> >& vec);
  Matrix(Matrix&& matrix);

  void operator=(const Matrix& matrix);

  ~Matrix();

  size_t getRows() const;
  size_t getColumns() const;
  
  friend Matrix operator+(const Matrix& m1, const Matrix& m2);

  Matrix& operator*=(int32_t num);

  ProxyRow& operator[](size_t i) const;

protected:
  size_t count_rows, count_column;
  ProxyRow* rows_ = nullptr;
};

std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
bool operator==(const Matrix& m1, const Matrix& m2);
bool operator!=(const Matrix& m1, const Matrix& m2);
