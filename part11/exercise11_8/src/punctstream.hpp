//------------------------------------------------------------------------------

#ifndef PUNCTSTREAM_H
#define PUNCTSTREAM_H

//------------------------------------------------------------------------------

#include "../../../bstroustrup_code/std_lib_facilities.h"

//------------------------------------------------------------------------------

struct Word_replace {   // Хранит слова для замены
    string short_form;  // Сокращенная форма слова (найти)
    string full_form;   // Полная форма слова (заменить)
};

//------------------------------------------------------------------------------

inline istream& operator >>(istream& ist, Word_replace& wr)
{
    string s1, s2;
    ist >> s1;
    if (!ist) return ist;
    while (ist >> s2) {
        wr.full_form += wr.full_form.size() ? " " + s2 : s2;
        if (s2.back() == ';') {
            wr.full_form.pop_back();
            break;
        }
    }
    if (wr.full_form.size())    // Если успешно считали полную
        wr.short_form = s1;     // форму, то записываем короткую
    return ist;
}

//------------------------------------------------------------------------------

class Punct_stream { // Аналогичен istream, но пользователь
                     // может задавать пробельные символы
public:
    Punct_stream(istream& is)
        :source{is}, sensetive{true},
          quotes{false} {}
    void whitespace(const string& s)    // Делает s строкой
        {   white = s;   }              // пробельных символов
    void add_white(char c)              // Добавляет символ в
        {   white += c;   }             // набор пробельных
    bool is_whitespace(char c) const;   // Входит ли c в набор
                                        // пробельных символов?
    // Делает w списком слов для замены
    void wordreplace(const vector<Word_replace>& w)
        {    wr_tb = w;   }
    // Добавляет слова в список для замены
    void add_wordreplace(const Word_replace& w)
        {   wr_tb.push_back(w);   }
    void find_replace(string& line);    // Находит и заменяет слова
                                        // из списка

    void case_sensitive(bool b) { sensetive = b; }
    bool is_case_sensitive()    { return sensetive; }
    bool is_quotes(char c);

    Punct_stream& operator>>(string& s);
    operator bool();

protected:
    virtual void analyzer_ch(char& ch, char prev, char next);
private:
    istream& source;            // Источник символов
    istringstream buffer;       // Буфер для форматирования
    string white;               // Пробельные символы
    bool sensetive;             // Чувствителен ли поток регистру?
    bool quotes;                // Чувствителен ли поток к пробельным
                                // символам
    vector<Word_replace> wr_tb; // Список слов для замены
};

//------------------------------------------------------------------------------

#endif // PUNCTSTREAM_H

//------------------------------------------------------------------------------
