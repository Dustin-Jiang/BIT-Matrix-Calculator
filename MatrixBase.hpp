#include <memory>
#include <iostream>

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
  MatrixBase(int w, int h) : width(w), height(h)
  {
    int count = 0;
    head = std::make_shared<MatrixNode<T>>(0);

    // 初始化所有值为0
    std::shared_ptr<MatrixNode<T>> curr = head, prev_head = std::make_shared<MatrixNode<T>>(0);
    prev_head.get()->down = head;
    for (int i = 1; i <= h; i++)
    {
      for (int j = 1; j < w; j++)
      {
        count++;
        curr->right = std::make_shared<MatrixNode<T>>(count);
        // 如果上一行有值, 设置上一行节点down指向刚刚创建的节点
        if (prev_head && prev_head->right)
        {
          prev_head->right->down = curr->right;
        }
        curr = curr->right;
      }
      if (i < h)
      {
        count++;
        prev_head->down->down = std::make_shared<MatrixNode<T>>(count);
        prev_head = prev_head->down;
        curr = prev_head->down;
      }
    }
  };

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
    bool operator!=(const LineIterator &i) const
    {
      return head != i.head;
    }
  };
  ;
  LineIterator line_begin() const
  {
    return LineIterator(head);
  };
  LineIterator line_end() const
  {
    return LineIterator(nullptr);
  };

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
};