#ifndef COMPONENTS_TEXT_HPP
#define COMPONENTS_TEXT_HPP

#include "ftxui/component/component.hpp"
#include <functional>

ftxui::Component Text(std::string text) {
  return ftxui::Renderer([text]() { return ftxui::text(text); });
}

ftxui::Component Text(std::string *text) {
  return ftxui::Renderer([text]() { return ftxui::text(*text); });
}

#endif