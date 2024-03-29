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

    //pre: 1st param lexemeToCheck is the lexeme that will be checked for a matching token
    //post: this method will return the token if any match
    //desc: this method loops through all the token lexeme pairs and checks if it matches any of them
    //      if it does it will return the token
    //      if it does not match any it will return an empty string
    string getToken(const string& lexemeToCheck){
        map<string, string>::iterator mapStrStrIter;
        string returnToken = "";

        for(mapStrStrIter = tokenmap.begin(); mapStrStrIter != tokenmap.end(); mapStrStrIter++){
            if((mapStrStrIter->second) == lexemeToCheck){
                returnToken = (mapStrStrIter->first);
            }
        }
        return returnToken;
    }

    //pre: 1st param string to check if it only contains digits
    //post: returns a bool if the string only contains digit or not
    //desc: loops through the string and uses the isdigit function to check if any character is not a digit
    bool checkIfAllNumber(const string& stringToCheck){
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
    bool checkIfThereAreSymbols(const string& wordToCheck){
        bool symbolCheck = false;
        for(int charCount = 0; charCount < wordToCheck.size(); charCount++){
            if(!isalnum(wordToCheck[charCount])){
                symbolCheck = true;
            }
        }
        return symbolCheck;
    }

    //pre: 1st param is the line to be split
    //     2nd param is the reference of the vector to put the split parts of the string into
    //
    //post: the vector passed in as the 2nd param will be populated with 1st params parts
    //      seperated by symbols
    //
    //desc: we take in a line of source code
    //      we will then loop through it and splits its parts by symbols
    //      if we find a valid symbol of length two it will keep those together
    void splitLine(const string& lineIn, vector<string>& splitLine){
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
                    if(getToken(lineIn.substr(cCount, 2)) != ""){
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
    //     2nd param lineNumber is the current line that we are on
    //     3rd param stringStrartLine is the line that the string started on
    //               this value may encompass multiple method calls
    //     4th param stringOn is a reference to the stringOn passed in to the method
    //               this value may encompass multiple method calls
    //     5th param totalString is a reference to the totalString passed in
    //               this value may encompass multiple method calls
    //     6th param error is a reference to the errors which may occur during the analysis
    //               this will saved for later use
    //
    //post: if the currentLex is a valid lexeme then it will populate the lexemes and tokens vectors
    //      if there is a string then the stringOn and totalString will be altered accordingly
    //      if currentLex is invalid it will change the text of error
    //
    //desc: this method will check currentLex and determine if it is valid or not and if there is an error.
    //      It will also form strings which may take multiple lines of source code
    void analyzeLex(const string& currentLex, const int& lineNumber, int& stringStartLine, bool& stringOn, string& totalString, vector<string>& error){
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
                stringStartLine = lineNumber;
            }else{
                if(getToken(currentLex) != ""){
                    addLexemeTokenPair(currentLex, getToken(currentLex));
                }else{
                    if(isdigit(currentLex[0])){
                        if(checkIfAllNumber(currentLex)){
                            addLexemeTokenPair(currentLex, "t_int");
                        }else{
                            error.push_back("Error invalid identifier on line " + to_string(lineNumber) + ": " + currentLex);
                        }
                    }else{
                        if(checkIfThereAreSymbols(currentLex)){
                            if(currentLex != " "){
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
    void addLexemeTokenPair(const string& lexemeIn, const string& tokenIn){
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
    void printToOutput(const vector<string>& errors, ostream& outfile){

        //lexemes should be the same length as tokens
        for(int tlCount = 0; tlCount < lexemes.size(); tlCount++){
            outfile << tokens[tlCount] << " : " << lexemes[tlCount] << endl;
        }

        for(int eCount = 0; eCount < errors.size(); eCount++){
            outfile << errors[eCount] << endl;
            cout << errors[eCount] << endl;
        }
        if(errors.size() == 0){
            cout << "File was Scanned Completely" << endl;
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

    // pre: 1st parameter refers to an open text file that contains source code in the language
    //      2nd parameter refers to an open empty output file
    //
    // post: If no error, the token and lexeme pairs for the given input
    //       file have been written to the output file.  If there is an error,
    //       the incomplete token/lexeme pairs, as well as an error message have
    //       written to the output file.  A success or fail message has printed
    //       to the console.
    void scanFile(istream& infile, ostream& outfile){
        string currentLine;
        int lineNumber = 1;

        string totalString;
        bool stringOn = false;
        int stringStartLine;

        vector<string> errors;

        //gets each line of the source code
        //it then splits it by symbols
        //it then will loop through each lexeme
        while(getline(infile, currentLine) && errors.size() == 0){
            vector<string> splitString;
            splitLine(currentLine, splitString);

            for(int i = 0; i < splitString.size(); i++){
                analyzeLex(splitString[i], lineNumber, stringStartLine, stringOn, totalString, errors);
            }

            lineNumber++;
        }

        //if stringOn is still true then there was a missing closing quote "
        if(stringOn){
            addLexemeTokenPair(totalString, "t_str");
            errors.push_back("Error string not closed on line " + to_string(stringStartLine));
        }

        printToOutput(errors, outfile);
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


    LexAnalyzer lexAnal(lexemeFileIn);
    lexAnal.scanFile(sourceFileIn, fileOut);

    //closes the output file
    fileOut.close();

    return 0;
}