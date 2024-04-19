#ifndef COMPONENTS_MATRIX_EDITOR_HPP
#define COMPONENTS_MATRIX_EDITOR_HPP

#include "ftxui-grid-container/grid-container.hpp"
#include "ftxui/component/component.hpp"
#include <memory>

using namespace ftxui;

Component MatrixEditor(
  std::vector<std::vector<std::shared_ptr<std::string>>> *ctx
)
{
  std::vector<Components> component;

  for (auto i: *ctx)
  {
    Components row;
    for (auto j: i)
    {
      row.push_back(Input(j.get(), { .multiline = false }) | flex | border);
    }
    component.push_back(row);
  }

  return GridContainer(component);
}

#endif