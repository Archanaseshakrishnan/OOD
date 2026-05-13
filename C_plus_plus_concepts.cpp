#include <iostream>
#include <string>
#include <memory>
class Sample
{
private:
    int value;
    
    Sample()
    {
        value = 5;
    }
    // Prevent copying - s1 = s2 sort of assignments where member to member copy is made
    Sample(const Sample&) = delete;
    Sample& operator=(const Sample&) = delete;

public:
    void print()
    {
        std::cout << "Hello, World!" << std::endl;
    }

    static Sample& getInstance()
    {
        static Sample instance;
        return instance;
    }
};

class AnotherClass
{
    int a;
    int b;
    std::string c;

    public:
        AnotherClass(int x, int y, std::string z) : a(x), b(y), c(z) {
            std::cout << "AnotherClass constructor called" << std::endl;
        }
        explicit AnotherClass(int x) {
            a = x;
            b = 0;
            c = "";
            std::cout << "AnotherClass constructor called" << std::endl;
        }
        void print()
        {
            std::cout << "AnotherClass: " << a << ", " << b << ", " << c << std::endl;
        }
        void setValues(int x, int y, std::string z)
        {
            a = x;
            b = y;
            c = z;
        }
        ~AnotherClass ()
        {
            std::cout << "AnotherClass destructor called" << std::endl;
        }
};

class B;

class A
{
public:
    std::shared_ptr<B> ptrB;

    ~A()
    {
        std::cout << "A destroyed\n";
        std::cout << "use_count of B ptrs: " << ptrB.use_count() << std::endl;
    }
};

class B
{
public:
    std::weak_ptr<A> ptrA;

    ~B()
    {
        std::cout << "B destroyed\n";
        std::cout << "use_count of A ptrs: " << ptrA.use_count() << std::endl;
    }
};

void fun (AnotherClass obj)
{
    obj.print();
}

void fun2(int x, int y, std::string z)
{
    AnotherClass obj(x, y, z);
    obj.print();
    AnotherClass obj2 = obj;
    obj2.setValues(4, 5, "six");
    obj2.print();
    obj.print();
    AnotherClass &obj3 = obj;
    obj3.setValues(7, 8, "nine");
    obj3.print();
    obj.print();
}

void fun3 ()
{
    std::cout << "fun3: " << std::endl;
    std::unique_ptr<AnotherClass> ptr = std::make_unique<AnotherClass>(1, 2, "three");
    ptr->print();
    AnotherClass * ptr2 = std::move(ptr).release();    
    ptr2->print();
    delete ptr2;
    // ptr->print(); - this will lead to seg fault
}

void fun4 ()
{
    std::cout << "Fun4: " << std::endl;
    std::shared_ptr<AnotherClass> A(new AnotherClass(5));
    std::shared_ptr<AnotherClass> B = A;
}

void fun5 ()
{
    std::cout << "Fun5: " << std::endl;
    std::shared_ptr<A> a = std::make_shared<A>();
    std::shared_ptr<B> b = std::make_shared<B>();

    a->ptrB = b;
    b->ptrA = a;
}

int main()
{
    Sample& s = Sample::getInstance();
    s.print();
    // fun(1); - this is prevented by explicit constructor where implicit AnotherClass(1) and a copy is not allowed
    fun2(1, 2, "three");
    fun3();
    fun4();
    fun5();
}
