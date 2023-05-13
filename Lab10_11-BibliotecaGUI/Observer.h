#pragma once
#pragma warning (disable: 26823) // dereferencing null pointer
#include <vector>
#include <algorithm>

using std::vector;
using std::remove;

/*
* Clasa abstracta pentru notificarea schimbarilor aparute in relatii de forma unul la mai multe intre clase
* metoda pur virtuala update() este implementata corespunzator de fiecare observator
* Clasa observabila va semnala prin notify() fiecare metoda care necesita un refresh
*/
class Observer {
public:
	virtual void update() = 0;

};

class Observable {
private:
	vector<Observer*> observers;

public:
	void addObserver(Observer* obs) {
		observers.push_back(obs);
	}

	void removeObserver(Observer* obs) {
		observers.erase(remove(observers.begin(), observers.end(), obs), observers.end());
	}

protected:
	void notify() {
		for (auto obs : observers) {
			obs->update();
		}
	}
};