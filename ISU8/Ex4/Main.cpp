#include <iostream>
#include <string>
#include <memory>

using namespace std;

int main() {
	shared_ptr<string> s1(new string("This is a string!"));
	cout << "String1 created. Content: " << *s1 << ". Count: " << s1.use_count() << "." << endl;
	shared_ptr<string> s2(s1);
	cout << "String2 created. Content: " << *s2 << ". Count: " << s2.use_count() << "." << endl;
	shared_ptr<string> s3;
	s3 = s1;
	cout << "String3 created. Content: " << *s3 << ". Count: " << s3.use_count() << "." << endl;

	string *str;
	{
		cout << "Creating string smartpointer." << endl;
		shared_ptr<string> sp(new string("Yespls"));

		str = sp.get();

		cout << "String is: " << *sp << ". Length: " << sp->length() << endl;
	}

	cout << "String after out-of-scope: ";

	{
		cout << *str << endl; // Gives segmentation fault, which is expected
	}

	return 0;
}
