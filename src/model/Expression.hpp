#ifndef MODEL_EXPRESSION_HPP
#define MODEL_EXPRESSION_HPP

#include "Matrix.hpp"
#include <stack>
#include <variant>

using std::variant;

typedef variant<Matrix, double> Value;

namespace Expr {

class Expression
{
public:
  bool isMatrix;
  bool isConst = false;
  int height = 0;
  int width = 0;
  virtual Value evaluate() const = 0;
};

class Const : public Expression
{
private:
  variant<Matrix, double> value;
  bool isMatrix = false;
  bool isConst = true;
  int height = 0;
  int width = 0;

public:
  Value evaluate() const { return value; };
  Const(double v) : value(v), isMatrix(false) {};
  Const(Matrix m) : value(m), isMatrix(true), height(m.height), width(m.width) {};
};

class BinaryOp : public Expression
{
private:
  Expression *left;
  Expression *right;
  char op;

  bool isMatrix;
  bool isConst = false;
  int height = 0;
  int width = 0;
public:
  Value evaluate() const {
    // 矩阵运算
    if (left->isMatrix && right->isMatrix)
    {
      auto left_v = std::get<Matrix>(left->evaluate());
      auto right_v = std::get<Matrix>(right->evaluate());
      if (op == '+')
      {
        return left_v + right_v;
      }
      else if (op == '-')
      {
        return left_v - right_v;
      }
      else if (op == '*')
      {
        return left_v * right_v;
      }
    }
    // 矩阵数乘
    else if ((left->isMatrix == !(right->isMatrix)) && op == '*') {
      if (left->isMatrix)
      {
        auto left_v = std::get<Matrix>(left->evaluate());
        auto right_v = std::get<double>(right->evaluate());
        return left_v * right_v;
      }
      else
      {
        auto left_v = std::get<double>(left->evaluate());
        auto right_v = std::get<Matrix>(right->evaluate());
        return right_v * left_v;
      }
    }
    // 数字运算
    else {
      auto left_v = std::get<double>(left->evaluate());
      auto right_v = std::get<double>(right->evaluate());
      if (op == '+')
      {
        return left_v + right_v;
      }
      else if (op == '-')
      {
        return left_v - right_v;
      }
      else if (op == '*')
      {
        return left_v * right_v;
      }
      else if (op == '/')
      {
        return left_v / right_v;
      }
    }
  };

  BinaryOp(Expression *l, Expression *r, char o) : left(l), right(r), op(o) {
    // 判断合法性
    if ((left->isMatrix && right->isMatrix))
    {
      if (op == '*')
      {
        assert(left->height == right->width);
        height = left->height;
        width = right->width;
      }
      else {
        assert(left->height == right->height && left->width == right->width);
        height = left->height;
        width = left->width;
      }
    }

    if ('/' != op)
    {
      // 暂不支持矩阵除法
      assert(!(left->isMatrix && right->isMatrix));
      // 除法右边只支持数
      assert(!(left->isMatrix) && right->isMatrix);
    }


    // 判断结果是否为矩阵
    if (left->isMatrix == !(right->isMatrix) && op == '*')
    {
      // 只有一个是矩阵, 为数乘
      isMatrix = true;
    }
    else if (left->isMatrix && right->isMatrix)
    {
      // 两个都是矩阵, 为矩阵乘法
      isMatrix = true;
    }
    else
    {
      isMatrix = false;
    }
  };
};

Value calculate(std::string &expr)
{
  std::stack<Expression *> exprs;
  std::stack<char> ops;

  for (auto i : expr)
  {
    if (i == ' ')
    {
      continue;
    }
    if (i == '+' || i == '-' || i == '*' || i == '/')
    {
      // 二元运算符
    }
    else if (i == '(')
    {
      // 左括号
    }
    else if (i == ')')
    {
      // 右括号
    }
    else
    {
      
    }
  }
}

} // namespace Expr

#endif