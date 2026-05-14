#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using std::chrono::system_clock;
using std::chrono::duration;

class BigArray {
public:
    BigArray(size_t len)
        : len_(len), data_(new int[len]) {
        cout << "Constructor of " << len_ << " elements\n";
    }

    // Copy constructor
    BigArray(const BigArray& other)
        : len_(other.len_), data_(new int[other.len_]) {
        cout << "Copy construction of " << other.len_ << " elements\n";
        std::copy(other.data_, other.data_ + len_, data_);
    }

    // Copy assignment
    BigArray& operator=(const BigArray& other) {
        cout << "Copy assignment of " << other.len_ << " elements\n";

        if (this != &other) {
            delete[] data_;

            len_ = other.len_;
            data_ = new int[len_];
            std::copy(other.data_, other.data_ + len_, data_);
        }

        return *this;
    }

    // Move constructor
    BigArray(BigArray&& other) noexcept
        : len_(other.len_), data_(other.data_) {
        cout << "Move construction of " << other.len_ << " elements\n";

        other.len_ = 0;
        other.data_ = nullptr;
    }

    // Move assignment
    BigArray& operator=(BigArray&& other) noexcept {
        cout << "Move assignment of " << other.len_ << " elements\n";

        if (this != &other) {
            delete[] data_;

            len_ = other.len_;
            data_ = other.data_;

            other.len_ = 0;
            other.data_ = nullptr;
        }

        return *this;
    }

    ~BigArray() {
        delete[] data_;
    }

private:
    size_t len_ = 0;
    int* data_ = nullptr;
};

int main() {
    vector<BigArray> myVec;

    auto begin = system_clock::now();
    BigArray bigArray(1000000000);
    auto end = system_clock::now() - begin;
    auto timeInSeconds = duration<double>(end).count();
    cout << "\ntime in seconds: " << timeInSeconds << endl;
    begin = system_clock::now();
    myVec.push_back(bigArray); // copy constructor
    end = system_clock::now() - begin;
    timeInSeconds = duration<double>(end).count();
    cout << "\ntime in seconds: " << timeInSeconds << endl;
    
    BigArray *p1 = new BigArray(1000000000);
    begin = system_clock::now();
    myVec.push_back(std::move(*p1)); 
    end = system_clock::now() - begin;
    timeInSeconds = duration<double>(end).count();
    cout << "\ntime in seconds: " << timeInSeconds << endl;
    begin = system_clock::now();
    myVec.push_back(BigArray(1000000000));
    end = system_clock::now() - begin;
    timeInSeconds = duration<double>(end).count();
    cout << "\ntime in seconds: " << timeInSeconds << endl;

}
