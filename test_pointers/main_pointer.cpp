#include <iostream>
#include <string>

using namespace std;

int add(int* a, int* b) {
	add(a, b);
	return *a + *b;
}

int subtract(int* a, int* b) {
	subtract(a, b);
	return *a - *b;
}

struct Human
{
	Car car;
	string name;
	int age;
};

struct Car
{
	string name;
	int speed;
};

int main()
{
	int*a;
	int b = 5;
	a = &b;

	string str = "Hello, World!";

	Human person1 = { "Alice", 30 };
	Human person2 = { "Alice", 30 };
	Car car1 = { "BMW", 200 };
	Cat cat1 = {};

	cat1.meow();

	str.length();

	add(a, &b);


	cout << *a;
}

Human* createHuman(string name, int age) {
	Human* newHuman = new Human;
	newHuman->name = name;
	newHuman->age = age;
	return newHuman;
}



class Animal
{
	string name;
	int age;
};

class Cat : public Animal
{
	string nameOfMeow;

public:
	void meow() {
		cout << "Meow!";
	}
};

