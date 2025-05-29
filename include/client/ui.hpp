#ifndef UI_HPP
#define UI_HPP

#include "views.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"  // for Ref

#include <memory>
#include <vector>
#include <string>
// #include <functional>



using namespace ftxui;

class UIBase {
public:
    virtual ~UIBase() = default;
    virtual Component GetComponent() = 0;
    virtual Element Render() = 0;
    virtual void Update() {};
};


class SideBarPanel : public UIBase {
private:
    std::vector<std::string> menu_items_;
    int selected_menu_ = 0;
    Component menu_component_;

public:
    SideBarPanel(const std::vector<std::string>& items);
    Component GetComponent() override;
    Element Render() override;
    int getSelectedItem() const;
};


class ContentPanel : public UIBase {
private:
    int selected_content_ = 0;
    Component content_component_;

public:
    ContentPanel();
    Component GetComponent() override;
    Element Render() override;
    void setSelectedContent(int index);
    int getSelectedContent() const;
};

class Home : public UIBase {
private:
    int selected_content_ = 0;
    Component content_component_;
    Component input_name_;
    Component input_category_;
    Component toggle_type_;
    Component input_amount_;

    std::vector<std::string> toggle_entries;

    std::string name;
    std::string category;
    int toggle_selected;
    std::string amount;

public:
    Home();
    Component GetComponent() override;
    Element Render() override;
    void setSelectedContent(int index);
    int getSelectedContent() const;
};


#endif 