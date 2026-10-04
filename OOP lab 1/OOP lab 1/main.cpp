#include <iostream>
#include <string>
#include <vector>
using namespace std;

class User {
private:
	string name;
	unsigned pos; // расстояние до ближайшего пункта выдачи (км)
	// сюда нужно добаввить масси ссылок на заказы этого пользователя
public:
	unsigned getPos();
};

unsigned User::getPos() {
	return pos;
}

class Сourier {
private:
	string name;
	unsigned transport; // 1 - пеший, 2 - вылик, 3 - машина/мотоцикл
	bool busy; // 1 - занят другим заказом, 0 - свободен
public:
	unsigned getT();
	unsigned getB();
	void setB(bool a);
};

unsigned Сourier::getT() {
	return transport;
}

unsigned Сourier::getB() {
	return busy;
}

void Сourier::setB(bool a) {
	busy = a;
}

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

// база данных
vector<User> users = {};
vector<Сourier> couriers = {};
vector<Item> items = {};

class Order {
private:
	unsigned num;
	User user;
	string address;
	vector<Item> items;
	unsigned cost;
	unsigned delmet; // для способа доставки 1/2/3, 1 - стандарт, 2 - экспересс, 3 - самовывоз
	Сourier courier;
	unsigned status; // кол-во часов до прибытия заказа
public:
	void addItem(Item a);
	void getItems();

	bool setCourier(unsigned a); // выбор курьера для доставки
	unsigned getCost();
	unsigned getStatus();
};

void Order::addItem(Item a) {
	items.push_back(a);
}

bool Order::setCourier(unsigned a) {
	unsigned temp = a;
	for (unsigned j = 0; j < 2; j++)
	{
		if (temp == 1) {
			for (unsigned i = 0; i < couriers.size(); i++)
			{
				if (couriers[i].getB() == 0 and couriers[i].getT() < 3) {
					couriers[i].setB(1);
					courier = couriers[i];
					return true;
				}
			}
		}
		for (unsigned i = 0; i < couriers.size(); i++)
		{
			if (couriers[i].getB() == 0 and couriers[i].getT() == 3) {
				couriers[i].setB(1);
				courier = couriers[i];
				return true;
			}
		}
		temp = 1;
	}
	cout << "свободных курьеров нет!" << endl;
	return false;
}

void Order::getItems() {
	cout << "Заказ:" << endl;
	for (unsigned i = 0; i < items.size(); i++)
	{
		cout << items[i].getN() << " | " << items[i].getQ() << " x " << items[i].getP() << " рублей" << endl;
	}
}

unsigned Order::getCost() {
	unsigned temp = 0;
	for (unsigned i = 0; i < items.size(); i++)
	{
		temp += (items[i].getP() * items[i].getQ());
	}
	
	switch (delmet)
	{
	case 1: // обычная доставка
		temp += 300;
		break;
	case 2: // экспресс доставка
		temp += 450;
		break;
	default: // самовывоз
		break;
	}

	cost = temp;
	return cost;
}

unsigned Order::getStatus() {
	unsigned temp = 0;
	unsigned delivery = 0;
	temp += 24; // стандартные 24 часа на доставку в пункт выдачи 

	switch (delmet)
	{
	case 1: // обычная доставка
		if (setCourier(1)) delivery = 1;
		delivery = 1;
		break;
	case 2: // экспресс доставка
		if (setCourier(2)) delivery = 1;
		break;
	default: // самовывоз
		break;
	}

	if (delivery == 1) { // расчет времени на доставку от пунта выдачи до дома
		if (courier.getT() == 1) temp += user.getPos() / 5;
		else if (courier.getT() == 2) temp += user.getPos() / 15;
		else if (courier.getT() == 3) temp += user.getPos() / 40;
	}

	status = temp;
	return status;
}

int main() {
	setlocale(LC_ALL, "Russian");

	
}