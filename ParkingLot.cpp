#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <unordered_map>
#include <queue>
#include <cassert>
#include <ctime>
#include <cmath>

#define VEHICLE_TYPES 3
#define MAX_PARKING_SPACES 100

/**
 * Intent: To provide a simple parking lot management system that can handle different types of vehicles and their parking fees.
 * Current version is not multi-threaded, nor is the accomodation of range queries implemented 9to support bigger vehicles by concatenating contiguous smaller spaces.
 */


std::unordered_map<int, float> rate_per_hour_with_type = {
    {0, 2.0}, // Regular
    {1, 3.0}, // Premium
    {2, 5.0}  // VIP
};

class Ticket {
    public:
        Ticket(int carId, int parkingSpace, int timestamp) : carId(carId), parkingSpace(parkingSpace), timestamp_at_parking(timestamp) {}

        double calculateParkingFee(int car_type) {
            int now = time(0);
            int duration = now - timestamp_at_parking;

            double hoursParked = duration / 3600.0;

            double fee =
                hoursParked * rate_per_hour_with_type[car_type];

            return std::round(fee * 100.0) / 100.0;
        }

        int getParkingSpace() {
            return parkingSpace;
        }

    private:
        int carId; // Unique identifier for the car
        int parkingSpace; // The parking space ID allocated to the car
        int timestamp_at_parking; // Time when the car was parked
};

class ParkingLot {
public:
    ParkingLot() {
        // Initialize the parking lot with n spaces
        this->num_occupied = 0;
        unoccupiedSpaces.resize(VEHICLE_TYPES); // Initialize the vector with 3 elements (for each car type)
        for (int i = 0; i < VEHICLE_TYPES; i++) {
            unoccupiedSpaces[i] = std::priority_queue<int, std::vector<int>, std::greater<int>>();
        }
        for (int i = 0; i < VEHICLE_TYPES; i++) {   
            for (int j = 0; j < MAX_PARKING_SPACES; j++) {
                unoccupiedSpaces[i].push((i * MAX_PARKING_SPACES) + j);
            }
        }
    }

    static ParkingLot& GetInstance() {
        static ParkingLot instance;
        return instance;
    }
    
    int Park (int car, int car_type) {
        if (car_type < 0 || car_type >= VEHICLE_TYPES) {
            return -1; // Invalid car type
        }

        // TODO: need to convert this to mutex based locking or other synchronization mechanisms
        if (num_occupied >= MAX_PARKING_SPACES * VEHICLE_TYPES) {
            return -1; // No available spaces
        }
        if (car_id_to_ticket.find(car) != car_id_to_ticket.end()) {
            return -1; // Car is already parked
        }
        
        int top_unoccupied = -1;
        if (!unoccupiedSpaces[car_type].empty()) {
            top_unoccupied = unoccupiedSpaces[car_type].top();
            unoccupiedSpaces[car_type].pop();
            auto ticket = std::make_unique<Ticket>(car, top_unoccupied, time(0));
            car_id_to_ticket[car] = std::make_pair(car_type, std::move(ticket));
            this->num_occupied++;
        }

        return top_unoccupied;
    }

    float Unpark(int car) {
        // TODO: need to convert this to mutex based locking or other synchronization mechanisms
        auto it = car_id_to_ticket.find(car);
        if (it == car_id_to_ticket.end()) {
            return -1; // Car is not parked
        }

        int car_type = it->second.first;
        Ticket *ticket = it->second.second.get();
        float fee = ticket->calculateParkingFee(car_type);
        int parkspaceID = ticket->getParkingSpace();
        assert (parkspaceID >= 0 && parkspaceID < MAX_PARKING_SPACES * VEHICLE_TYPES);

        #ifdef DEBUG
            std::priority_queue<int, std::vector<int>, std::greater<int>> temp = unoccupiedSpaces[car_type];
            bool found = false;
            while (!temp.empty() && !found && temp.top() <= parkspaceID) {
                if (temp.top() == parkspaceID) {
                    found = true;
                    break;
                }
                temp.pop();
            }
            assert(!found);
        #endif

        // Return the parking space to the heap
        unoccupiedSpaces[car_type].push(parkspaceID);
        car_id_to_ticket.erase(it);
        this->num_occupied--;

        return fee; // Success
    }

    private:
        std::vector<std::priority_queue<int, std::vector<int>, std::greater<int>>> unoccupiedSpaces; 
        /**
         * unoccupiedSpaces[index] refers to the heap of unoccupied spaces that accomodates cars of a specific type.
         * 
         */
        std::unordered_map<int, std::pair<int, std::unique_ptr<Ticket>>> car_id_to_ticket; // license plate to type and ticket mapping
        /** 
         * Instead of Car class we can use Car std::hash(license plate) as the key, with type as key.
         */
        int num_occupied; // Number of occupied parking spaces
};

int main() {
    ParkingLot &lot = ParkingLot::GetInstance();
    std::cout << "Parking lot initialized with 10 spaces\n";

    struct Request { int car; int type; };
    Request parkRequests[] = {
        {101, 0},
        {102, 1},
        {103, 2},
        {104, 0},
        {105, 1}
    };

    for (const auto &req : parkRequests) {
        int space = lot.Park(req.car, req.type);
        if (space >= 0) {
            std::cout << "Car " << req.car << " parked in space " << space << " (type " << req.type << ")\n";
        } else {
            std::cout << "Failed to park car " << req.car << " (type " << req.type << ")\n";
        }
    }

    int unparkCars[] = {102, 104, 999};
    for (int car : unparkCars) {
        float fee = lot.Unpark(car);
        if (fee >= 0) {
            std::cout << "Car " << car << " unparked, fee = $" << fee << "\n";
        } else {
            std::cout << "Car " << car << " is not parked or could not be unparked\n";
        }
    }

    return 0;
}
