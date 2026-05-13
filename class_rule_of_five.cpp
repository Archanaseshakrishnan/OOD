#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include <utility>
#include <iterator>

class rule_of_five
{
    char* cstring; // raw pointer used as a handle to a
                   // dynamically-allocated memory block
public:
    explicit rule_of_five(const char* s = "") : cstring(nullptr)
    { 
        if (s)
        {
            cstring = new char[std::strlen(s) + 1]; // allocate
            std::strcpy(cstring, s); // populate 
        } 
    }
 
    ~rule_of_five()
    {
        delete[] cstring; // deallocate
    }
 
    rule_of_five(const rule_of_five& other) // copy constructor
        : rule_of_five(other.cstring) {}
 
    rule_of_five(rule_of_five&& other) noexcept // move constructor
        : cstring(std::exchange(other.cstring, nullptr)) {}
 
    rule_of_five& operator=(const rule_of_five& other) // copy assignment
    {
        // implemented as move-assignment from a temporary copy for brevity
        // note that this prevents potential storage reuse
        return *this = rule_of_five(other);
    }
 
    rule_of_five& operator=(rule_of_five&& other) noexcept // move assignment
    {
        std::swap(cstring, other.cstring);
        return *this;
    }

    const char* c_str() const noexcept
    {
        return cstring ? cstring : "";
    }

    bool empty() const noexcept
    {
        return cstring == nullptr || cstring[0] == '\0';
    }

    void set_string (const char* s)
    {
        delete[] cstring;
        cstring = nullptr;
        if (s)
        {
            cstring = new char[std::strlen(s) + 1];
            std::strcpy(cstring, s);
        }
    }

// alternatively, replace both assignment operators with copy-and-swap
// implementation, which also fails to reuse storage in copy-assignment.
//  rule_of_five& operator=(rule_of_five other) noexcept
//  {
//      std::swap(cstring, other.cstring);
//      return *this;
//  }
};

static void print_state(const rule_of_five& obj, const std::string& name)
{
    std::cout << name << ": " << (obj.empty() ? "[empty]" : obj.c_str()) << '\n';
}

int main()
{
    rule_of_five a("Hello");
    print_state(a, "a");

    rule_of_five b = a; // copy constructor
    print_state(b, "b (copy of a)");
    print_state(a, "a after copy construction");

    b.set_string("World");
    print_state(b, "b after setting string");
    print_state(a, "a after setting b's string");

    rule_of_five c(std::move(a)); // move constructor
    print_state(c, "c (moved from a)");
    print_state(a, "a after move construction");

    rule_of_five d;
    d = b; // copy assignment
    print_state(d, "d (copy assigned from b)");
    print_state(b, "b after copy assignment");

    rule_of_five e;
    e = std::move(b); // move assignment
    print_state(e, "e (move assigned from b)");
    print_state(b, "b after move assignment");

    return 0;
}
