#include <memory>
#include "MatrixNode.hpp"

#ifndef MATRIX_ITERS_HPP
#define MATRIX_ITERS_HPP

namespace iters
{
  // 遍历迭代器
  class Iterator
  {
  private:
    std::shared_ptr<MatrixNode> head;
    std::shared_ptr<MatrixNode> line_head;

  public:
    Iterator(std::shared_ptr<MatrixNode> h) : head(h)
    {
      line_head = head;
    };
    Iterator &operator++()
    {
      if (head->right)
      {
        head = head->right;
      }
      else
      {
        head = line_head->down;
        line_head = head;
      }
      return *this;
    }
    double operator*() const
    {
      return head->value;
    }
    MatrixNode *operator&() const
    {
      return head.get();
    }
    bool operator!=(const Iterator &i) const
    {
      return head != i.head;
    }
  };


  // 遍历行迭代器
  class LineIterator
  {
  public:
    std::shared_ptr<MatrixNode> head;
    LineIterator(std::shared_ptr<MatrixNode> h) : head(h){};
    LineIterator &operator++()
    {
      head = head->right;
      return *this;
    }
    double operator*() const
    {
      return head->value;
    }
    MatrixNode *operator&() const
    {
      return head.get();
    }
    bool operator!=(const LineIterator &i) const
    {
      return head != i.head;
    }
  };


  // 遍历列迭代器
  class ColumnIterator
  {
  private:
    std::shared_ptr<MatrixNode> head;

  public:
    ColumnIterator(std::shared_ptr<MatrixNode> h) : head(h){};
    ColumnIterator &operator++()
    {
      head = head->down;
      return *this;
    }
    double operator*() const
    {
      return head->value;
    }
    bool operator!=(const ColumnIterator &i) const
    {
      return head != i.head;
    }
  };
}

#endif