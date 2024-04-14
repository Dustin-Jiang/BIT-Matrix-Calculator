#include <iostream>
#include "Matrix.hpp"

int main()
{
  // Matrix m{4, 4};
  // std::cout << m.at(2, 2) << std::endl;
  // std::cout << std::endl;

  // m.showT();
  // std::cout << std::endl;

  // auto r1 = m * 2.8;
  // r1.show();
  // std::cout << std::endl;

  // Matrix t{4, 4};
  // auto r2 = m + t;
  // r2.show();
  // std::cout << std::endl;

  // auto r3 = r1 - m;
  // r3.show();
  // std::cout << std::endl;

  // auto r4 = Matrix{new std::vector<std::vector<double>>{
  //     {1, 2, 3, 4}}};
  // r4.show();
  // std::cout << std::endl;

  // (r4 * m).show();
  // std::cout << std::endl;

  auto r5 = Matrix{new std::vector<std::vector<double>>{
      {1, 2, 3, 4},
      {5, 6, 7, 8},
      {9, 10, 11, 12},
      {13, 14, 15, 16}}};
  r5.swap_line(0,3);
  r5.show();
  std::cout << std::endl;

  r5.row_reduce().show();
  std::cout << std::endl;

  return 0;
}