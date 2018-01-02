//------------------------------------------------------------------------------

#include "Input_orders_window.hpp"

//------------------------------------------------------------------------------

using namespace Graph_lib;

//-----------------------------------------------------------------------------

int main()
try
{
    Input_orders_window iow{Point{100, 100}, 1024, 800, "Order"};
    return gui_main();
}
catch (const exception& e) {
    cout << "\nerror: " << e.what() << '\n';
    return -1;
}
catch (...) {
    cerr << "oops!";
    return -2;
}

//------------------------------------------------------------------------------
