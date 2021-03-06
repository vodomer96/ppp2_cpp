//-----------------------------------------------------------------------------

#include "../../../bstroustrup_code/std_lib_facilities.h"
#include "chess_window.h"

//-----------------------------------------------------------------------------

int main()
try {
    Chess_window win{Point{100, 100}, 600, 400, "Chess"};
    return gui_main();
}
catch(exception& e) {
    cerr << e.what() << '\n';
    return -1;
}
catch (...) {
    cerr << "exiting\n";
    return -2;
}

//-----------------------------------------------------------------------------
