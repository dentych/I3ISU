#include <iostream>
#include <string>
#include <osapi/MsgQueue.hpp>
#include "../inc/Threads.hpp"

using namespace std;

int main() {

	int carAmount = 10;

	// Guard message queue
	MsgQueue entryQueue(carAmount);
	MsgQueue exitQueue(carAmount);
	
	EntryGuardThread entryGuardThread(&entryQueue);
	ExitGuardThread exitGuardThread(&exitQueue);
	
	Thread entryGuard(&entryGuardThread);
	Thread exitGuard(&exitGuardThread);
	
	entryGuard.start();
	exitGuard.start();
	
	// Queue for car threads, so we can keep track of them, to be able to run .join() after creation.
	queue<Thread*> cars;
	queue<CarThread*> carThreads;
	for (int i = 0; i < carAmount; i++) {
		CarData *data = new CarData;
		data->entryMQ = &entryQueue;
		data->exitMQ = &exitQueue;
		data->ID = i;
		CarThread *carThread = new CarThread(data);
		Thread *car = new Thread(carThread);
		cars.push(car);
		carThreads.push(carThread);
		car->start();
	}

	entryGuard.join();
	exitGuard.join();
	while (!cars.empty()) {
		Thread *car = cars.front();
		cars.pop();
		car->join();
		delete car;
	}

	while (!carThreads.empty()) {
		CarThread *thread = carThreads.front();
		carThreads.pop();
		delete thread;
	}

	return 0;
}
