#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "../Exercise1/Message.h"
#include "../Exercise1/MsgQueue.h"

using namespace std;

enum {
	DATA_REQ,
	DATA_CFM
};
struct Point3D : public Message {
	int x,y,z;
};

void printOutPoint(Point3D * point) {
	cout << "Point3D print:" << endl;
	cout << "X: " << point->x << ". Y: " << point->y << ". Z: " << point->z << "." << endl;
}

void printOutPointHandler(Message* msg, unsigned long id) {
	switch (id) {
		case DATA_REQ:
		{
			Point3D * point = static_cast<Point3D *>(msg);
			printOutPoint(point);
			break;
		}
		case DATA_CFM:
			//Nothing.
			break;
		default:
			cout << "Something went wrong. You shouldn't be here..?!" << endl;
			break;
	}
}

void * senderThread(void * arg) {
	// Cast the void * arg to a MsgQueue.
	MsgQueue* mq = static_cast<MsgQueue*> (arg);

	// Run as long as the program is running.
	while(1) {
		// Create new Point3D message (inherits from message)
		Point3D * req = new Point3D;
		req->x = rand() % 100;
		req->y = rand() % 100;
		req->z = rand() % 100;

		// Send the message to the MsgQueue with id = DATA_REQ (a request).
		mq->send(DATA_REQ, req);

		sleep(1);
	}
}

void * receiverThread(void * arg) {
	MsgQueue* mq = static_cast<MsgQueue*>(arg);

	while(1) {
		unsigned long id;
		Message * msg = mq->receive(id);
		printOutPointHandler(msg, id);
		delete msg;
	}
}

int main() {
	// Create message queue
	MsgQueue mq(10);
	// Thread variables
	pthread_t sender, receiver;
	// Init threads
	pthread_create(&sender, NULL, senderThread, &mq);
	pthread_create(&receiver, NULL, receiverThread, &mq);
	
	pthread_join(sender, NULL);
	pthread_join(receiver, NULL);

	return 0;
}
