use std::collections::HashMap;

struct Idle;
struct HasCoin {
    money: u32,
}
struct Dispence {
    name: String,
    change: u32,
}
struct VendingMachine<State = Idle> {
    items: HashMap<String, u32>,
    state: State,
}

impl VendingMachine<Idle> {
    pub fn new(items: HashMap<String, u32>) -> VendingMachine {
        VendingMachine { items, state: Idle }
    }
    pub fn insert(self, money: u32) -> (VendingMachine<HasCoin>, Vec<String>) {
        let allowed = self
            .items
            .iter()
            .filter_map(|(name, price)| {
                if *price <= money {
                    Some(name.into())
                } else {
                    None
                }
            })
            .collect();
        (
            VendingMachine {
                items: self.items,
                state: HasCoin { money },
            },
            allowed,
        )
    }
}
impl VendingMachine<HasCoin> {
    pub fn select(self, name: String) -> Result<VendingMachine<Dispence>, &'static str> {
        let price = *self.items.get(&name).ok_or("Item doesn't exist")?;
        if price > self.state.money {
            return Err("Not enough money to select the product");
        }
        Ok(VendingMachine {
            items: self.items,
            state: Dispence {
                name,
                change: self.state.money - price,
            },
        })
    }
}
impl VendingMachine<Dispence> {
    pub fn dispence(self) -> VendingMachine {
        let Dispence { name, change } = self.state;
        println!("Dispence {name}. Your change: {change}");
        VendingMachine {
            items: self.items,
            state: Idle,
        }
    }
}
impl<T> VendingMachine<T> {
    pub fn show_items(&self) {
        println!("name         price");
        for (name, price) in &self.items {
            println!("{name}  {price}");
        }
    }
}
fn main() {
    let machine = VendingMachine::new(HashMap::from([("water".into(), 1), ("fire".into(), 10)]));
    machine.show_items();
    let (machine, options) = machine.insert(2);
    let machine = machine.select(options.into_iter().nth(0).unwrap()).unwrap();
    let _machine = machine.dispence();
    // _machine.select("fire"); //Compile time error
}
