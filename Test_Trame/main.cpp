#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::vector;
using std::cin; using std::stringstream;


int main()
{
    string filename("trame.txt");
    vector<string> lines;
    string line;
    string trame;
    int k=0;
    string delimiter = ",";
    vector<string> words{};
    size_t pos;
    stringstream sstream(trame);
    string word;

    ifstream input_file(filename);
    if (!input_file.is_open()){
        cerr << "Impossible d'ouvrir le fichier - '"
             << filename << "'" << endl;
        return EXIT_FAILURE;
    }

    while (getline(input_file, line)){
        lines.push_back(line);
    }

    for (const auto &i : lines)
    {

        trame = i;
        cout << trame << endl;
//        while (std::getline(sstream, word, delimiter)){
        words.push_back(word);
    }

    for (const auto &str : words) {

        cout << str << endl;
    }











    input_file.close();
    return EXIT_SUCCESS;
}
