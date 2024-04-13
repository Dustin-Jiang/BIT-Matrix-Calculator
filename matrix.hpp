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

  // 初始化
  Matrix(int w, int h) : base(w, h), width(w), height(h){};

  // 暴露MatrixBase中部分方法
  void show() { base.show(); };
  void showT() { base.showT(); }
  MATRIX_TYPE at(int r, int c) { return base(r,c); }
  void set(int r, int c, MATRIX_TYPE v) { base.set(r,c,v); }
  auto line_begin() { return base.line_begin(); }
  auto col_begin() { return base.col_begin(); }

  // 重载运算符
  Matrix operator+ (Matrix addition)
  {
    assert(addition.width == width && addition.height == height);
    Matrix result {width, height};

    auto it_res = result.line_begin();
    auto it_base = base.line_begin();
    auto it_add = addition.line_begin();
    while (it_res != base.line_end())
    {
      (&it_res)->value = *it_base + *it_add;
      ++it_res; ++it_base; ++it_add;
    }
    return result;
  }
  Matrix operator- (Matrix addition)
  {
    assert(addition.width == width && addition.height == height);
    Matrix result{width, height};

    auto it_res = result.line_begin();
    auto it_base = base.line_begin();
    auto it_add = addition.line_begin();
    while (it_res != base.line_end())
    {
      (&it_res)->value = *it_base - *it_add;
      ++it_res;
      ++it_base;
      ++it_add;
    }
    return result;
  }
  Matrix operator* (MATRIX_TYPE scale)
  {
    Matrix result{width, height};
    int count = 0;
    for (auto it = base.line_begin(); it != base.line_end(); ++it)
    {
      result.set(count / width, count % width, *it * scale);
      count++;
    }
    return result;
  }
};