#include <iostream>
#include "SmartString.h"

using namespace std;

int main() {
	SmartString s(new string("Joachim"));

	cout << "String is this: " << *s << ". Length: " << s->length() << endl;
	SmartString s2(s);
	cout << "String2 is this: " << *s2 << ". Length: " << s2->length() << endl;
	SmartString s3;
	s3 = s;
	cout << "String3 is this: " << *s3 << ". Length: " << s3->length() << endl;

	return 0;
}
