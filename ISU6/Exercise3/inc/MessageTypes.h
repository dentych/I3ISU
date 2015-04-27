#ifndef MESSAGETYPES_H_
#define MESSAGETYPES_H_

enum {
	DOOR_IN_OPEN_REQ,
	DOOR_IN_OPEN_CFM,
	DOOR_OUT_OPEN_REQ,
	DOOR_OUT_OPEN_CFM,
	CAR_INSIDE,
	CAR_OUTSIDE
};

enum GuardState {
	ST_READY,
	ST_BUSY
};

struct CarData {
	MsgQueue *entryMQ;
	MsgQueue *exitMQ;
	int ID;
};

struct DoorOpenReq : public Message {
	MsgQueue *mq;
	int carID;
};

struct DoorOpenCfm : public Message {
	bool result;
};

struct CarInside : public Message {
	int id;
};

struct CarOutside : public Message {
	int id;
};

#endif
