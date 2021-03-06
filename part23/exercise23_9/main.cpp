//------------------------------------------------------------------------------

#include "../../bstroustrup_code/std_lib_facilities.h"

//------------------------------------------------------------------------------

template<typename Target, typename Source>
Target m_to(const Source& arg)
{
    stringstream interpreter;
    Target result;
    if (       !(interpreter << arg)    // Запись arg в поток
            || !(interpreter >> result) // Чтение result из потока
            || !(interpreter >> std::ws).eof())
        throw runtime_error{"сбой в m_to<>()"};
    return result;
}

//------------------------------------------------------------------------------

void exercise23_9()
{
    ifstream in{"table.txt"};   // Входной файл
    if (!in) error("Нет входного файла\n");

    string line;                // Входной буфер
    int lineno{0};

    regex header{R"(^[\w ]+(\s+[\w ]+)*$)"};
    regex row{R"(^[\w ]+(\s+\d+)(\s+\d+)(\s+\d+)$)"};

    if (getline(in, line)) {    // Проверка заголовка
        smatch matches;
        if (!regex_match(line, matches, header))
            error("нет заголовка");
    }

    // Итог по столбцам
    int boys{0};
    int girls{0};

    int curr_boy{0};
    int curr_girl{0};

    while (getline(in, line)) { // Проверка данных
        ++lineno;
        smatch matches;
        if (!regex_match(line, matches, row))
            error("неверная строка ", to_string(lineno));

        // Проверка строки
        curr_boy   = m_to<int>(matches[1]);
        curr_girl  = m_to<int>(matches[2]);
        int curr_total = m_to<int>(matches[3]);

        if (curr_boy+curr_girl != curr_total)
            error("Неверная сумма в строке\n");

        if ((in >> ws).eof())   // Внимание! Съедает пробельные
            break;              // символы, определяет последнюю строку

        // Обновление итоговых значений
        boys  += curr_boy;
        girls += curr_girl;
    }

    if (curr_boy != boys)
        error("Количество мальчиков не сходится\n");
    if (curr_girl != girls)
        error("Количество девочек не сходится\n");
}

//------------------------------------------------------------------------------

int main()
try
{
    exercise23_9();
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
