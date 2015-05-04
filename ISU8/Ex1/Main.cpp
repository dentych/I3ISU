#include <iostream>
#include "SmartString.h"

using namespace std;

int main ()
{
	SmartString ss(new string("Hello world"));
	cout << "String length : " << ss->length() << endl;

	if(ss->find ("world") != string::npos )
		cout << "The ’ss’ string does contain ’world’" << endl ;
	else
		cout << "The ’ss’ string does NOT contain ’world’" << endl;

	//SmartString ss2 = ss; // <- Deliberately why ? What happens ?
}
