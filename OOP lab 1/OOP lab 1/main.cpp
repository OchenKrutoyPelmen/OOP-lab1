#include <iostream>
#include <string>
#include <vector>
using namespace std;

class User {
private:
	string name;
public:

};

class Сourier {
private:
	string name;
	unsigned transport; // 1 - пеший, 2 - вылик, 3 - машина/мотоцикл
public:

};

class Item {
private:
	string name;
	unsigned quantity;
	unsigned price;
public:
	string getN();
	unsigned getQ();
	unsigned getP();
};

string Item::getN() {
	return name;
}
unsigned Item::getQ() {
	return quantity;
}
unsigned Item::getP() {
	return price;
}

class Order {
private:
	unsigned num;
	User user;
	string address;
	vector<Item> items;
	unsigned cost;
	unsigned delmet; // для способа доставки 1/2/3, 1 - стандарт, 2 - экспересс, 3 - самовывоз
	Сourier courier;
	unsigned status; // кол-во часов/дней до прибытия заказа
public:
	void addItem(Item a);
	void getItems();

	unsigned getCost();
};

void Order::addItem(Item a) {
	items.push_back(a);
}

void Order::getItems() {
	cout << "Заказ:" << endl;
	for (unsigned i = 0; i < items.size(); i++)
	{
		cout << items[i].getN() << " | " << items[i].getQ() << " x " << items[i].getP() << " рублей" << endl;
	}
}

unsigned Order::getCost() {

}

void main() {
	setlocale(LC_ALL, "Russian");

	
}