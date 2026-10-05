#include <cassert>
#include <cstring>
#include <functional>
#include <memory>
#include <utility>
#include <vector>
using namespace std;

class UIComponent {
    UIComponent *parent;
    char *chars;
    vector<unique_ptr<UIComponent>> children;
    vector<function<void(UIComponent &)>> on_distroy;

    void distroy() noexcept {
        if (chars) {
            delete[] chars;
        }
        for (auto cb : on_distroy) {
            cb(*this);
        }
    }
    void copy(UIComponent &&other) noexcept {
        parent = other.parent;
        chars = other.chars;
        children = std::move(other.children);
        for (auto &child : children) {
            child->parent = this;
        }
        on_distroy = std::move(other.on_distroy);
        other.chars = nullptr;
        other.parent = nullptr;
    }

  public:
    UIComponent(const char *data) : parent(nullptr) {
        if (!data) {
            throw "matherchood";
        }
        chars = new char[strlen(data) + 1];
        strcpy(chars, data);
    }
    UIComponent(const UIComponent &other) = delete;
    UIComponent &operator=(const UIComponent &other) = delete;
    UIComponent(UIComponent &&other) noexcept { copy(std::move(other)); }
    UIComponent &operator=(UIComponent &&other) noexcept {
        distroy();
        copy(std::move(other));
        return *this;
    }
    const vector<unique_ptr<UIComponent>> &get_children() { return children; }
    void add(UIComponent &&comp) {
        comp.parent = this;
        children.push_back(make_unique<UIComponent>(std::move(comp)));
    }
    void add_distroy_callback(function<void(UIComponent &)> cb) {
        on_distroy.push_back(cb);
    }
    ~UIComponent() { distroy(); }
};
namespace FocusManager {
UIComponent *focused_component = nullptr;
//Moving a focused component is UB
static void focus(UIComponent *comp) {
    focused_component = comp;
    if (comp) {
        comp->add_distroy_callback([](UIComponent &dying_compt) {
            if (focused_component == &dying_compt) {
                focused_component = nullptr;
            }
        });
    }
}
}; // namespace FocusManager

int main() {
    UIComponent comp("water");
    comp.add(UIComponent("fire"));
    FocusManager::focus(&comp);
    return 0;
}
