#include <iostream>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <string>
#include <utility>
#include <functional>

using std::vector;
using std::string;
using std::unordered_map;
using std::function;
using std::cout;

/**
 * car score   = distance / fuel + reliability * 2
truck score = distance / fuel - reliability
bike score  = distance / fuel + reliability * 5
 */

vector<vector<string>> rows = { // {type, name, distance, fuel, reliability} // also fuel can be 0
    {"car", "sedan", "120", "40", "3"},
    {"truck", "hauler", "300", "60", "5"},
    {"bike", "roadster", "30", "10", "2"},
    {"bike", "mountain", "25", "15", "3"},
    {"car", "coupe", "150", "50", "4"}
};

class Vehicle {
    protected:
        string model;
        int speed;
        int fuel_capacity;
        int passengers;
    public:
        ~Vehicle() = default;
        Vehicle(const string& model, int speed, int fuel_capacity, int passengers)
            : model(model), speed(speed), fuel_capacity(fuel_capacity), passengers(passengers) {}
        virtual void display_info() const = 0;
};

class Car : public Vehicle {
    public:
        Car(const string& model, int speed, int fuel_capacity, int passengers)
            : Vehicle(model, speed, fuel_capacity, passengers) {}
        void display_info() const override {
            int score = fuel_capacity == 0 ? 1 : (speed * passengers) / fuel_capacity + 3 * 2; // distance is speed * passengers for simplicity 
            std::cout << "Car Model: " << model << ", Speed: " << speed 
                      << " km/h, Fuel Capacity: " << fuel_capacity 
                      << " liters, Passengers: " << passengers 
                      << ", Score: " << score << std::endl;
        }
};

class Truck : public Vehicle {
    public:
        Truck(const string& model, int speed, int fuel_capacity, int passengers)
            : Vehicle(model, speed, fuel_capacity, passengers) {}
        void display_info() const override {
            int score = fuel_capacity == 0 ? 1 : (speed * passengers) / fuel_capacity - 5; // distance is speed * passengers for simplicity
            std::cout << "Truck Model: " << model << ", Speed: " << speed 
                      << " km/h, Fuel Capacity: " << fuel_capacity 
                      << " liters, Passengers: " << passengers 
                      << ", Score: " << score << std::endl;
        }
};

class Bike : public Vehicle {
    public:
        Bike(const string& model, int speed, int fuel_capacity, int passengers)
            : Vehicle(model, speed, fuel_capacity, passengers) {}
        void display_info() const override {
            int score = fuel_capacity == 0 ? 1 : (speed * passengers) / fuel_capacity + 3 * 5; // distance is speed * passengers for simplicity
            std::cout << "Bike Model: " << model << ", Speed: " << speed 
                      << " km/h, Fuel Capacity: " << fuel_capacity 
                      << " liters, Passengers: " << passengers 
                      << ", Score: " << score << std::endl;
        }
};

int main() {
    unordered_map<string, std::function<void(const string&, int, int, int)>> factory = {
        {"car", [](const string& model, int speed, int fuel_capacity, int passengers) {
            Car(model, speed, fuel_capacity, passengers).display_info();
        }},
        {"truck", [](const string& model, int speed, int fuel_capacity, int passengers) {
            Truck(model, speed, fuel_capacity, passengers).display_info();
        }},
        {"bike", [](const string& model, int speed, int fuel_capacity, int passengers) {
            Bike(model, speed, fuel_capacity, passengers).display_info();
        }}
    };

    for (const auto& row : rows) {
        string type = row[0];
        string model = row[1];
        int distance = std::stoi(row[2]);
        int fuel = std::stoi(row[3]);
        int reliability = std::stoi(row[4]);
        
        if (factory.find(type) != factory.end()) {
            factory[type](model, distance, fuel, reliability);
        } else {
            cout << "Unknown vehicle type: " << type << std::endl;
        }
    }
}
