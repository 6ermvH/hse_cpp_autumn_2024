#include "bigint.hpp"

#include <algorithm>
#include <iostream>  //

BigInt::BigInt(const std::string& str)
    : _capacity(str.size()), _data(new int8_t[str.size()]), _sign(false) {
  auto it_begin = str.rbegin(), it_end = str.rend();
  if (str[0] == '-') {
    _sign = true;
    --it_end;
  }
  for (std::size_t i = 0; i < _capacity; ++i) {
    _data[i] = 0;
  }
  for (std::size_t i = 0; it_begin != it_end; ++it_begin, ++i) {
    _data[i] = (*it_begin - '0');
  }
  clean_lead_zero();
}

BigInt::BigInt(int32_t num)
    : _capacity(10), _data(new int8_t[10]), _sign(false) {
  if (num < 0) {
    _sign = true;
    num *= -1;
  }
  for (std::size_t i = 0; i < _capacity; ++i) {
    _data[i] = num % 10;
    num /= 10;
  }
  clean_lead_zero();
}

BigInt::BigInt(const BigInt& num)
    : _capacity(num._size),
      _data(new int8_t[num._size]),
      _size(num._size),
      _sign(num._sign) {
  std::copy(num._data, num._data + _size, _data);
}

BigInt::BigInt(BigInt&& num)
    : _capacity(num._capacity),
      _data(std::move(num._data)),
      _size(num._size),
      _sign(num._sign) {
  num._data = nullptr;
}

BigInt::BigInt(std::size_t capacity)
    : _capacity(capacity), _data(new int8_t[capacity]), _sign(false) {
  for (std::size_t i = 0; i < _capacity; ++i) {
    _data[i] = 0;
  }
  clean_lead_zero();
}

BigInt& BigInt::operator=(const BigInt& num) {
  if (this == &num) {
    return *this;
  }
  this->~BigInt();
  _capacity = num._size;
  _data = new int8_t[num._size];
  _size = num._size;
  _sign = num._sign;
  std::copy(num._data, num._data + _size, _data);
  return *this;
}

BigInt::~BigInt() {
  if (_data) delete[] _data;
}

std::size_t BigInt::size() const { return _size; }

std::string BigInt::to_string() const {
  std::string result;
  for (std::size_t i = 0; i < _size; ++i) {
    result += std::to_string(_data[i]);
  }
  if (_sign) {
    result += '-';
  }
  std::reverse(result.begin(), result.end());
  return result;
}

void BigInt::clean_lead_zero() {
  _size = 1;
  for (std::size_t i = 1; i < _capacity; ++i) {
    if (_data[i] != 0) {
      _size = i + 1;
    }
  }
  if (_size == 1 && _data[0] == 0) {
    _sign = false;
  }
}

BigInt BigInt::operator-() const {
  BigInt result(*this);
  result._sign = !(result._sign);
  return result;
}

BigInt operator+(const BigInt& n1, const BigInt& n2) {
  BigInt result(std::max(n1.size(), n2.size()) + 1);
  if (n1._sign == n2._sign) {
    int8_t carry = 0;
    for (std::size_t i = 0; i < result._capacity; ++i) {
      int8_t sum = carry;
      if (i < n1._size) sum += n1._data[i];
      if (i < n2._size) sum += n2._data[i];
      result._data[i] = sum % 10;
      carry = sum / 10;
    }
    result._sign = n1._sign;
  } else {
    const BigInt* larger = &n1;
    const BigInt* smaller = &n2;
    if (n1.size() < n2.size() ||
        (n1.size() == n2.size() &&
         n1._data[n1.size() - 1] < n2._data[n2.size() - 1])) {
      std::swap(larger, smaller);
    }
    int8_t borrow = 0;
    for (std::size_t i = 0; i < result._capacity; ++i) {
      int8_t diff = -borrow;
      if (i < larger->_size) diff += larger->_data[i];
      if (i < smaller->_size) diff -= smaller->_data[i];
      if (diff < 0) {
        diff += 10;
        borrow = 1;
      } else {
        borrow = 0;
      }
      result._data[i] = diff;
    }
    result._sign = larger->_sign;
  }
  result.clean_lead_zero();
  return result;
}

BigInt operator-(const BigInt& n1, const BigInt& n2) { return n1 + (-n2); }

BigInt operator*(const BigInt& n1, const BigInt& n2) {
  BigInt result(n1.size() + n2.size());
  for (std::size_t i = 0; i < n1.size(); ++i) {
    int8_t carry = 0;
    for (std::size_t j = 0; j < n2.size() || carry; ++j) {
      int16_t current = result._data[i + j] + carry;
      if (j < n2.size()) {
        current += n1._data[i] * n2._data[j];
      }
      result._data[i + j] = current % 10;
      carry = current / 10;
    }
  }
  result._sign = n1._sign != n2._sign;
  result.clean_lead_zero();
  return result;
}

std::ostream& operator<<(std::ostream& os, const BigInt& num) {
  os << num.to_string();
  return os;
}

bool operator==(const BigInt& n1, const BigInt& n2) {
  if (n1._sign != n2._sign || n1._size != n2._size) {
    return false;
  }
  for (std::size_t i = 0; i < n1._size; ++i) {
    if (n1._data[i] != n2._data[i]) {
      return false;
    }
  }
  return true;
}

bool operator!=(const BigInt& n1, const BigInt& n2) { return !(n1 == n2); }

bool operator<(const BigInt& n1, const BigInt& n2) {
  if (n1._sign != n2._sign) {
    return n1._sign;
  }
  if (n1._size != n2._size) {
    return n1._sign ? n2._size < n1._size : n1._size < n2._size;
  }
  for (std::size_t i = n1._size; i > 0; --i) {
    if (n1._data[i - 1] != n2._data[i - 1]) {
      return n1._sign ? n2._data[i - 1] < n1._data[i - 1]
                      : n1._data[i - 1] < n2._data[i - 1];
    }
  }
  return false;
}

bool operator>(const BigInt& n1, const BigInt& n2) { return n2 < n1; }

bool operator<=(const BigInt& n1, const BigInt& n2) { return !(n2 < n1); }

bool operator>=(const BigInt& n1, const BigInt& n2) { return !(n1 < n2); }
