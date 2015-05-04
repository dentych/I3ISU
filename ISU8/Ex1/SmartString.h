#ifndef SMARTSTRING_H_
#define SMARTSTRING_H_

#include <string>

class SmartString {
	public:
		// Methods
		SmartString(std::string *str = new std::string(""));
		~SmartString();
		std::string* get();
		std::string* operator->();
		std::string& operator*();

	private:
		// Methods
		SmartString(const SmartString &other);
		SmartString& operator=(const SmartString &other);

		// Attributes
		std::string *str;
};

#endif
