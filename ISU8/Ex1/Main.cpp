#include <iostream>
#include "SmartString.h"

using namespace std;

int main() {
	SmartString s(new string("Joachim"));

	cout << "String is this: " << *s << ". Length: " << s->length() << endl;

	return 0;
}
