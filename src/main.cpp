#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class LexAnalyzer{
private:
	vector<string> lexemes;  // source code file lexemes
	vector<string> tokens;   // source code file tokens
	map<string, string> tokenmap;  // valid lexeme/token pairs
	// other private methods
public:

    LexAnalyzer(istream& infile);
    // pre: parameter refers to open data file consisting of token and
    // lexeme pairs i.e.  s_and and t_begin begin t_int 27.  Each pair    // appears on its own input line.
    // post: tokenmap has been populated

    void scanFile(istream& infile, ostream& outfile);
    // pre: 1st parameter refers to an open text file that contains source
    // code in the language, 2nd parameter refers to an open empty output
    // file
    // post: If no error, the token and lexeme pairs for the given input
    // file have been written to the output file.  If there is an error,
    // the incomplete token/lexeme pairs, as well as an error message have // written to the output file.  A success or fail message has printed // to the console.
};




int main() {
    string lexemeFileNameInput;
    cout << "Enter the filename containing your token/lexeme pairs: ";
    cin >> lexemeFileNameInput;

    ifstream lexemeFileIn(lexemeFileNameInput);
    if(!lexemeFileIn){
        cout << "Error Opening File " << lexemeFileNameInput << endl;
        exit(-1);
    }

    string sourceFileNameInput;
    cout << "Enter the filename containing your code: ";
    cin >> sourceFileNameInput;

    ifstream sourceFileIn(sourceFileNameInput);
    if(!sourceFileIn){
        cout << "Error Opening File " << sourceFileNameInput << endl;
        exit(-1);
    }

    string fileNameOut;
    cout << "Enter the filename to output your lexical analysis: ";
    cin >> fileNameOut;

    ofstream fileOut(fileNameOut);
    if(!fileOut){
        cout << "Error Opening File " << fileNameOut << endl;
        exit(-1);
    }

    LexAnalyzer lexAnal(lexemeFileIn);

    lexAnal.scanFile(sourceFileIn, fileOut);

    fileOut.close();

    return 0;
}