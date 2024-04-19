#ifndef MODEL_MATRIX_LIST_HPP
#define MODEL_MATRIX_LIST_HPP

#include "Matrix.hpp"
#include <vector>

class MatrixList: public std::vector<Matrix>
{
private:
  std::vector<Matrix> list;
public:
  MatrixList(std::vector<std::vector<std::vector<double>>> *v): list()
  {
    for (auto i : *v)
    {
      list.push_back(Matrix(&i));
    }
  };
  MatrixList(): list() {};

  void add(std::vector<std::vector<double>> *m)
  {
    list.push_back(Matrix(m));
  }

  void add(std::vector<std::vector<std::shared_ptr<double>>> *m)
  {
    std::vector<std::vector<double>> matrix;
    for (auto i: *m)
    {
      std::vector<double> row;
      for (auto j: i)
      {
        row.push_back(*j);
      }
      matrix.push_back(row);
    }
    list.push_back(Matrix(&matrix));
  }

  auto get(int index)
  {
    std::vector<std::vector<std::shared_ptr<double>>> result;
    auto m = list[index];
    for (int i = 0; i < m.height; i++){
      std::vector<std::shared_ptr<double>> row;
      for (auto j = m.line_begin(i); j != m.line_end(); ++j)
      {
        row.push_back(std::make_shared<double>(*j));
      }
      result.push_back(row);
    }
    return result;
  }

  std::vector<std::string> entries()
  {
    std::vector<std::string> result;
    int count = 0;
    for (auto i : list)
    {
      result.push_back(std::string("Mat") + std::to_string(++count));
    }
    result.push_back("定义矩阵");
    return result;
  }
};

#endif