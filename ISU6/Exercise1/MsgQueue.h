#ifndef MSGQUEUE_H_
#define MSGQUEUE_H_

#include <iostream>
#include <deque>
#include <pthread.h>
#include "Message.h"

struct Item {
	unsigned long id;
	Message * msg;
};

class MsgQueue {
public:
	MsgQueue(unsigned long maxSize);
	void send(unsigned long id, Message * msg = NULL);
	Message * receive(unsigned long & id);
	~MsgQueue();
private:
	std::deque<Item> mqitems;
	unsigned long maxSize;
	pthread_mutex_t mqMut;
	pthread_cond_t sender;
	pthread_cond_t receiver;
};

#endif
