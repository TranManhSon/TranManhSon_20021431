#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <cctype>
#include <fstream>
#include <thread>
#include <chrono>
#include <algorithm>
#include "hm_until.h"
#include <SDL_mixer.h>

using namespace std;
const int MAX_BAD_GUESSES = 7;
vector<string> wordListFull;

void Topic::readFile( ifstream &file )
{
        string wordL;
        string nameT;
        getline(file, nameT, ':');
        nameT = deleteEndl(nameT);
        setNameTopic(nameT);
        for(int i = 0 ; i < 40 ; i++){
            file >> wordL;
            setWordList( wordL, i);
            wordListFull.push_back(wordL);
        }
}

int main(int argc, char *argv[])
{
    Topic arr[10];
    readWordList(fileName, arr, wordListFull);
    srand(time(0));
    int totalScore = 0;
    int x;
    while(true)
    {
        string word = chooseWord(arr, wordListFull);
        if (word.empty()) {
            cout << "No word available " << endl;
            return 0;
        }
        for(int i = 0; i < wordListFull.size(); i++){
                if( word == normalize(wordListFull[i])){
                    x = i;
                    break;
                }
        }
        string guessedWord = string(word.length(), '-');
        int badGuessCount = 0;
        string badGuess;
        do{
            renderGame(guessedWord,badGuessCount,badGuess, arr, x);
            char guess = readAGuess();
            if( guess == '1' ){
                if( totalScore > 0){
                    int y;
                    cin >> y;
                    choosePosSuggest( y, word ,guessedWord);
                    update(guessedWord, word, guess);
                    totalScore--;
                }
                else continue;
            }
            if( contains(word, guess))
                update(guessedWord,word,guess);
            else {
                badGuessCount++;
                badGuess += guess;
            }
        }
        while(badGuessCount < MAX_BAD_GUESSES && word != guessedWord);
        if( badGuessCount < 7 ){
            totalScore++;
        }
        printGameOverInfo(guessedWord, word, badGuessCount,badGuess, arr, x );
        cout << "Do you want more (Y/N) ?";
        string answer;
        cin.ignore(1);
        getline(cin, answer);
        if (answer != "Y" && answer != "y")
            break;
    }
    return 0;
}
