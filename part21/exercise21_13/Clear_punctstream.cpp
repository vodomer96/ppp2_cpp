//------------------------------------------------------------------------------

#include "Clear_punctstream.hpp"

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
    ifstream ifs{path + "wordreplace"};
    if (!ifs) error("Невозможно открыть входной файл!");
    ifs.exceptions(ifs.exceptions()|ios_base::badbit);
    for (Word_replace wr; ifs >> wr;)
        wr_tb.push_back(wr);

    end_of_loop(ifs, '*', "Некорректный ввод");
}

//------------------------------------------------------------------------------

map<string, int> get_data_from_file(const string& fname)
{
    vector<Word_replace> wr_tb;
    init_word_replace("./", wr_tb);
    // Открытие потока istream для бинарного чтения файла
    ifstream ifs {fname};
    if (!ifs) error("Невозможно открыть входной файл: ", fname);

    Clear_punctstream ps(ifs);
    ps.whitespace(".;,?-'()");
    ps.wordreplace(move(wr_tb));
    ps.case_sensitive(false);
    ps.set_plural(true);

    map<string, int> words; // Поддержка пар (слово, частота)
    for (string s; ps >> s; ) ++words[s];
    return words;
}

//------------------------------------------------------------------------------
