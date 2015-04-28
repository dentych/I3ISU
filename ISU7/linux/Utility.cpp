#include <osapi/Utility.hpp>

namespace osapi {
	void sleep(unsigned long msecs) {
		unsigned long sleeptime = msecs*1000;
		usleep(sleeptime);
	}
}
