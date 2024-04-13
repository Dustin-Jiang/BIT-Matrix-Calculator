#include "matrixbase.hpp"
#ifndef MATRIX_TYPE
#define MATRIX_TYPE double
#endif

class Matrix
{
private:
  MatrixBase<MATRIX_TYPE> base;

public:
  int width;
  int height;
  Matrix(int w, int h) : base(w, h), width(w), height(h){};
  void show() { base.show(); };
  // Matrix operator *(MATRIX_TYPE scale)
  // {

  // }
};