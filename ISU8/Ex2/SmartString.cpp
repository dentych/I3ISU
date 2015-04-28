#include "SmartString.h"

SmartString::SmartString(std::string *str) : str(str), counter(new unsigned int) {
	*counter = 1;
}

SmartString::~SmartString() {
	if (--(*counter) <= 0) {
		delete str;
		delete counter;
	}
}

std::string* SmartString::get() {
	return str;
}

std::string* SmartString::operator->() {
	return get();
}

std::string& SmartString::operator*() {
	std::string *temp = get();
	return *temp;
}

SmartString::SmartString(const SmartString &other) {
	*this = other;
}

SmartString& SmartString::operator=(const SmartString &other) {
	str = other.str;
	counter = other.counter;
	(*counter)++;
}
