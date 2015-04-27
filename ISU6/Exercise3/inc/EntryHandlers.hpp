#ifndef ENTRYHANDLERS_HPP_
#define ENTRYHANDLERS_HPP_

#include <iostream>
#include <queue>
#include "../inc/MessageTypes.h"
#include "../../Exercise1/Message.h"
#include "../../Exercise1/MsgQueue.h"

using namespace std;

void entryGenerateDoorOpenCfm(MsgQueue *mq) {
	DoorOpenCfm *response = new DoorOpenCfm;
	response->result = true;
	mq->send(DOOR_IN_OPEN_CFM, response);
}

void handleEntryDoorInOpenReq(DoorOpenReq *req) {
	cout << "ENTRY DOOR open for car #" << req->carID << "." << endl;
	entryGenerateDoorOpenCfm(req->mq);
}

void handleEntryReady(Message *msg, unsigned long id, GuardState &state, queue<DoorOpenReq*> &guardQ) {
	switch(id) {
		case DOOR_IN_OPEN_REQ:
		{
			DoorOpenReq *req = static_cast<DoorOpenReq*>(msg);
			handleEntryDoorInOpenReq(req);
			break;
		}
		default:
		{
			cout << "Entry guard handler handleEntryReady received a wrong Message" << endl;
		}
	}
}

void handleEntryBusy(Message *msg, unsigned long id, GuardState &state, queue<DoorOpenReq*> &guardQ) {
	switch(id) {
		case DOOR_IN_OPEN_REQ:
		{
			DoorOpenReq *req = static_cast<DoorOpenReq*>(msg);
			guardQ.push(req);
			break;
		}
		case CAR_INSIDE:
		{
			CarInside *req = static_cast<CarInside*>(msg);
			if (!guardQ.empty()) {
				//handleEntryWaitingReqs(guardQ);
			}
			state = ST_READY;
			break;
		}
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
