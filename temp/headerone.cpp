#include "headerone.h"
#include <iostream>

using namespace std;

HeaderOne::HeaderOne(string n)
    : age(0) {
        cout << "my name is " << n << " and I am " << age << "years old" << endl;
}