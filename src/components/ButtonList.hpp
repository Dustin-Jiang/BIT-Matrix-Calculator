#ifndef COMPONENTS_BUTTON_LIST_HPP
#define COMPONENTS_BUTTON_LIST_HPP

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include <functional>
#include <string>
#include <vector>

#include "ButtonTiny.hpp"

using namespace ftxui;

Component ButtonList(std::vector<std::string> *buttons, std::function<void(int index)> *on_click) {
  auto component = std::vector<Component>();
  int count = 0;
  for (auto b = (*buttons).begin(); b != (*buttons).end(); ++b)
  {
    component.push_back(ButtonTiny(*b, [count, on_click]() { (*on_click)(count); }));
    count++;
  }
  return Container::Vertical(component);
}

#endif