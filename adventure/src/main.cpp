#include <iostream>

#include "adventure.h"

using namespace std;

int main()
{
    Adventure::get_instance()->start_adventuring();
    return 0;
}