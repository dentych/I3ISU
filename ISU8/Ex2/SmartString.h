#ifndef SMARTSTRING_H_
#define SMARTSTRING_H_

#include <iostream>
#include <string>

class SmartString {
	public:
		// Methods
		SmartString(std::string *str = new std::string(""));
		~SmartString();
		std::string* get();
		std::string* operator->();
		std::string& operator*();
		SmartString(const SmartString &other);
		SmartString& operator=(const SmartString &other);

	private:
		// Attributes
		std::string *str;
		unsigned int *counter;
};

#endif
