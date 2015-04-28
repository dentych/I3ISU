#include <iostream>
#include <string>
#include <memory>

using namespace std;

int main() {
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
