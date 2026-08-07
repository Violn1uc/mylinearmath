#include "mylinearmath.hpp"
// #include <cmath>
#include <iostream>

void PrintVector(const Vector &v) {
  for (int i = 0; i < v.size(); ++i) {
    std::cout << v[i] << " ";
  }
  std::cout << "\n";
}

Vector operator*(const Vector &v, int n) {
  Vector result(v.size());
  for (size_t i = 0; i < v.size(); i++) {
    result[i] = v[i] * n;
  }
  return result;
}

Vector operator+(const Vector &v1, const Vector &v2) {
  if (v1.size() != v2.size()) {
    throw std::invalid_argument("Vector sizes must match");
  }
  Vector result(v1.size());
  for (size_t i = 0; i < v1.size(); i++) {
    result[i] = v1[i] + v2[i];
  }
  return result;
}

double Vector::length() {
  double result(0);
  for (size_t i = 0; i < size(); i++) {
    result += elem[i] * elem[i];
  }
  return std::sqrt(result);
}
