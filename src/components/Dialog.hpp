#ifndef COMPONENT_DIALOG_HPP
#define COMPONENT_DIALOG_HPP

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include <string>
#include <functional>

#include "ButtonTiny.hpp"

using namespace ftxui;

class DialogBase : public ComponentBase
{
private:
  Component component;
public:
  DialogBase(Component c): component(c) {};
  Element Render()
  {
    return hbox(component);
  }
};

struct DialogAction
{
  std::string name;
  std::function<void()> action;
};

Component Dialog(
  const char * title,
  DialogAction comfirm,
  DialogAction cancel
) {
  auto component = Container::Horizontal({
      ButtonTiny(comfirm.name, comfirm.action) | flex,
      ButtonTiny(cancel.name, cancel.action) | flex,
  });
  // Polish how the two buttons are rendered:
  component |= Renderer(
    [&](Element inner)
    {
      return vbox({
          text(title),
          separator(),
          inner,
      }) | size(WIDTH, GREATER_THAN, 30) | border;
    }
  );
  return Make<DialogBase>(component);
}

Component DialogSingle(
  std::string title,
  DialogAction button
) {
  auto component = Container::Horizontal({
      ButtonTiny(button.name, button.action)
  });
  // Polish how the two buttons are rendered:
  component |= Renderer([&](Element inner) {
    return vbox({
               text(title),
               separator(),
               inner,
           }) |
           size(WIDTH, GREATER_THAN, 30) | border;
  });
  return Make<DialogBase>(component);
}

#endif