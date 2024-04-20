#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "matrixbase.hpp"
#include <vector>
#include <cmath>

class Matrix
{
private:
  MatrixBase base;

public:
  int width;
  int height;

  // 初始化
  Matrix(int w, int h) : base(w, h), width(w), height(h){};
  Matrix(std::vector<std::vector<double>> *v) : base(v->front().size(), v->size()), width(v->front().size()), height(v->size())
  {
    setAll(*v);
  };

  // 暴露MatrixBase中部分方法
  void show() { base.show(); };
  void showT() { base.showT(); }
  std::string to_string() { return base.to_string(); }
  double at(int r, int c) { return base(r, c); }
  void set(int r, int c, double v) { base.set(r, c, v); }
  void swap_line(int f, int t) { base.swap_line(f, t); }
  void remove_row(int r) { base.remove_row(r); --height; }
  void remove_col(int c) { base.remove_col(c); --width; }

  // 迭代器初始化
  auto begin() { return base.begin(); };
  auto end() { return base.end(); };
  auto line_begin(int n) { return base.line_begin(base.at(n, 0)); }
  auto line_end() { return base.line_end(); }
  auto col_begin(int n) { return base.col_begin(base.at(0, n)); }
  auto col_end() { return base.col_end(); }

  // 全部赋值
  void setAll(std::vector<std::vector<double>> v)
  {
    auto it = begin();
    for (auto i : v)
    {
      for (auto j : i)
      {
        (&it)->value = j;
        ++it;
      }
    }
  }

  // 重载赋值
  Matrix &operator=(std::vector<std::vector<double>> v)
  {
    assert(v.size() == height && v[0].size() == width);
    setAll(v);
    return *this;
  }

  // 重载加法
  Matrix operator+(Matrix addition)
  {
    assert(addition.width == width && addition.height == height);
    Matrix result{width, height};

    // 取三个矩阵迭代器
    auto it_res = result.begin();
    auto it_base = base.begin();
    auto it_add = addition.begin();

    while (it_res != base.end())
    {
      (&it_res)->value = *it_base + *it_add;
      ++it_res;
      ++it_base;
      ++it_add;
    }
    return result;
  }

  // 重载负数
  Matrix operator-()
  {
    auto result = Matrix{width, height};
    auto it = result.begin();
    for (it; it != result.end(); ++it)
    {
      (&it)->value = -*it;
    }
    return result;
  }

  // 重载减法
  Matrix operator-(Matrix addition)
  {
    assert(addition.width == width && addition.height == height);
    return operator+(-addition);
  }

  // 数乘重载
  Matrix operator*(double scale)
  {
    Matrix result{width, height};
    int count = 0;
    for (auto it = base.begin(); it != base.end(); ++it)
    {
      result.set(count / width, count % width, *it * scale);
      count++;
    }
    return result;
  }

  // 数乘重载 使用友元重载到`double`上实现`double * Matrix`
  friend Matrix operator*(double multi, Matrix matrix)
  {
    return matrix * multi;
  };

  // 右乘重载
  Matrix operator*(Matrix multi)
  {
    assert(multi.height == width);

    Matrix result{multi.width, height};

    for (int i = 0; i < result.height; i++)
    {
      for (int j = 0; j < result.width; j++)
      {
        double sum = 0.0;
        auto it_l = line_begin(i);
        auto it_r = multi.col_begin(j);
        while (it_l != line_end() && it_r != multi.col_end())
        {
          sum += *it_l * *it_r;
          ++it_l;
          ++it_r;
        }
        result.set(i, j, sum);
      }
    }

    return result;
  }

  // 数除重载
  Matrix operator/(double scale) {
    Matrix result{width, height};
    int count = 0;
    for (auto it = base.begin(); it != base.end(); ++it) {
      result.set(count / width, count % width, *it / scale);
      count++;
    }
    return result;
  }

  // 深拷贝
  Matrix deep_copy()
  {
    auto n = Matrix{width, height};
    auto it_n = n.begin();
    auto it_v = begin();
    for (; it_n != n.end(); ++it_n, ++it_v)
    {
      (&it_n)->value = *it_v;
    }
    return n;
  }


  // 高斯消元
  Matrix row_reduce()
  {
    auto res = deep_copy();
    int row_offset = 0;
    for (int i = 0; i < res.height; i++)
    {
      // 寻找最小主元
      int minRow = i - row_offset;
      for (int j = i + 1; j < res.height; j++)
      {
        if (fabs(res.at(j - row_offset, i)) < fabs(res.at(minRow, i)) && fabs(res.at(j - row_offset, i)) > 1e-8)
        {
          minRow = j - row_offset;
        }
      }

      // 将主元放在对角线
      if (i - row_offset != minRow)
        res.swap_line(i - row_offset, minRow);

      if (fabs(res.at(i - row_offset, i)) < 1e-9)
      {
        // 应对非满秩情况偏移
        row_offset++;
        continue;
      }

      int main_row = i - row_offset;

      // 主元行归一化
      if (res.at(main_row, i) != 1)
      {
        double unify = res.at(main_row, i);
        for (int j = 0; j < width; j++)
        {
          double v = res.at(main_row, j) / unify;
          res.set(main_row, j, v < 1e-8 ? 0 : v);
        }
      }

      // 其他行消元
      for (int j = 0; j < res.height; j++)
      {
        // 跳过本行
        if (j == main_row) continue;
        double ratio = res.at(j, i) / res.at(main_row, i);
        for (int k = i; k < res.width; k++)
        {
          double v = res.at(j, k) - ratio * res.at(main_row, k);
          res.set(j, k, fabs(v) < 1e-8 ? 0 : v);
        }
      }
    }

    return res;
  }


  // 行列式
  double determinant()
  {
    assert(width == height);
    if (width == 1 && height == 1)
    {
      return at(0,0);
    }

    double res = 0.0;
    for (int c = 0; c < width; c++)
    {
      auto m = deep_copy();
      m.remove_col(c);
      m.remove_row(0);
      
      // 遇到0直接得0
      if (at(0, c) == 0) continue;

      if (c % 2 == 0)
      {
        res += at(0,c) * m.determinant();
      }
      else
      {
        res -= at(0,c) * m.determinant();
      }
    }
    return res;
  }
};

#endif