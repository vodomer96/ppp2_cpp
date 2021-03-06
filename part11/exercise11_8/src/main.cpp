//------------------------------------------------------------------------------

#include "punctstream.hpp"

//------------------------------------------------------------------------------

void end_of_loop(istream& ist, char term, const string& message)
{
    if (ist.eof()) {
        ist.clear();
        return;
    }
    if (ist.fail()) {
        ist.clear();
        char c;
        if (ist >> c && c==term) return;
        error("end_of_loop: ", message);
    }
}

//------------------------------------------------------------------------------

void init_word_replace(const string& path, vector<Word_replace>& wr_tb)
{
    cout << "Введите имя файла для ввода "
         << "списка слов для замены\n";
    string iname;
    cin >> iname;
    iname = path + iname;
    ifstream ifs {iname};
    if (!ifs) error("Невозможно открыть входной файл: ", iname);
    ifs.exceptions(ifs.exceptions()|ios_base::badbit);
    for (Word_replace wr; ifs >> wr;)
        wr_tb.push_back(wr);

    end_of_loop(ifs, '*', "Некорректный ввод");
}

//------------------------------------------------------------------------------

void work_with_ps()
{
    const string path = "./";
    vector<Word_replace> wr_tb;
    init_word_replace(path, wr_tb);
    // Открытие потока istream для бинарного чтения файла
    cout << "Введите имя файла для ввода\n";
    string iname;
    cin >> iname;
    iname = path + iname;
    ifstream ifs {iname};
    if (!ifs) error("Невозможно открыть входной файл: ", iname);

    Punct_stream ps(ifs);
    //Punct_stream ps(cin);
    ps.whitespace(".;,?-'");
    ps.wordreplace(wr_tb);
    ps.case_sensitive(false);

    vector<string> vs;
    for (string word; ps >> word;)
        vs.push_back(word);         // Ввод слов
    sort(vs.begin(), vs.end());     // Сортировка в лексикографическом
                                    // порядке
    for (size_t i=0; i <vs.size(); ++i)
        if (i==0 || vs[i]!=vs[i-1]) cout << vs[i] << '\n';
}

//------------------------------------------------------------------------------

int main()
try {
    work_with_ps();
    keep_window_open("~~");
    return 0;
}
catch(exception& e) {
    cerr << e.what() << '\n';
    keep_window_open("~~");
    return -1;
}

//------------------------------------------------------------------------------
