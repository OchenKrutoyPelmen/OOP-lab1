#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Order {
private:
	double num;
	User user;
	string address;
	vector<Item> items;
	double cost;
	double delmet; // для способа доставки 1/2/3
	Сourier courier;
	double status; // кол-во часов/дней до прибытия заказа
public:

};

class User {
private:
	string name;
public:

};

class Сourier {
private:
	string name;
public:

};

class Item {
private:
	string name;
	unsigned quantity;
	unsigned price;
public:

};

void main() {
	setlocale(LC_ALL, "Russian");

	
}