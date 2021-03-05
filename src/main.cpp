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




    //pre: 1st param word to check if it is a keyword
    //post: returns if the word is a keyword
    //desc: loops through all tokens and finds the ones starting with a t
    //      it then checks if it matches with the word passed in
    //      it will then return if it found a match or not
    bool checkIfKeyword(string wordToCheck){
        map<string, string>::iterator mapStrStrIter;
        bool keywordCheck = false;

        for(mapStrStrIter = tokenmap.begin(); mapStrStrIter != tokenmap.end(); mapStrStrIter++){
            if((mapStrStrIter->first)[0] == 't' && (mapStrStrIter->second) == wordToCheck){
                keywordCheck = true;
            }
        }

        return keywordCheck;
    }
    //overloaded in case we need to pass in a char
    //pre: 1st param character to check if it is a keyword
    //post: returns if the character is a keyword
    //desc: converts the char to a string with length 1 and passes it the other checkIfKeyword
    bool checkIfKeyword(char charToCheck){
        return checkIfKeyword(string(1, charToCheck));
    }




    //pre: 1st param string to check if it is a valid Symbol
    //post: returns if the string is a valid Symbol
    //desc: loops through all tokens and finds the ones starting with a s
    //      it then checks if it matches with the string passed in
    //      it will then return if it found a match or not
    bool checkIfValidSymbol(string stringToCheck){
        map<string, string>::iterator mapStrStrIter;
        bool symbolCheck = false;

        for(mapStrStrIter = tokenmap.begin(); mapStrStrIter != tokenmap.end(); mapStrStrIter++){
            if((mapStrStrIter->first)[0] == 's' && (mapStrStrIter->second) == stringToCheck){
                symbolCheck = true;
            }
        }

        return symbolCheck;
    }
    //overloaded in case we need to pass in a char
    //pre: 1st param character to check if it is a valid Symbol
    //post: returns if the character is a valid Symbol
    //desc: converts the char to a string with length 1 and passes it the other checkIfValidSymbol
    bool checkIfValidSymbol(char charToCheck){
        return checkIfValidSymbol(string(1, charToCheck));
    }




    //pre: 1st param string to check if it starts with a digit
    //post: returns if the string starts with a digit
    //desc: uses the isdigit function to check if the first character is a digit
    bool checkIfStaringWithNumber(string stringToCheck){
        return isdigit(stringToCheck[0]);
    }
    //overloaded in case we need to pass in a char
    //pre: 1st param character to check if is a digit
    //post: returns if the char is a digit
    //desc: uses the isdigit function to check if the character is a digit
    bool checkIfStaringWithNumber(char charToCheck){
        return isdigit(charToCheck);
    }




    //pre: 1st param string to check if it only contains digits
    //post: returns if the string only contains digit
    //desc: loops through the string and uses the isdigit function to check if any character is not a digit
    bool checkIfAllNumber(string stringToCheck){
        bool nonNumberCheck = true;
        for(int charCount = 0; charCount < stringToCheck.size(); charCount++){
            if(!isdigit(stringToCheck[charCount])){
                nonNumberCheck = false;
            }
        }
        return nonNumberCheck;
    }
    //overloaded in case we need to pass in a char
    //pre: 1st param character to check if is a digit
    //post: returns if the char is a digit
    //desc: uses the isdigit function to check if the character is a digit
    bool checkIfAllNumber(char charToCheck){
        return isdigit(charToCheck);
    }




    //pre: 1st param string to check if it contains any symbols
    //post: returns if the string contains any symbols
    //desc: loops through the string and uses the isalnum function to check if any character is not alphanumeric
    bool checkIfThereAreSymbols(string wordToCheck){
        bool symbolCheck = false;
        for(int charCount = 0; charCount < wordToCheck.size(); charCount++){
            if(!isalnum(wordToCheck[charCount])){
                symbolCheck = true;
            }
        }
        return symbolCheck;
    }
    //overloaded in case we need to pass in a char
    //pre: 1st param character to check if is a symbol
    //post: returns if the char is a symbol
    //desc: uses the isalnum function to check if the character is a not alphanumeric
    bool checkIfThereAreSymbols(char charToCheck){
        return !isalnum(charToCheck);
    }

    void printToPage(ostream& outfile){
        if(lexemes.size() == tokens.size()){
            for(int i = 0; i < lexemes.size(); i++){
                outfile << tokens[i] << " : " << lexemes[i] << endl;
            }
        }else{
            //! some error
        }
    }



