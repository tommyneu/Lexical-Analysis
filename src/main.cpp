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

    //pre:
    //post:
    //desc:
    string getKeywordToken(string lexemeToCheck){
        map<string, string>::iterator mapStrStrIter;
        string keywordToken = "";

        for(mapStrStrIter = tokenmap.begin(); mapStrStrIter != tokenmap.end(); mapStrStrIter++){
            if((mapStrStrIter->first)[0] == 't' && (mapStrStrIter->second) == lexemeToCheck){
                keywordToken = (mapStrStrIter->first);
            }
        }

        return keywordToken;
    }

    //pre:
    //post:
    //desc:
    string getSymbolToken(string lexemeToCheck){
        map<string, string>::iterator mapStrStrIter;
        string symbolToken = "";

        for(mapStrStrIter = tokenmap.begin(); mapStrStrIter != tokenmap.end(); mapStrStrIter++){
            if((mapStrStrIter->first)[0] == 's' && (mapStrStrIter->second) == lexemeToCheck){
                symbolToken = (mapStrStrIter->first);
            }
        }

        return symbolToken;
    }

    //pre: 1st param string to check if it starts with a digit
    //post: returns if the string starts with a digit
    //desc: uses the isdigit function to check if the first character is a digit
    bool checkIfStaringWithNumber(string stringToCheck){
        return isdigit(stringToCheck[0]);
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

    //pre:
    //post:
    //desc:
    void printToPage(ostream& outfile){
        if(lexemes.size() == tokens.size()){
            for(int i = 0; i < lexemes.size(); i++){
                outfile << tokens[i] << " : " << lexemes[i] << endl;
            }
        }else{
            //! some error
        }
    }




    //pre: 1st param is the line to be split
    //     2nd param is the the vector to put the split parts of the string into
    //
    //post: the vector passed in as the 2nd param will be populated with 1st params parts
    //      seperated by symbols
    //
    //desc: we take in a line of source code which has been split by spaces
    //      we then loop through it and splits its parts by symbols
    //      if we find a valid symbol of length two it will keep those together
    void splitLine(string lineIn, vector<string>& splitLine){
        string currentLex = "";
        for(int cCount = 0; cCount < lineIn.size(); cCount++){
            if(isalnum(lineIn[cCount])){
                currentLex += string(1, lineIn[cCount]);
            }else{
                if(currentLex != ""){
                    splitLine.push_back(currentLex);
                    currentLex = "";
                }
                if(cCount < (lineIn.size() - 1)){
                    if(getSymbolToken(lineIn.substr(cCount, 2)) != ""){
                        splitLine.push_back(lineIn.substr(cCount, 2));
                        cCount++;
                    }else{
                        splitLine.push_back(string(1, lineIn[cCount]));
                    }
                }else{
                    splitLine.push_back(string(1, lineIn[cCount]));
                }
            }
            if(cCount == (lineIn.size()-1) && currentLex != ""){
                splitLine.push_back(currentLex);
            }
        }
    }




    //pre: 1st param currentLex is the current lexeme that is being analyzed
    //     2nd param previousLex is the previous lexeme that was from right before
    //     3nd param stringOn is a reference to the stringOn passed in to the method, this value may encompass multiple method calls
    //     4rd param totalString is a reference to the totalString passed in, this value may encompass multiple method calls
    //     5th param error is a reference to the errors which may occur during the analysis
    //
    //post: if the currentLex is a valid lexeme then it will populate the lexemes and tokens vectors
    //      if there is a string then the stringOn and totalString will be altered accordingly
    //      if currentLex is invalid it will change the text of error
    //
    //desc: this method will check currentLex and determine if it is valid or not and if there is an error.
    //      It will also form strings which may take multiple lines of source code
    void analyzeLex(string currentLex, int lineNumber, bool& stringOn, string& totalString, vector<string>& error){
        if(stringOn){
            if(currentLex == "\""){
                stringOn = false;
                addLexemeTokenPair(totalString, "t_str");
                totalString = "";
            }else{
                totalString += currentLex;
            }
        }else{
            if(currentLex == "\""){
                stringOn = true;
            }else{
                if(getKeywordToken(currentLex) != ""){
                    addLexemeTokenPair(currentLex, "t_"+currentLex);
                }else{
                    if(checkIfStaringWithNumber(currentLex)){
                        if(checkIfAllNumber(currentLex)){
                            addLexemeTokenPair(currentLex, "t_int");
                        }else{
                            error.push_back("Error invalid identifier on line " + to_string(lineNumber) + ": " + currentLex);
                        }
                    }else{
                        if(checkIfThereAreSymbols(currentLex)){
                            if(getSymbolToken(currentLex) != ""){
                                addLexemeTokenPair(currentLex, getSymbolToken(currentLex));
                            }else if(currentLex != " "){
                                error.push_back("Error invalid symbol on line " + to_string(lineNumber) + ": " + currentLex);
                            }
                        }else{
                            addLexemeTokenPair(currentLex, "t_id");
                        }
                    }
                }
            }
        }
    }



    //pre: 1st param lexemeIn is the lexeme to add to the vector lexemes
    //     2nd param tokenIn is the token to be added to the vector tokens
    //
    //post: the vectors tokens and lexemes will be populated with one more value
    //
    //desc: this method will use the push_back method to add the parameters in to the vectors lexemes and tokens
    void addLexemeTokenPair(string lexemeIn, string tokenIn){
        lexemes.push_back(lexemeIn);
        tokens.push_back(tokenIn);
    }


    //pre: 1st param error is any errors that may have occurred during the analysis
    //     2nd param outfile is the output where the analysis will be printed
    //
    //post: outfile will have the analysis and errors printed to it
    //
    //desc: this method will first loop through all lexemes and tokens and print those to outfile
    //      it will then print out an error if there was any
    void printToOutput(vector<string>& errors, ostream& outfile){

        //lexemes should be the same length as tokens
        for(int tlCount = 0; tlCount < lexemes.size(); tlCount++){
            outfile << tokens[tlCount] << " : " << lexemes[tlCount] << endl;
        }

        for(int eCount = 0; eCount < errors.size(); eCount++){
            outfile << errors[eCount] << endl;
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

        int lineNumber = 1;

        string totalString;
        bool stringOn = false;

        vector<string> errors;

        while(getline(infile, currentLine) && errors.size() == 0){
            vector<string> splitString;
            splitLine(currentLine, splitString);

            for(int i = 0; i < splitString.size(); i++){
                analyzeLex(splitString[i], lineNumber, stringOn, totalString, errors);
            }

            lineNumber++;
        }

        if(stringOn){
            addLexemeTokenPair(totalString, "t_str");
            errors.push_back("Error string not closed");
        }

        printToOutput(errors, outfile);
    };
};




int main() {

    //gets the filename of the lexeme file
    //and checks if that file exists
    string lexemeFileNameInput = "tokenlexemedata.txt";
    // cout << "Enter the filename containing your token/lexeme pairs: ";
    // cin >> lexemeFileNameInput;

    ifstream lexemeFileIn(lexemeFileNameInput);
    if(!lexemeFileIn){
        cout << "Error Opening File " << lexemeFileNameInput << endl;
        exit(-1);
    }


    //gets the filename of the source code
    //and checks if that file exists
    string sourceFileNameInput = "test.txt";
    // cout << "Enter the filename containing your code: ";
    // cin >> sourceFileNameInput;

    ifstream sourceFileIn(sourceFileNameInput);
    if(!sourceFileIn){
        cout << "Error Opening File " << sourceFileNameInput << endl;
        exit(-1);
    }

    //gets the filename of the output file
    //and checks if that file exists
    string fileNameOut = "output.txt";
    // cout << "Enter the filename to output your lexical analysis: ";
    // cin >> fileNameOut;

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