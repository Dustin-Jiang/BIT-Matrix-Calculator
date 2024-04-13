#include <memory>
#include <iostream>
#include <assert.h>

template <typename T>
class MatrixNode
{
public:
  std::shared_ptr<MatrixNode<T>> right;
  std::shared_ptr<MatrixNode<T>> down;
  T value;
  MatrixNode(T v) : value(v), right(nullptr), down(nullptr) {}
};

template <typename T>
class MatrixBase
{
private:
  std::shared_ptr<MatrixNode<T>> head;

public:
  int width;
  int height;

  // 初始化
  MatrixBase(int w, int h) : width(w), height(h)
  {
    int count = 0;
    head = std::make_shared<MatrixNode<T>>(0);

    // 初始化所有值
    std::shared_ptr<MatrixNode<T>> curr = head;
    // 上一行行首指针
    std::shared_ptr<MatrixNode<T>> prev_line = std::make_shared<MatrixNode<T>>(0);
    // 上一行对应值指针
    std::shared_ptr<MatrixNode<T>> prev_head = nullptr;
    prev_line.get()->down = head;
    for (int i = 1; i <= h; i++)
    {
      for (int j = 1; j < w; j++)
      {
        count++;
        curr->right = std::make_shared<MatrixNode<T>>(count);
        // 如果上一行有值, 设置上一行对应节点down指向刚刚创建的节点
        if (prev_head && prev_head->right)
        {
          prev_head->right->down = curr->right;
          prev_head = prev_head->right;
        }
        curr = curr->right;
      }
      if (i < h)
      {
        count++;
        prev_line->down->down = std::make_shared<MatrixNode<T>>(count);
        prev_line = prev_line->down;
        curr = prev_line->down;
        prev_head = prev_line;
      }
    }
  };

  // 遍历迭代器
  class LineIterator
  {
  private:
    std::shared_ptr<MatrixNode<T>> head;
    std::shared_ptr<MatrixNode<T>> line_head;

  public:
    LineIterator(std::shared_ptr<MatrixNode<T>> h) : head(h)
    {
      line_head = head;
    };
    LineIterator &operator++()
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
    T operator*() const
    {
      return head->value;
    }
    MatrixNode<T>* operator&() const
    {
      return head.get();
    }
    bool operator!=(const LineIterator &i) const
    {
      return head != i.head;
    }
  };
  LineIterator line_begin() const
  {
    return LineIterator(head);
  };
  LineIterator line_end() const
  {
    return LineIterator(nullptr);
  };

  // 转置遍历迭代器
  class ColumnIterator
  {
  private:
    std::shared_ptr<MatrixNode<T>> head;
    std::shared_ptr<MatrixNode<T>> col_head;

  public:
    ColumnIterator(std::shared_ptr<MatrixNode<T>> h) : head(h)
    {
      col_head = head;
    };
    ColumnIterator &operator++()
    {
      if (head->down)
      {
        head = head->down;
      }
      else
      {
        head = col_head->right;
        col_head = head;
      }
      return *this;
    }
    T operator*() const
    {
      return head->value;
    }
    bool operator!=(const ColumnIterator &i) const
    {
      return head != i.head;
    }
  };
  ColumnIterator col_begin() const
  {
    return ColumnIterator(head);
  }
  ColumnIterator col_end() const
  {
    return ColumnIterator(nullptr);
  }

  // 显示矩阵
  void show()
  {
    int count = 0;
    for (auto it = line_begin(); it != line_end(); ++it)
    {
      std::cout << *it << "\t";
      count++;
      if (count % width == 0)
      {
        std::cout << std::endl;
      }
    }
  };

  // 显示转置矩阵
  void showT()
  {
    int count = 0;
    for (auto it = col_begin(); it != col_end(); ++it)
    {
      std::cout << *it << "\t";
      count++;
      if (count % height == 0)
      {
        std::cout << std::endl;
      }
    }
  }

  // 取对应位置值, 重载了()运算符
  T& operator() (int r, int c)
  {
    assert(r < height && c < width);
    return at(r,c).get()->value;
  }

  // 取对应位置shared_ptr
  std::shared_ptr<MatrixNode<T>> at(int r, int c)
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
  void set(int r, int c, T v)
  {
    at(r,c).get()->value = v;
  };
};