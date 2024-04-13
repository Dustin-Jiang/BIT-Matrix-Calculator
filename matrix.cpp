#include <iostream>
#include "matrix.hpp"

int main()
{
  Matrix m {4,4};
  std::cout << m.at(2,2) << std::endl;
  auto r1 = m * 2.8;
  r1.show();

  Matrix t {4,4};
  auto r2 = m + t;
  r2.show();

  auto r3 = r1 - m;
  r3.show();

  return 0;
}