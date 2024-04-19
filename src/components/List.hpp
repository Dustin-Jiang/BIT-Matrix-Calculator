#ifndef COMPONENTS_LIST_HPP
#define COMPONENTS_LIST_HPP

#include "ftxui/component/component.hpp"
#include <functional>

ftxui::Component List(std::vector<std::string> text) {
  return ftxui::Renderer([text]() {
    ftxui::Elements elements;
    for (auto i : text)
    {
      elements.push_back(ftxui::text(i));
    }
    return ftxui::vbox(std::move(elements)) | ftxui::yframe;
  });
}

ftxui::Component List(std::vector<std::string> *text) {
  return List(*text);
}

#endif