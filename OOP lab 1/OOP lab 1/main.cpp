#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;

class Order;

class User {
private:
	unsigned uid;
	string name;
	unsigned pos; // расстояние до ближайшего пункта выдачи (км)
	map<unsigned, Order*> orders; //map ссылок на заказы этого пользователя
public:
	unsigned getPos();
	void addOrder(Order* a);
};

unsigned User::getPos() {
	return pos;
}

class Courier {
private:
	string name;
	unsigned transport; // 1 - пеший, 2 - вылик, 3 - машина/мотоцикл
	bool busy; // 1 - занят другим заказом, 0 - свободен
public:
	unsigned getT();
	unsigned getB();
	void setB(bool a);
};

unsigned Courier::getT() {
	return transport;
}

unsigned Courier::getB() {
	return busy;
}

void Courier::setB(bool a) {
	busy = a;
}

class Item {
private:
	unsigned iid; // item id
	string name;
	unsigned price;
public:
	string getN();
	unsigned getP();
	unsigned getIID();
};

string Item::getN() {
	return name;
}

unsigned Item::getP() {
	return price;
}

unsigned Item::getIID() {
	return iid;
}

// база данных
vector<User> users = {};
vector<Courier> couriers = {};
map<unsigned, Item> ITEMS = {};

class Order {
private:
	unsigned oid;
	User* user;
	unsigned address; // тоже расстояние до ближайшего пунта выдачи
	map<unsigned, unsigned> items; // первый - item id, второй - его количество.
	unsigned cost;
	unsigned delmet; // для способа доставки 1/2/3, 1 - стандарт, 2 - экспересс, 3 - самовывоз
	Courier* courier;
	unsigned status; // кол-во часов до прибытия заказа
public:
	void addItem(Item i, unsigned n);
	void getItems();
	unsigned getOID();

	bool setCourier(unsigned a); // выбор курьера для доставки
	unsigned getCost();
	unsigned getStatus();

	void Create(unsigned id, User* u, unsigned del);
};

void Order::Create(unsigned id, User* u, unsigned del) {
	oid = id;
	user = u;
	address = user->getPos();
	delmet = del;

	setCourier(delmet);
	getCost();
	getStatus();
}

void Order::addItem(Item i, unsigned n) {
	items[i.getIID()] += n;
}

void Order::getItems() {
	cout << "Заказ:" << endl;
	for (auto& kv : items) {
		cout << ITEMS[kv.first].getN() << " | " << kv.second << " x " << ITEMS[kv.first].getP() << " рублей" << endl;
	}
}

unsigned Order::getOID() {
	return oid;
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
					*courier = couriers[i];
					return true;
				}
			}
		}
		for (unsigned i = 0; i < couriers.size(); i++)
		{
			if (couriers[i].getB() == 0 and couriers[i].getT() == 3) {
				couriers[i].setB(1);
				*courier = couriers[i];
				return true;
			}
		}
		temp = 1;
	}
	cout << "свободных курьеров нет!" << endl;
	return false;
}

unsigned Order::getCost() {
	unsigned temp = 0;
	for (auto& kv : items) {
		temp += (kv.second * ITEMS[kv.first].getP());
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
		if ((*courier).getT() == 1) temp += (*user).getPos() / 5;
		else if ((*courier).getT() == 2) temp += (*user).getPos() / 15;
		else if ((*courier).getT() == 3) temp += (*user).getPos() / 40;
	}

	status = temp;
	return status;
}

void User::addOrder(Order* a) {
	if (a) {
		orders[a->getOID()] = a;
	}
}

int main() {
	setlocale(LC_ALL, "Russian");

	unsigned tempG = 0;
	unsigned temp = 0;
	unsigned OrderID = 0;
	unsigned userID = 0;

	while (1) {
		cout << "1. Сделать заказ" << endl;
		cout << "2. Мои заказы" << endl;
		cout << "3. Выйти" << endl;

		while (tempG != 1 and tempG != 2 and tempG != 3) {
			cin >> tempG;
		}

		switch (tempG)
		{
		case 1:
		{
			Order* myorder = new Order();

			cout << "Каталог:" << endl;
			unsigned tempiid = 0;
			while (1) {
				for (unsigned i = 1; i <= 8; i++)
				{
					if (tempiid == ITEMS.size()) break;
					cout << i << ". " << ITEMS[tempiid].getN() << ": " << ITEMS[tempiid].getP() << " за шт." << endl;
					tempiid++;
				}
				cout << "9. Назад" << endl;
				if (tempiid < ITEMS.size()) cout << "0. Вперед" << endl;
				if (tempiid > 8) tempiid -= 8;
				else tempiid = 0;
				cout << "Выберите товар: "; cin >> temp;
				if (temp == 0 and tempiid < ITEMS.size()) {
					tempiid += 8;
				}
				else if (temp == 1 and tempiid > 7) {
					tempiid -= 8;
				}
				else if (temp == 1) {
					break;
				}
				else if (temp >= 1 and temp <= 8 and temp < tempiid + ITEMS.size() - 1) {
					// выбор количества
					cout << "Выберите количество " << "\"" << ITEMS[tempiid].getN() << "\": "; cin >> temp;
					myorder->addItem(ITEMS[tempiid], temp);
				}
				else {
					cout << "Неверный ввод" << endl;
				}
			}

			cout << "Выберите способ доставки(1 - стандарт, 2 - экспересс, 3 - самовывоз): "; cin >> temp;
			while (temp != 1 and temp != 2 and temp != 3) cin >> temp;
			
			myorder->Create(OrderID, &users[userID], temp);
			users[userID].addOrder(myorder);
			OrderID++;
			break;
		}
		case 2:
			break;
		}
		if (tempG == 3) break;
	}
	
}