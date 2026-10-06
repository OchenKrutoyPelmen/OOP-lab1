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
	User(unsigned a, string b, unsigned c);
	~User();

	unsigned getPos();
	string getName();
	void addOrder(Order* a);
	void getOrders();
	bool isOrdersEmpty();
};

User::User(unsigned a, string b, unsigned c) {
	uid = a;
	name = b;
	pos = c;
}

User::~User() {
	for (auto& kv : orders) {
		delete kv.second;
	}
	orders.clear();
}

bool User::isOrdersEmpty() {
	return orders.empty();
}

unsigned User::getPos() {
	return pos;
}

string User::getName() {
	return name;
}

class Courier {
private:
	unsigned cid;
	string name;
	unsigned transport; // 1 - пеший, 2 - вылик, 3 - машина/мотоцикл
	bool busy; // 1 - занят другим заказом, 0 - свободен
public:
	Courier(unsigned a, string b, unsigned c);

	unsigned getT();
	unsigned getB();
	string getName();
	void setB(bool a);
};

Courier::Courier(unsigned a, string b, unsigned c) {
	cid = a;
	name = b;
	transport = c;
	busy = 0;
}

string Courier::getName() {
	return name;
}

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
	Item(unsigned a, string b, unsigned c);

	string getN();
	unsigned getP();
	unsigned getIID();
};

Item::Item(unsigned a, string b, unsigned c) {
	iid = a;
	name = b;
	price = c;
}

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
vector<User> users = { User(0, "Свет", 150), User(1, "Миша", 10), User(2, "Макс", 1000) };
vector<Courier> couriers = { Courier(0, "Арсений", 1), Courier(2, "Артем", 2), Courier(3, "Ярик", 3) };
map<unsigned, Item> ITEMS = {
	{0,  Item(0,  "Яблоки",          120)},
	{1,  Item(1,  "Бананы",           90)},
	{2,  Item(2,  "Апельсины",       150)},
	{3,  Item(3,  "Виноград",        250)},
	{4,  Item(4,  "Клубника",        380)},
	{5,  Item(5,  "Помидоры",        180)},
	{6,  Item(6,  "Огурцы",          130)},
	{7,  Item(7,  "Картофель",        60)},
	{8,  Item(8,  "Морковь",          55)},
	{9,  Item(9,  "Лук репчатый",     45)},
	{10, Item(10, "Капуста",          70)},
	{11, Item(11, "Молоко 3.2%",     95)},
	{12, Item(12, "Кефир 1%",        88)},
	{13, Item(13, "Сметана 20%",     140)},
	{14, Item(14, "Творог 5%",       170)},
	{15, Item(15, "Сыр Российский",  480)},
	{16, Item(16, "Масло сливочное", 220)},
	{17, Item(17, "Хлеб белый",       45)},
	{18, Item(18, "Хлеб чёрный",      50)},
	{19, Item(19, "Батон нарезной",   55)},
	{20, Item(20, "Яйца куриные 10шт",130)},
	{21, Item(21, "Курица (филе)",   340)},
	{22, Item(22, "Говядина",        620)},
	{23, Item(23, "Свинина",         450)},
	{24, Item(24, "Рыба (минтай)",   290)}
};

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
	bool isItemsClear();

	bool setCourier(unsigned a); // выбор курьера для доставки
	unsigned getCost();
	unsigned getStatus();
	User* getUser();
	Courier* getCourier();
	unsigned getDelmet();

	void Create(unsigned id, User* u, unsigned del);
};

unsigned Order::getDelmet() {
	return delmet;
}

User* Order::getUser() {
	return user;
}
Courier* Order::getCourier() {
	return courier;
}

bool Order::isItemsClear() {
	return items.empty();
}

