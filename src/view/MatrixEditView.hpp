#ifndef VIEW_ALERT_VIEW_HPP
#define VIEW_ALERT_VIEW_HPP

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include "../components/ButtonTiny.hpp"
#include "../components/ViewBase.hpp"
#include "../components/Text.hpp"
#include "../model/MatrixList.hpp"

using namespace ftxui;

// Definition of the modal component. The details are not important.
Component MatrixEditView(
  MatrixList *matrix_list,
  std::function<void()> hide_modal,
  std::function<void(std::vector<std::vector<double>> *v)> update_matrix
)
{
  // MatrixEditView
  int height = 0, width = 0;
  std::string height_v = "", width_v = "";

  std::vector<std::vector<double>> m = {};

  auto set_size = [&] {
    height = std::stoi(height_v);
    width = std::stoi(width_v);
    for (int i = 0; i < height; i++) {
      std::vector<double> row{};
      for (int j = 0; j < width; j++) {
        row.push_back(0.0);
      }
      m.push_back(row);
    }
  };

  auto component = Container::Vertical({
    Container::Vertical({
      Text("定义矩阵尺寸"),
      Container::Horizontal({
        Input(width_v, { .placeholder = "宽度", .multiline = false }) | underlined | flex | border,
        Input(height_v, { .placeholder = "高度", .multiline = false }) | underlined | flex | border,
        ButtonTiny("确定", set_size) | flex | border,
      }),
    }),
    ButtonTiny("添加矩阵", [&] { update_matrix(&m); }),
    ButtonTiny("返回", hide_modal),
  });

  return ViewBase(component, "编辑矩阵");
}

#endif