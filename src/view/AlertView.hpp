#ifndef VIEW_ALERT_VIEW_HPP
#define VIEW_ALERT_VIEW_HPP

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include "../components/ButtonTiny.hpp"
#include "../components/ViewBase.hpp"

using namespace ftxui;

// Definition of the modal component. The details are not important.
Component ModalComponent(
  std::function<void()> do_nothing,
  std::function<void()> hide_modal
)
{
  auto button_style = ButtonOption::Animated();
  auto component = Container::Vertical({
      ButtonTiny("Do nothing", do_nothing),
      ButtonTiny("Quit modal", hide_modal),
  });
  return ViewBase(component, "Alert");
}

#endif