#ifndef CARHANDLERS_HPP_
#define CARHANDLERS_HPP_

#include "../../Exercise1/Message.h"
#include "../../Exercise1/MsgQueue.h"
#include "../inc/MessageTypes.h"

DoorOpenReq* carGenerateOpenMsg(MsgQueue *entryQueue, int id) {
	DoorOpenReq *req = new DoorOpenReq;
	req->mq = entryQueue;
	req->carID = id;

	return req;
}

void handleMsg(Message *msg, unsigned long id) {
	switch(id) {
		case DOOR_IN_OPEN_CFM:
			
	}
}

#endif
