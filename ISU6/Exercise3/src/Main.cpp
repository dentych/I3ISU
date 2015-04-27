#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include "../inc/CarHandlers.hpp"
#include "../inc/EntryHandlers.hpp"

using namespace std;

void * entryGuardThread(void *data) {
	MsgQueue *mq = static_cast<MsgQueue*>(data);
	GuardState state = ST_READY;
	queue<DoorOpenReq*> waitingReqs;

	for (;;) {
		unsigned long id;
		Message *msg = mq->receive(id);
		entryHandleMsg(msg, id, state, waitingReqs);
		delete msg;
	}
}

void * carThread(void *data) {
	// Convert void * data to CarData struct.
	CarData *cardata = static_cast<CarData*>(data);
	// Extract the mq pointers from the CarData struct
	MsgQueue *entryMQ = cardata->entryMQ;
	MsgQueue *exitMQ = cardata->exitMQ;
	// ID of this car thread
	int carID = cardata->ID;
	// A queue specific for this car (for the door open confirm)
	MsgQueue carQ(1);
	
	// Start ze car
	entryMQ->send(DOOR_IN_OPEN_REQ, carGenerateOpenMsg(entryMQ, carID));

	for (;;) {
		unsigned long id;
		Message *msg = carQ.receive(id);
		delete msg;
	}
}

/*
void * exitGuardThread(void *data) {
	MsgQueue *mq = static_cast<MsgQueue*>(data);
	queue<Message*> waitingReqs;
	GuardState state = ST_READY;

	for (;;) {
		db("For loop");
		unsigned long id;
		Message *msg;

		if (state == ST_READY) {
			if (waitingReqs.empty()) {
				msg = mq->receive(id);
				dispatcher(msg, id);
				delete msg;
			}
			else {
				msg = waitingReqs.front();
				waitingReqs.pop();
				dispatcher(msg, DOOR_OUT_OPEN_REQ);
				delete msg;
			}
			state = ST_BUSY;
		}
		else {
			// ST_BUSY
			msg = mq->receive(id);

			if (id == CAR_OUTSIDE) {
				dispatcher(msg, id);
				delete msg;
				state = ST_READY;
			}
			else {
				// ID DOOR_OUT_OPEN_REQ
				waitingReqs.push(msg);
			}
		}
	}
}
*/

int main() {
	srand(time(NULL));

	int carAmount = 200;

	// Guard message queue
	MsgQueue entryQueue(carAmount);
	MsgQueue exitQueue(carAmount);

	// Thread variables
	pthread_t entryGuard, exitGuard, cars[carAmount];

	if (pthread_create(&entryGuard, NULL, entryGuardThread, &entryQueue) != 0) {
		cout << "Error creating entryGuard thread.." << endl;
		return -1;
	}
	/*if (pthread_create(&exitGuard, NULL, exitGuardThread, &exitQueue) != 0) {
		cout << "Error creating exitGuard thread.." << endl;
		return -1;
	}*/
	for (int i = 0; i < carAmount; i++) {
		CarData *data = new CarData;
		data->entryMQ = &entryQueue;
		data->exitMQ = &exitQueue;
		data->ID = i;
		if (pthread_create(&cars[i], NULL, carThread, data) != 0) {
			cout << "Error creating car #" << data->ID;
		}
	}

	pthread_join(entryGuard, NULL);
	//pthread_join(exitGuard, NULL);
	for (int i = 0; i < carAmount; i++) {
		pthread_join(cars[i], NULL);
	}
}
