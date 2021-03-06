//------------------------------------------------------------------------------

#include "../../bstroustrup_code/std_lib_facilities.h"

//------------------------------------------------------------------------------

template<typename In, typename T>
// Требует Input_iterator<In>()
// и Equality_comparable<Value_type<T>>()
Iterator_difference_type<In> mcount(In first, In last,
                                    const T& val, bool is_empty)
// Количество вхождений val в диапазон [first, last]
{
    Iterator_difference_type<In> cnt{0};
    if (is_empty) return cnt;
    while (true) {
        if (*first == val) ++cnt;
        if (first == last) break;
        ++first;
    }   
    return cnt;
}

//------------------------------------------------------------------------------

template<typename In, typename Pred>
// Требует Input_iterator<In>()
// и Predicate<Pred, Value_type<In>>
Iterator_difference_type<In> mcount_if(In first, In last,
                                       Pred pred, bool is_empty)
// Количество элементов удовлетворяющих предикату
{
    Iterator_difference_type<In> cnt{0};
    if (is_empty) return cnt;
    while (true) {
        if (pred(*first)) ++cnt;
        if (first == last) break;
        ++first;
    }
    return cnt;
}

//------------------------------------------------------------------------------

template<typename In, typename T>
// Требует Input_iterator<In>()
// и Equality_comparable<Value_type<T>>()
pair<bool, In> mfind(In first, In last, const T& val, bool is_empty)
// Находит первый элемент в [first, last], равный val
{
    auto p{make_pair(false, In{})};
    if (is_empty) return p;
    while (*first != val) {
        if (first == last) return p;
        ++first;
    }
    return {true, first};
}

//------------------------------------------------------------------------------

void exercise_21_5()
{
    vector<int> vi{0, 2, 3, 3, 4, 5, 3, 6, 3};
    auto last_vi{vi.end()};
    --last_vi;

    cout << mcount(vi.begin(), last_vi, 3, vi.empty()) << '\n';

    list<int> li{vi.begin(), vi.end()};
    auto last_li{li.end()};
    --last_li;

    cout << mcount(li.begin(), last_li, -1, li.empty()) << '\n'
         << mcount_if(li.begin(), last_li,
                      [] (int a) { return a > 3; }, li.empty())
         << '\n'
         << mcount_if(li.begin(), last_li,
                      [] (int a) { return a > 2; }, li.empty())
         << '\n';

    auto p{mfind(li.begin(), last_li, 2, li.empty())};
    if (p.first)
        cout << "I find: " << *p.second << '\n';
    else
        cout << "Not found!\n";
}

//------------------------------------------------------------------------------

int main()
try
{
    exercise_21_5();
    return 0;
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
