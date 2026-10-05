#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <sys/select.h>
#include <unordered_map>
#include <vector>
using namespace std;
class Machine {
    class Engine {
      public:
        virtual ~Engine() = default;
        virtual vector<string> insert_coin(Machine &machine, int val) {
            throw runtime_error("Coin already inserted.");
        }
        virtual void select(Machine &machine, string name) {
            throw runtime_error("Machine not in select state");
        }
        virtual void dispense(Machine &machine) {
            throw runtime_error("No item selected");
        }
    };
    class DispenceEngine : public Engine {
        int change;
        string name;

      public:
        DispenceEngine(string name, int change) : name(name), change(change) {}
        void dispense(Machine &machine) override {
            cout << "Dispenced " << name << ". Your change: " << change << '\n';
            machine.engine = make_unique<IdleEngine>();
        }
    };
    class HasCoinEngine : public Engine {
        int money;

      public:
        HasCoinEngine(int money) : money(money) {}
        void select(Machine &machine, string name) override {
            auto it = machine.items.find(name);
            if (it == machine.items.end()) {
                throw runtime_error("Item doesn't exist");
            }
            if (it->second > money) {
                throw runtime_error("Not enough money to select the product");
            }
            machine.engine =
                make_unique<DispenceEngine>(name, money - it->second);
        }
    };
    class IdleEngine : public Engine {
      public:
        vector<string> insert_coin(Machine &machine, int money) override {
            vector<string> allowed;
            for (auto [name, price] : machine.items) {
                if (price <= money) {
                    allowed.push_back(name);
                }
            }
            machine.engine = make_unique<HasCoinEngine>(money);
            return allowed;
        }
    };

    unique_ptr<Engine> engine;
    unordered_map<string, int> items; // Map to item name -> price
  public:
    Machine() : engine(make_unique<IdleEngine>()) {}
    vector<string> insert_coin(int val) {
        return engine->insert_coin(*this, val);
    }
    void select(string name) { engine->select(*this, name); }
    void dispense() { engine->dispense(*this); }
    void update(string name, int price) { items[name] = price; }
};
int main() {
    Machine machine;
    machine.update("water", 1);
    machine.update("fire", 10);
    auto options = machine.insert_coin(2);
    machine.select(options[0]);
    machine.dispense();
    machine.select("fire");
}
