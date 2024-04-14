#include <iostream>
#include "../src/Matrix.hpp"

int main()
{
  Matrix m{4, 4};
  std::cout << m.at(2, 2) << std::endl;
  std::cout << std::endl;

  m.showT();
  std::cout << std::endl;

  auto r1 = m * 2.8;
  r1.show();
  std::cout << std::endl;

  Matrix t{4, 4};
  auto r2 = m + t;
  r2.show();
  std::cout << std::endl;

  auto r3 = r1 - m;
  r3.show();
  std::cout << std::endl;

  auto r4 = Matrix{new std::vector<std::vector<double>>{
      {1, 2, 3, 4}}};
  r4.show();
  std::cout << std::endl;

  (r4 * m).show();
  std::cout << std::endl;

  auto r5 = Matrix{new std::vector<std::vector<double>>{
      {1,0,1,1},
      {3,0,0,1},
      {0,0,1,4},
      {0,0,0,0}}};

  r5.swap_line(0,1);
  r5.show();
  std::cout << std::endl;

  assert(r5.determinant() == 0);

  r5.row_reduce().show();
  std::cout << std::endl;
  r5.show();
  std::cout << std::endl;

  r5.remove_col(2);
  r5.show();
  std::cout << std::endl;

  auto r6 = Matrix{new std::vector<std::vector<double>>{
    {1,87,1,30},
    {41,5,5,8},
    {45,7,1,2},
    {48,3,16,48}
  }};
  assert(r6.determinant() == 370840);

  return 0;
}