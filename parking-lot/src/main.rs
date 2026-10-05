use crate::parking::{ParkingLot, ParkingAllocator};
use std::collections::HashMap;
mod parking;

#[derive(Debug)]
struct Ticket {
    kind: ParkingAllocator,
    id: u32,
}
struct Gate {
    //Map of id -> ParkingLot
    db: HashMap<u32, ParkingLot>,
}
impl Gate {
    pub fn new() -> Gate {
        Gate { db: HashMap::new() }
    }
    //Factory method
    pub fn get_ticket(&mut self, vehicle: ParkingAllocator) -> Option<Ticket> {
        vehicle.allocate(&mut self.db)
    }
    pub fn revoke_ticket(&mut self, ticket: Ticket) -> Result<(), ()> {
        ticket.kind.deallocate(&mut self.db, ticket.id)
    }
    fn add_parking_lot(&mut self, id: u32, parking_lot: ParkingLot) {
        self.db.insert(id, parking_lot);
    }
}
fn main() {
    let mut gate = Gate::new();
    gate.add_parking_lot(1, ParkingLot::new(10, 100));
    gate.add_parking_lot(2, ParkingLot::new(102, 2500));
    let car_ticket = dbg!(gate.get_ticket(ParkingAllocator::Car));
    dbg!(gate.get_ticket(ParkingAllocator::Truck));
    dbg!(gate.get_ticket(ParkingAllocator::Bike));
    if let Some(car_ticket) = car_ticket {
        let _ = dbg!(gate.revoke_ticket(car_ticket));
    }
    dbg!(gate.get_ticket(ParkingAllocator::Truck));
}
