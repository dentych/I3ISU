#ifndef OSAPI_LINUX_MUTEX_HPP
#define OSAPI_LINUX_MUTEX_HPP

#include <pthread.h>

namespace osapi
{    
	class Mutex
	{
		public:
		Mutex();
		~Mutex();
		void lock();
		void unlock();
		private:
		pthread_mutex_t mut;
	};

}


#endif
