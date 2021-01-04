#include <vector>
#include <iostream>
using namespace std;
int main()
{
    vector<int> a;
    a[0] = 1;
    for (vector<int>::iterator it = a.begin(); it != a.end(); it++)
        cout << *it;
}
