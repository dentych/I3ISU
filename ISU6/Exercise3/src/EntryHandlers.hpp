#ifndef ENTRYHANDLERS_HPP_
#define ENTRYHANDLERS_HPP_

#include <iostream>
#include "../inc/MessageTypes.h"

using namespace std;

void handleEntryReady(Message *msg, unsigned long id, GuardState &state, queue<DoorOpenReq*> &guardQ) {
	switch(id) {
		case DOOR_IN_OPEN_REQ:
			DoorOpenReq *req = static_cast<DoorOpenReq*>(msg);
			handleEntryDoorInOpenReq(req);
			break;
		default:
			cout << "Entry guard handler handleEntryReady received a wrong Message" << endl;
			break;
	}
}

void handleEntryBusy(Message *msg, unsigned long id, GuardState &state, queue<DoorOpenReq*> &guardQ) {
	switch(id) {
		case DOOR_IN_OPEN_REQ:
			DoorOpenReq *req = static_cast<DoorOpenReq*>(msg);
			guardQ.push(req);
			break;
		case CAR_INSIDE:
			CarInside *req = static_cast<CarInside*>(msg);
			if (!guardQ.empty()) {
				handleEntryWaitingReqs(guardQ);
			}
			break;
	}
}

void entryHandleMsg(Message *msg, unsigned long id, GuardState &state, queue<DoorOpenReq*> &guardQ) {
	switch (state) {
		case ST_READY:
			handleEntryReady(msg, id, state, guardQ);
			break;
		case ST_BUSY:
			handleEntryBusy(msg, id, state, guardQ);
			break;
	}
}

#endif
