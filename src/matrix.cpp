#include <vector>

class Matrix {
private:
  size_t m_rows;
  size_t m_cols;

  std::vector<double> m_data;

public:
  Matrix(size_t rows, size_t cols)
      : m_rows(rows), m_cols(cols), m_data(rows * cols) {}
};
