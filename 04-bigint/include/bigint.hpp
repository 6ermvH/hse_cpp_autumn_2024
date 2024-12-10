#pragma once

#include <cstdint>
#include <string>

class BigInt {
 public:
  BigInt(const std::string& str);
  BigInt(int32_t num);
  BigInt(const BigInt& num);
  BigInt(BigInt&& num);
  BigInt& operator=(const BigInt& num);
  ~BigInt();

  std::size_t size() const;

  std::string to_string() const;

  BigInt operator-() const;

  friend BigInt operator+(const BigInt& n1, const BigInt& n2);
  friend BigInt operator-(const BigInt& n1, const BigInt& n2);
  friend BigInt operator*(const BigInt& n1, const BigInt& n2);
  friend BigInt operator/(const BigInt& n1, const BigInt& n2);

  friend bool operator==(const BigInt& n1, const BigInt& n2);
  friend bool operator!=(const BigInt& n1, const BigInt& n2);
  friend bool operator<(const BigInt& n1, const BigInt& n2);
  friend bool operator>(const BigInt& n1, const BigInt& n2);
  friend bool operator<=(const BigInt& n1, const BigInt& n2);
  friend bool operator>=(const BigInt& n1, const BigInt& n2);

 private:
  BigInt(std::size_t capacity);

  void clean_lead_zero();

  std::size_t _capacity;
  int8_t* _data;
  std::size_t _size;
  bool _sign;
};

std::ostream& operator<<(std::ostream& os, const BigInt& num);
