//-----------------------------------------------------------------------------

#include "../../../bstroustrup_code/std_lib_facilities.h"

//-----------------------------------------------------------------------------

double ctof(double c)	// Преобразует температуру
{						// в градусах Цельсия
                        // в температуру в градусах Фаренгейта
    double f = 9.0 / 5 * c + 32;
    return f;
}

//-----------------------------------------------------------------------------

int main()
try {
    cout << "Введите температуру в Цельсиях: ";
    double c = 0;			// Объявляем входную переменную
    cin >> c;				// Вводим температуру
    double f = ctof(c);		// Преобразуем температуру
    cout << f << '\n';		// Выводим температуру

    keep_window_open();
    return 0;
}
catch (exception& e) {
    cerr << e.what() << '\n';
    keep_window_open();
    return -1;
}
catch (...) {
    cerr << "exiting\n";
    keep_window_open();
    return -2;
}

//-----------------------------------------------------------------------------