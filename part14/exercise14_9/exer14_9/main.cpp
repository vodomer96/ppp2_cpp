// exercise14_9
//-----------------------------------------------------------------------------

#include "../../../bstroustrup_code/std_lib_facilities.h"
#include "../../../bstroustrup_code/Simple_window.h"
#include "../../../bstroustrup_code/Graph.h"
#include "FL/fl_utf8.h"
#include <locale.h>

//-----------------------------------------------------------------------------

void exercise14_9()
{
    Simple_window win{Point {0, 0}, 1200, 800, "exercise 14_9"};
    Chess_board cb{Point{100,100}, 693};

    win.attach(cb);   
    win.wait_for_button();

    cb.move_checker(10, 80, 80);

    win.wait_for_button();

    cb.set_color_up_checkers(Color::dark_red);

    win.wait_for_button();
}

//-----------------------------------------------------------------------------

int main()
try {
    exercise14_9();
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
