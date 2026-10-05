use crate::Ticket;
use std::collections::HashMap;

pub struct ParkingLot {
    capacity: u32,
    booked: u32,
    strength: u32,
}
impl ParkingLot {
    pub fn new(capacity: u32, strength: u32) -> ParkingLot {
        ParkingLot {
            capacity,
            booked: 0,
            strength,
        }
    }
}

#[derive(Copy, Clone, Debug)]
pub enum ParkingAllocator {
    Bike,
    Car,
    Truck,
}
impl ParkingAllocator {
    fn size(&self) -> u32 {
        match self {
            ParkingAllocator::Bike => 1,
            ParkingAllocator::Car => 5,
            ParkingAllocator::Truck => 100,
        }
    }
    fn weight(&self) -> u32 {
        match self {
            ParkingAllocator::Bike => 50,
            ParkingAllocator::Car => 150,
            ParkingAllocator::Truck => 2500,
        }
    }
    pub fn allocate(&self, db: &mut HashMap<u32, ParkingLot>) -> Option<Ticket> {
        for (id, parking_lot) in db {
            if parking_lot.strength >= self.weight()
                && parking_lot.capacity >= self.size() + parking_lot.booked
            {
                parking_lot.booked += self.size();
                return Some(Ticket {
                    id: *id,
                    kind: *self,
                });
            }
        }
        None
    }
    pub fn deallocate(&self, db: &mut HashMap<u32, ParkingLot>, id: u32) -> Result<(), ()> {
        db.get_mut(&id).ok_or(())?.booked -= self.size();
        Ok(())
    }
}
