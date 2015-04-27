// HEJ SØREN.

// Denne fil indeholder et spørgsmål ang. carthread funktionen

// Er dette okay i forhold til Listen 3.1?

void * carThread(void *param) {
	// Variable stuff

	for (;;) {
		generateInMsg();
		receiveReply();
		handleInCfmMsg();
		deleteMsg();

		sleepWhileParked();

		generateOutMsg();
		receiveReply();
		handleOutCfmMsg();
		deleteMsg();
	}
}
