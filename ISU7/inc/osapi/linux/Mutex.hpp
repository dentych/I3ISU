#ifndef OSAPI_LINUX_MUTEX_HPP_
#define OSAPI_LINUX_MUTEX_HPP_

#include <pthread.h>

namespace osapi
{    
	class Mutex
	{
		public:
			Mutex();
			void lock();
			void unlock();
			~Mutex();
		private:
			pthread_mutex_t mut_;
			
			friend class Conditional;
	};

}


#endif
