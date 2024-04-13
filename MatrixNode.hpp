#include <memory>
#ifndef MATRIX_NODE_HPP
#define MATRIX_NODE_HPP

class MatrixNode
{
public:
  std::shared_ptr<MatrixNode> right;
  std::shared_ptr<MatrixNode> down;
  double value;
  MatrixNode(double v) : value(v), right(nullptr), down(nullptr) {}
};

#endif