public:

    // pre: parameter refers to open data file consisting of token and
    // lexeme pairs i.e.  s_and and t_begin begin t_int 27.  Each pair    // appears on its own input line.
    // post: tokenmap has been populated
    LexAnalyzer(istream& infile){
        string token;
        string lexeme;
        while(!infile.eof()){
            infile >> token;
            infile >> lexeme;

            tokenmap[token] = lexeme;
        }
    };

    // pre: 1st parameter refers to an open text file that contains source
    // code in the language, 2nd parameter refers to an open empty output
    // file
    // post: If no error, the token and lexeme pairs for the given input
    // file have been written to the output file.  If there is an error,
    // the incomplete token/lexeme pairs, as well as an error message have // written to the output file.  A success or fail message has printed // to the console.
    void scanFile(istream& infile, ostream& outfile){
        string currentLine;
        int lineNumber = 0;

        string totalString;
        bool stringOn = false;
        int stringStartLine;

        string errors = "";

        infile >> currentLine;
        lineNumber++;
        while(!infile.eof() && errors.size() == 0){

            //splits line by delimitors(symbols)
            //it will check if symbol is a 2 length symbol
            vector<string> splitString;
            string currentLex = "";
            for(int cCount = 0; cCount < currentLine.size(); cCount++){
                if(isalnum(currentLine[cCount])){
                    currentLex += string(1, currentLine[cCount]);
                }else{
                    if(currentLex != ""){
                        splitString.push_back(currentLex);
                        currentLex = "";
                    }
                    if(cCount < (currentLine.size() - 1)){
                        if(checkIfValidSymbol(currentLine.substr(cCount, 2))){
                            splitString.push_back(currentLine.substr(cCount, 2));
                            cCount++;
                        }else{
                            splitString.push_back(string(1, currentLine[cCount]));
                        }
                    }else{
                        splitString.push_back(string(1, currentLine[cCount]));
                    }
                }
                if(cCount == (currentLine.size()-1) && currentLex != ""){
                    splitString.push_back(currentLex);
                }
            }


            cout << lineNumber << ": " << endl;
            cout << "=================" << endl;
            for(int i = 0; i < splitString.size(); i++){
                cout << splitString[i] << endl;
                //! logic for getting lex stuff
            }
            cout << endl;


            infile >> currentLine;
            lineNumber++;
        }

        //!output errors
        //!output lexical analysis
    };
};




int main() {

    //gets the filename of the lexeme file
    //and checks if that file exists
    string lexemeFileNameInput;
    cout << "Enter the filename containing your token/lexeme pairs: ";
    cin >> lexemeFileNameInput;

    ifstream lexemeFileIn(lexemeFileNameInput);
    if(!lexemeFileIn){
        cout << "Error Opening File " << lexemeFileNameInput << endl;
        exit(-1);
    }


    //gets the filename of the source code
    //and checks if that file exists
    string sourceFileNameInput;
    cout << "Enter the filename containing your code: ";
    cin >> sourceFileNameInput;

    ifstream sourceFileIn(sourceFileNameInput);
    if(!sourceFileIn){
        cout << "Error Opening File " << sourceFileNameInput << endl;
        exit(-1);
    }

    //gets the filename of the output file
    //and checks if that file exists
    string fileNameOut;
    cout << "Enter the filename to output your lexical analysis: ";
    cin >> fileNameOut;

    ofstream fileOut(fileNameOut);
    if(!fileOut){
        cout << "Error Opening File " << fileNameOut << endl;
        exit(-1);
    }


    //creates the lexical analyzer
    LexAnalyzer lexAnal(lexemeFileIn);

    //scans the source code and outputs lexical analysis
    lexAnal.scanFile(sourceFileIn, fileOut);

    //closes the output file
    fileOut.close();

    return 0;
}