#include <iostream>
#include <string>


class Animal {
public:
    explicit Animal(std::string name) : name_(std::move(name)) {}
    virtual ~Animal() = default;
    virtual void voice() const = 0;

protected:
    std::string name_;
};


class Dog final : public Animal {
public:
    explicit Dog(std::string name = "Sobaka Defaultovna") : Animal(std::move(name)) {}
    void voice() const override { std::cout << name_ << " barks: Bark\n"; }
};


class Cat final : public Animal {
public:
    explicit Cat(std::string name = "Koshka Defaultovna") : Animal(std::move(name)) {}
    void voice() const override { std::cout << name_ << " meows: Meow\n"; }
};


int main() {

    Dog dog1;
    Dog dog2("Bibu");
    Cat cat1;
    Cat cat2("Bubi");

    Animal* animals[] = { &dog1, &dog2, &cat1, &cat2 };
    for (Animal* a : animals)
        a->voice();

    return EXIT_SUCCESS;
}