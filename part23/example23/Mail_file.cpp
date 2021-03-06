//------------------------------------------------------------------------------

#include "Mail_file.hpp"

//------------------------------------------------------------------------------

Mail_file::Mail_file(const string& n)
    // Открывает файл с именем n
    // Считывает строки из файла в n
    // Находит в lines сообщения и помещает их в m;
    // для простоты полагаем, что каждое сообщение
    // заканчивается строкой "----"
{
    ifstream in{n}; // Открываем файл
    if (!in) throw Bad_mail_file{"Не удалось открыть файл: " + n};

    string marker{"----"};
    string s;
    if (!getline(in, s) || s==marker)
        throw Bad_mail_file{"Некоректное начало файла!"};
    lines.push_back(s);

    while (getline(in, s)) {            // Создаем вектор строк
        if (s==marker && lines.back()==marker)
            throw Bad_mail_file{"Два marker подряд!"};
        lines.push_back(s);
    }

    auto first{lines.begin()};          // Создаем вектор сообщений

    for (auto p{lines.begin()}; p != lines.end(); ++p)
        if (*p == marker) {             // Конец сообщения
            m.push_back(Message{first, p});
            first = p+1;                // Строка "----" не является
                                        // частью сообщения
        }

    if (m.begin() == m.end())
        throw Bad_mail_file{"Нет писем!"};
}

//------------------------------------------------------------------------------

bool find_from_addr(const Message* m, string& s)
{
    if (m == nullptr)
        throw runtime_error{"Bad message"};

    for (const auto& x : *m)
        if (int n = is_prefix(x, "From: ")) {
            s = string(x, n);
            return true;
        }
    return false;
}

//------------------------------------------------------------------------------

bool find_subject(const Message* m, string& s)
{
    if (m == nullptr)
        throw runtime_error{"Bad message"};

    for (const auto& x : *m)
        if (int n = is_prefix(x, "Subject: Re: ")){
            s = string(x, n);
            return true;
        }
        else if (int n = is_prefix(x, "Subject: ")) {
            s = string(x, n);
            return true;
        }

    return false;
}

//------------------------------------------------------------------------------
