// Глава №9, задание №5
//------------------------------------------------------------------------------

#include "book.h"

//------------------------------------------------------------------------------

int main()
{
    Book b{"231-3123-313-3", "Fdscx$",
          "ssdD", Chrono::Date{2013,Chrono::Month(5),2}, 0};

    (b.is_issue())
            ? cout << "Книга выдана!" << '\n'
            : cout << "Книга не выдана!" << '\n';

    keep_window_open("~~");
    return 0;
}

//------------------------------------------------------------------------------
