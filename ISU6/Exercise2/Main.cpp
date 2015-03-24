#include <iostream>
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

int main() {
	return 0;
}

void * senderThread(void * arg) {
	// Cast the void * arg to a MsgQueue.
	MsgQueue* mq = static_cast<MsgQueue*> (arg);
	
	// Run as long as the program is running.
	while(1) {
		// Create new Point3D message (inherits from message)
		Point3D * req = new Point3D;
		req->x = 1;
		req->y = 2;
		req->z = 3;
		
		// Send the message to the MsgQueue with id = DATA_REQ (a request).
		mq->send(DATA_REQ, req);
	}
}

void * receiverThread(void * arg) {
}
