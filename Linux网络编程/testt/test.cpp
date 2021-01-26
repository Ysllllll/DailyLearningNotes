#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Object
{
public:
    Object(int t) : i(t)
    {
    }
    ~Object()
    {
    }

private:
    int i;
    int c;
    int g;
};

void func()
{
    static Object object_s(48);
}

static Object object_s(8);

int main()
{
    Object object(16);
    Object *object_p = new Object(32);
    delete object_p;
    func();
    return 0;
}
