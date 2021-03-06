//------------------------------------------------------------------------------

#include "../../bstroustrup_code/std_lib_facilities.h"

//------------------------------------------------------------------------------

namespace X {
int var = 0;
void print();
}

//------------------------------------------------------------------------------

void X::print()
{
    cout << "X::var = " << X::var << '\n';
}

//------------------------------------------------------------------------------

namespace Y {
int var = 0;
void print();
}

//------------------------------------------------------------------------------

void Y::print()
{
    cout << "Y::var = " << Y::var << '\n';
}

//------------------------------------------------------------------------------

namespace Z {
int var = 0;
void print();
}

//------------------------------------------------------------------------------

void Z::print()
{
    cout << "Z::var = " << Z::var << '\n';
}

//------------------------------------------------------------------------------
