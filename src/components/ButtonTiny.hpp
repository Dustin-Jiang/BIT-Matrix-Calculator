#ifndef COMPONENTS_BUTTON_TINY
#define COMPONENTS_BUTTON_TINY

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include <string>
#include <functional>

using namespace ftxui;

Component ButtonTiny(
  ConstStringRef label,
  std::function<void()> on_click
) {
  ButtonOption option = ButtonOption::Ascii();
  option.label = label;
  option.on_click = std::move(on_click);
  return Button(std::move(option));
}

#endif