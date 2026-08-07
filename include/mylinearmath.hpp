#pragma once

#include <cstddef>

class Vector {
public:
  Vector(size_t s) : elem{new double[s]}, sz{s} {}
  double &operator[](size_t i) { return elem[i]; }
  double operator[](size_t i) const { return elem[i]; }
  size_t size() const { return sz; }
  friend Vector operator*(const Vector &v, int n);
  friend Vector operator+(const Vector &v1, const Vector &v2);
  double length();

private:
  double *elem;
  size_t sz;
};

void PrintVector(const Vector &v);
