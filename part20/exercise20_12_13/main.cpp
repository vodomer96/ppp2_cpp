//------------------------------------------------------------------------------

#include "../example20/List_easy.hpp"

//------------------------------------------------------------------------------

int main()
try
{
    return 0;
}
catch(const exception& e) {
    cout << "\nerror: " << e.what() << '\n';
    return -1;
}
catch (...) {
    cerr << "oops!";
    return -2;
}

//----------------------------------------------------------------
