#include <memory>
#include <iostream>
#include <assert.h>

#include "MatrixNode.hpp"
#include "MatrixIters.hpp"

class MatrixBase
{
private:
  std::shared_ptr<MatrixNode> head;

public:
  int width;
  int height;


  // 初始化迭代器
  iters::Iterator begin() const
  {
    return iters::Iterator(head);
  };
  iters::Iterator end() const
  {
    return iters::Iterator(nullptr);
  };
  iters::LineIterator line_begin(std::shared_ptr<MatrixNode> h) const
  {
    return iters::LineIterator(h);
  };
  iters::LineIterator line_end() const
  {
    return iters::LineIterator(nullptr);
  };
  iters::ColumnIterator col_begin(std::shared_ptr<MatrixNode> h) const
  {
    return iters::ColumnIterator(h);
  }
  iters::ColumnIterator col_end() const
  {
    return iters::ColumnIterator(nullptr);
  }


  // 初始化矩阵
  MatrixBase(int w, int h) : width(w), height(h)
  {
    int count = 0;
    head = std::make_shared<MatrixNode>(0);

    // 初始化所有值
    std::shared_ptr<MatrixNode> curr = head;
    // 上一行行首指针
    std::shared_ptr<MatrixNode> prev_line = std::make_shared<MatrixNode>(0);
    // 上一行对应值指针
    std::shared_ptr<MatrixNode> prev_head = nullptr;
    prev_line.get()->down = head;
    
    for (int i = 1; i <= h; i++)
    {
      for (int j = 1; j < w; j++)
      {
        count++;
        curr->right = std::make_shared<MatrixNode>(count);

        // 如果上一行有值, 设置上一行对应节点down指向刚刚创建的节点
        if (prev_head && prev_head->right)
        {
          prev_head->right->down = curr->right;
          prev_head = prev_head->right;
        }

        curr = curr->right;
      }

      // 下一行处理
      if (i < h)
      {
        count++;
        prev_line->down->down = std::make_shared<MatrixNode>(count);
        prev_line = prev_line->down;
        curr = prev_line->down;
        prev_head = prev_line;
      }
    }
  };


  // 显示矩阵
  void show()
  {
    for (int i = 0; i < height; i++) {
      for (auto it = line_begin(at(i,0)); it != line_end(); ++it)
      {
        std::cout << *it << "\t";
      }
      std::cout << std::endl;
    }
  };


  // 显示转置矩阵
  void showT()
  {
    int count = 0;
    for (int i = 0; i < width; i++)
    {
      for (auto it = col_begin(at(0, i)); it != col_end(); ++it)
      {
        std::cout << *it << "\t";
        count++;
        if (count % height == 0)
        {
          std::cout << std::endl;
        }
      }
    }
  }


  // 取对应位置值, 重载了()运算符
  double &operator()(int r, int c)
  {
    assert(r < height && c < width);
    return at(r,c).get()->value;
  }


  // 取对应位置shared_ptr
  std::shared_ptr<MatrixNode> at(int r, int c)
  {
    auto curr = head;
    for (int i = 0; i < r; i++)
    {
      curr = curr->down;
    }
    for (int j = 0; j < c; j++)
    {
      curr = curr->right;
    }
    return curr;
  };


  // 设置对应位置值
  void set(int r, int c, double v)
  {
    at(r,c).get()->value = v;
  };
};