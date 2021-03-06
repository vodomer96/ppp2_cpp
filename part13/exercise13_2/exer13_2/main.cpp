// Exercise 13_2
//-----------------------------------------------------------------------------

#include "../../../bstroustrup_code/std_lib_facilities.h"
#include "../../../bstroustrup_code/Simple_window.h"
#include "../../../bstroustrup_code/Graph.h"
#include "FL/fl_utf8.h"
#include <locale.h>

//-----------------------------------------------------------------------------

void use_box()
{
    Box rect{Point{100, 100}, 150, 200};
    Box rect2{Point{400, 200}, 200, 300, 51, 51};

    const Point x{0, 0};
    Simple_window win{x, 800, 600, "Box"};

    win.attach(rect);
    win.attach(rect2);

    win.wait_for_button();
}

int main()
try {
    use_box();
    keep_window_open("~~");
    return 0;
}
catch(exception& e) {
    cerr << e.what() << '\n';
    keep_window_open("~~");
    return -1;
}
catch (...) {
    cout << "exiting\n";
    keep_window_open("~");
    return -2;
}

//-----------------------------------------------------------------------------
