#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
using namespace std;

int main()
{
	string s = "hello world! ysl ";
	ssize_t pos = s.find('e', 0);
	cout << (pos < 0) << endl;
	cout << "pos " << pos << endl;
	string ss = s.substr(0, pos);
	cout << ss << endl;

	string a = "welcome";
	char *p = new char[50];
	strcpy(p, a.c_str());
	cout << p << endl;

	return 0;
}