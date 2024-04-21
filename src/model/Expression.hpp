#ifndef MODEL_EXPRESSION_HPP
#define MODEL_EXPRESSION_HPP

#include "Matrix.hpp"
#include <stack>
#include <variant>

#include "MatrixList.hpp"

using std::variant;
using std::get;

typedef variant<Matrix, double> Value;

namespace Expr {

class Const
{
private:
  variant<Matrix, double> value;
public:
  bool isMatrix = false;
  int height = 0;
  int width = 0;
  double _v_f = 0.0;

public:
  Value evaluate() const { return value; };
  Const(double v) : value(v), isMatrix(false), _v_f(v) {};
  Const(Matrix m) : value(m), isMatrix(true), height(m.height), width(m.width) {};
};

Const process(std::stack<Const> &st, char op)
{
  if (op < 0) {
    auto l = st.top();
    st.pop();
    if (-op == '-')
    {
      if (l.isMatrix)
      {
        auto l_v = get<Matrix>(l.evaluate());
        return Const { -l_v };
      }
      else
      {
        auto l_v = get<double>(l.evaluate());
        return Const { -l_v };
      }
    }
    else
    {
      throw std::runtime_error("RuntimeError: Undefined behavior");
    }
  }
  else
  {
    auto r = st.top();
    st.pop();
    auto l = st.top();
    st.pop();

    // 判断合法性
    if ((l.isMatrix && r.isMatrix))
    {
      if (op == '*')
      {
        if (l.height != r.width)
        {
          throw std::runtime_error("MathError: 矩阵尺寸不匹配");
        }
      }
      else {
        if (l.height != r.height || l.width != r.width)
        {
          throw std::runtime_error("MathError: 矩阵尺寸不匹配");
        }
      }
    }

    if ('/' == op)
    {
      // 暂不支持矩阵除法
      if (l.isMatrix && r.isMatrix)
      {
        throw std::runtime_error("MathError: 暂不支持矩阵除法");
      }
      // 除法右边不支持矩阵
      if (!(l.isMatrix) && r.isMatrix)
      {
        throw std::runtime_error("MathError: 除法右边不支持矩阵");
      }
    }

    auto l_v = l.evaluate(), r_v = r.evaluate();
    // 计算
    if (op == '+') {
      if (l.isMatrix && r.isMatrix)
      {
        return Const{get<Matrix>(l_v) + get<Matrix>(r_v)};
      }
      else if (!l.isMatrix && !l.isMatrix)
      {
        return Const{get<double>(l_v) + get<double>(r_v)};
      }
      else
      {
        throw std::runtime_error("MathError: Matrix + Const");
      }
    }
    else if (op == '-')
    {
      if (l.isMatrix && r.isMatrix)
      {
        return Const{get<Matrix>(l_v) - get<Matrix>(r_v)};
      }
      else if (!l.isMatrix && !l.isMatrix)
      {
        return Const{get<double>(l_v) - get<double>(r_v)};
      }
      else
      {
        throw std::runtime_error("MathError: Matrix - Const");
      }
    }
    else if (op == '*')
    {
      if (l.isMatrix && r.isMatrix)
      {
        return Const{get<Matrix>(l_v) * get<Matrix>(r_v)};
      }
      else if (l.isMatrix == !(r.isMatrix))
      {
        // 数乘
        if (l.isMatrix)
        {
          return Const{get<Matrix>(l_v) * get<double>(r_v)};
        }
        else
        {
          return Const{get<double>(l_v) * get<Matrix>(r_v)};
        }
      }
      else
      {
        auto l_d = get<double>(l_v), r_d = get<double>(r_v);
        return Const{get<double>(l_v) * get<double>(r_v)};
      }
    }
    else if (op == '/') {
      if (l.isMatrix && !(r.isMatrix))
      {
        return Const{get<Matrix>(l_v) / get<double>(r_v)};
      }
      else
      {
        return Const{get<double>(l_v) / get<double>(r_v)};
      }
    }
    else
    {
      throw std::runtime_error("RuntimeError: Undefined behavior");
    }
  }
};

bool is_op(char c) { return c == '+' || c == '-' || c == '*' || c == '/'; };
bool is_unary(char c) { return c == '-'; }
int priority(char op) {
  if (op < 0) // unary operator
    return 3;
  if (op == '+' || op == '-')
    return 1;
  if (op == '*' || op == '/')
    return 2;
  return -1;
}
bool is_num(char c) { return c >= '0' && c <= '9'; }
bool is_double(char c) { return is_num(c) || c == '.'; }
bool is_double(char c, int &dots) {
  if (c == '.') dots++;
  return is_num(c) || c == '.';
}

Const calculate(std::string expr, MatrixList &ml)
{
  std::stack<Const> exprs;
  std::stack<char> ops;
  // 初始时可能为单元运算
  bool may_be_unary = true;

  for (int i = 0; i < expr.size(); i++)
  {
    if (expr[i] == ' ')
    {
      continue;
    }
    if (expr[i] == '(')
    {
      ops.push('(');
      // 左括号后可能为单元运算
      may_be_unary = true;
    }
    else if (expr[i] == ')')
    {
      while (ops.top() != '(')
      {
        exprs.push(process(exprs, ops.top()));
        ops.pop();
      }
      // 左括号出栈
      ops.pop();
      // 不会为单元运算
      may_be_unary = false;
    }
    else if (is_op(expr[i]))
    {
      char op = expr[i];
      if (may_be_unary && is_unary(op))
      {
        op = -op;
      }
      while (
        !ops.empty() &&
        ((op >= 0 && priority(ops.top()) >= priority(op)) ||
        (op < 0 && priority(ops.top()) > priority(op)))
      )
      {
        exprs.push(process(exprs, op));
        ops.pop();
      }
      ops.push(op);
      may_be_unary = true;
    }
    // 读取数字
    else if (is_double(expr[i]))
    {
      std::string number = "";
      int dots = 0;
      while (
        i < expr.size() &&
        is_double(expr[i], dots)
      )
      {
        number += expr[i];
        i++;
      }
      i--;

      if (dots > 1)
      {
        throw std::runtime_error("MathError: 错误的小数点");
      }

      auto r = std::stod(number);
      exprs.push(Const{r});
      may_be_unary = false;
    }
    // 读取矩阵
    else if (expr[i] == 'M')
    {
      if(expr[++i] == 'a' && expr[++i] == 't')
      {
        std::string matrix_num = "";
        ++i;
        while (i < expr.size() && is_num(expr[i]))
        {
          matrix_num += expr[i++];
        }
        --i;
        
        int idx = std::stoi(matrix_num) - 1;
        if (idx >= (ml).size())
        {
          throw std::runtime_error("ReferenceError: 不存在的矩阵");
        }
        exprs.push(Const{ (ml).get_mat(idx) });
        may_be_unary = false;
      }
      else {
        throw std::runtime_error("SyntaxError: 解析失败");
      }
    }
    else {
      throw std::runtime_error("SyntaxError: 解析失败");
    }
  }

  while (!ops.empty())
  {
    auto r = process(exprs, ops.top());

    // double v = 0.0;
    // if (!r.isMatrix) v = get<double>(r.evaluate());
    exprs.push(r);
    ops.pop();
  }

  auto r = exprs.top();
  return r;
}

} // namespace Expr

#endif