void Order::Create(unsigned id, User* u, unsigned del) {
	oid = id;
	user = u;
	address = user->getPos();
	delmet = del;
	courier = nullptr;

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
		cout << ITEMS.at(kv.first).getN() << " | " << kv.second << " x " << ITEMS.at(kv.first).getP() << " рублей" << endl;
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
					courier = &couriers[i];
					return true;
				}
			}
		}
		for (unsigned i = 0; i < couriers.size(); i++)
		{
			if (couriers[i].getB() == 0 and couriers[i].getT() == 3) {
				couriers[i].setB(1);
				courier = &couriers[i];
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
		temp += (kv.second * ITEMS.at(kv.first).getP());
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

	if ((delmet == 1 or delmet == 2) and courier != nullptr) { // расчет времени на доставку от пунта выдачи до дома
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

void User::getOrders() {
	for (auto& kv : orders) {
		cout << "Заказ номер " << kv.first << ":" << endl;
		cout << "Заказчик: " << kv.second->getUser()->getName() << endl;
		if (kv.second->getDelmet() != 3) cout << "Курьер: " << kv.second->getCourier()->getName() << " ";
		cout << "(";
		if (kv.second->getDelmet() == 1) cout << "Стандартная доставка";
		else if (kv.second->getDelmet() == 2) cout << "Экспресс доставка";
		else if (kv.second->getDelmet() == 3) cout << "Самовывоз";
		cout << ")" << endl;
		kv.second->getItems();
		cout << "Итого: " << kv.second->getCost() << " рублей" << endl;
		cout << "Примерное время ожидания: " << kv.second->getStatus() << " часов" << endl << endl;
	}
}

int main() {
	setlocale(LC_ALL, "Russian");

	unsigned tempG = 0;
	unsigned temp = 0;
	unsigned quantity = 0;
	unsigned OrderID = 0;
	unsigned userID = 0;

	while (1) {
		cout << "1. Сделать заказ" << endl;
		cout << "2. Мои заказы" << endl;
		cout << "3. Выйти" << endl;

		tempG = 0;
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
					cout << i << ". " << ITEMS.at(tempiid).getN() << ": " << ITEMS.at(tempiid).getP() << " за шт." << endl;
					tempiid++;
				}
				if (tempiid > 8) cout << "9. Назад" << endl;
				else cout << "9. Завершить" << endl;
				if (tempiid < ITEMS.size()) cout << "0. Вперед" << endl;
				if (tempiid > 8) {
					if (tempiid % 8 == 0) tempiid -= 8;
					else tempiid -= tempiid % 8;
				}
				else tempiid = 0;
				cout << "Выберите товар: "; cin >> temp;
				if (temp == 0 and tempiid < ITEMS.size()) {
					tempiid += 8;
				}
				else if (temp == 9 and tempiid > 7) {
					tempiid -= 8;
				}
				else if (temp == 9) {
					break;
				}
				else if (temp >= 1 and temp <= 8 and temp < tempiid + ITEMS.size() - 1) {
					cout << "Выберите количество " << "\"" << ITEMS.at(tempiid + temp - 1).getN() << "\": "; cin >> quantity;
					if (quantity != 0) myorder->addItem(ITEMS.at(tempiid + temp - 1), quantity);
				}
				else {
					cout << "Неверный ввод" << endl;
				}
			}

			if (myorder->isItemsClear()) {
				delete myorder;
				break;
			}

			cout << endl;
			myorder->getItems();
			
			while (temp != 1 and temp != 2 and temp != 3) {
				cout << "Выберите способ доставки(1 - стандарт, 2 - экспересс, 3 - самовывоз): "; cin >> temp;
			}

			myorder->Create(OrderID, &users[userID], temp);
			users[userID].addOrder(myorder);

			cout << endl << "Итого: " << myorder->getCost() << " рублей" << endl;
			cout << "Примерное время ожидания: " << myorder->getStatus() << " часов" << endl;
			
			OrderID++;
			break;
		}
		case 2:
			cout << "Заказы пользователя " << users[userID].getName() << ":" << endl;
			if (users[userID].isOrdersEmpty()) cout << "здесь пока пусто :(" << endl;
			else users[userID].getOrders();
			break;
		}
		if (tempG == 3) break;
		cout << endl;
	}
	
}