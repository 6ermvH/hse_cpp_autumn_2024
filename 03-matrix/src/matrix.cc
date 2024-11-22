#include "matrix.hh"

#include <iostream>

int32_t& Matrix::ProxyRow::operator[](size_t j) {
  if (j >= count_column) {
    throw std::out_of_range("out of columns");
  }
  return data_[j];
} 

Matrix::ProxyRow& Matrix::operator[](size_t i) const{
  if (i >= count_rows) {
    throw std::out_of_range("out of rows");
  }
  return rows_[i];
} 

Matrix::ProxyRow::ProxyRow(size_t column, int32_t standart_value) {
  count_column = column;
  data_ = new int32_t[column];
  for (size_t i = 0; i < column; ++i) {
    data_[i] = standart_value;
  }
}

Matrix::Matrix(size_t rows, size_t column, int32_t standart_value) {
  count_rows = rows;
  count_column = column;
  rows_ = new ProxyRow[rows];
  for (size_t i = 0; i < rows; ++i) {
    Matrix::ProxyRow row(column, standart_value);
    rows_[i] = row;
    row.data_ = nullptr;
  }
}

Matrix::Matrix(const Matrix& matrix) {
  size_t rows = matrix.getRows();
  size_t column = matrix.getColumns();
  count_rows = rows;
  count_column = column;
  rows_ = new ProxyRow[rows];
  for (size_t i = 0; i < rows; ++i) {
    Matrix::ProxyRow row(column);
    rows_[i] = row;
    row.data_ = nullptr;
  }
  for (size_t i = 0; i < rows; ++i) {
    for(size_t j = 0; j < column; ++j) {
      (*this)[i][j] = matrix[i][j];
    }
  }
}

Matrix::Matrix(const std::vector<std::vector<int32_t> >& vec) {
  size_t rows = vec.size();
  size_t column = vec[0].size();
  count_rows = rows;
  count_column = column;
  rows_ = new ProxyRow[rows];
  for (size_t i = 0; i < rows; ++i) {
    Matrix::ProxyRow row(column);
    rows_[i] = row;
    row.data_ = nullptr;
  }
  for (size_t i = 0; i < rows; ++i) {
    for(size_t j = 0; j < column; ++j) {
      (*this)[i][j] = vec[i][j];
    }
  }
}

Matrix::Matrix(Matrix&& matrix) {
  this->rows_ = matrix.rows_;
  this->count_rows = matrix.count_rows;
  this->count_column = matrix.count_column;
}

void Matrix::operator=(const Matrix& matrix) {
  size_t rows = matrix.getRows();
  size_t column = matrix.getColumns();
  count_rows = rows;
  count_column = column;
  rows_ = new ProxyRow[rows];
  for (size_t i = 0; i < rows; ++i) {
    Matrix::ProxyRow row(column);
    rows_[i] = row;
    row.data_ = nullptr;
  }
  for (size_t i = 0; i < rows; ++i) {
    for(size_t j = 0; j < column; ++j) {
      (*this)[i][j] = matrix[i][j];
    }
  }
}

Matrix::~Matrix() {
    delete[] rows_;
}

Matrix::ProxyRow::~ProxyRow() {
    delete[] data_;
}

size_t Matrix::getRows() const {
  return count_rows;
}

size_t Matrix::getColumns() const {
  return count_column;
}

Matrix& Matrix::operator*=(int32_t num) {
  for (size_t i = 0; i < count_rows; ++i) {
    for (size_t j = 0; j < count_column; ++j) {
      (*this)[i][j] *= num;
    }
  }
  return *this;
}

Matrix operator+(const Matrix& m1, const Matrix& m2) {
  if (m1.count_rows != m2.count_rows || m1.count_column != m2.count_column) {
    throw std::runtime_error("matrix not equal");
  }
  Matrix res(m1.count_rows, m1.count_column);
  for (size_t i = 0; i < m1.count_rows; ++i) {
    for (size_t j = 0; j < m1.count_column; ++j) {
      res[i][j] = m1[i][j] + m2[i][j];
    }
  }
  return res;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
  for (size_t i = 0; i < matrix.getRows(); ++i) {
    for (size_t j = 0; j < matrix.getColumns(); ++j) {
      os << matrix[i][j] << ' ';
    }
    os << '\n';
  }
  return os;
}

bool operator==(const Matrix& m1, const Matrix& m2) {
  for (size_t i = 0; i < m1.getRows(); ++i) {
    for (size_t j = 0; j < m1.getColumns(); ++j) {
      if (m1[i][j] != m2[i][j]) {
        return false;
      }
    }
  }
  return true;
}

bool operator!=(const Matrix& m1, const Matrix& m2) {
  return !(m1 == m2);
}


