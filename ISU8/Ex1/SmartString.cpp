#include "SmartString.h"

SmartString::SmartString(std::string *str) : str(str) {}

SmartString::~SmartString() {
	delete str;
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

SmartString::SmartString(const SmartString &other) {}

SmartString& SmartString::operator=(const SmartString &other) {}
