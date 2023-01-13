#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cctype>
#include <fstream>
#include <thread>
#include <chrono>
#include <windows.h>
#include <SDL.h>
#include <algorithm>
#include <SDL_image.h>
#include "hm_until.h"
#include "SDL.h"
#include <SDL_mixer.h>

using namespace std;

//ad comment


void Topic::setWordList( string  wordL, int i){
       this->wordListTopic[i] = wordL;
}

bool Init(){ //ham am thanh
    if( SDL_Init(SDL_INIT_EVERYTHING) == -1){
        return false;
    }

    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096)  == -1)
            return false;
    sound_guess =  Mix_LoadWAV("guess.wav");
    sound_lose = Mix_LoadWAV("Cry.wav");
    //sound_win = Mix_LoadWAV("Laugh.wav");

    if( sound_lose == NULL || sound_guess == NULL ){
        return false;
    }
    return true;
}

string normalize(const string& s)
{
	string res = s ;
	transform(s.begin(), s.end(), res.begin(), ::tolower);
	return res;
}

void textColor(int x) //ham mau
{
    HANDLE color;
    color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute( color, x);
}

void readWordList( const string& fileName, Topic arr[], vector<string> wordListFull){ // ham doc tu va chu de
    ifstream file(fileName);
    if (file.is_open())
    {
        int n;
        file >> n;
        char ss[5];
        file.getline( ss, 3);
        for(int j = 0; j < n; j++){
            arr[j].readFile(file);
        }
    }
    file.close();
}

string chooseWord(Topic arr[], vector<string> wordListFull)
{
    if (wordListFull.size() > 0) {
        int randomIndex = rand() % wordListFull.size();
        return normalize(wordListFull[randomIndex]);
    }
    else return "";
}

void choosePosSuggest(int x,const string& word , string& guessedWord) // ham goi y
{
    guessedWord[x-1] = word[x-1];
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#elif __linux__
    system("clear");
#endif
}

char readAGuess()
{
    char guess;
    cout << "Please enter your next guess: ";
    cin >> guess;
    Init();
    Mix_PlayChannel(-1, sound_guess, 0);
    return guess;
}

bool contains(string word, char guess)
{
    return (word.find(guess) != string::npos);
}

void update(string& guessedWord, const string& word, char guess)
{
    int n = guessedWord.length();
    for (int i = 0; i < n; i++)
    {
        if (word[i] == guess)
            guessedWord[i] = guess;
    }
}

void printGameOverInfo(string guessedWord, string word, int badGuessCount, string badGuess, Topic arr[], int x)
{
    vector<string> hanged;
    renderGame(guessedWord, badGuessCount , badGuess ,arr , x);
    if (guessedWord != word) {
        hanged = {
"   ------------\n"
"   |         /\n"
"   |        O\n"
"   |       /|\\\n"
"   |       / \\\n"
"   |         \n"
"   |         \n"
"   |         \n"
"   |         \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\\\n"
"   |         / \\\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |           \\\n"
"   |            O\n"
"   |           /|\\\n"
"   |           / \\\n"
"   |             \n"
"   |             \n"
"   |             \n"
"   |             \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\\\n"
"   |         / \\\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        "
};
        int n = 0;
        Init();
        Mix_PlayChannel(-1,sound_lose, 0)         ;
        while (true) {
            clearScreen();
            cout << "Game Over!!! You are hanged" << endl
                 << "The correct word is " << word << endl;
            textColor(2);
            cout << hanged[n];
            n = (n+1) % hanged.size();
            std::this_thread::sleep_for (std::chrono::milliseconds(400/badGuessCount));//tang toc do hangman theo so luong tu sai
        }
    }
}

void renderGame(string guessedWord, int badGuessCount, string badGuess, Topic arr[], int x)
{
    clearScreen();
    cout << "Current guess = " << guessedWord << endl;
    cout << "Topic : " << arr[x/40].nameTopic << endl;
    cout << endl;
    cout << "Suggest, press : 1."<< endl;
    cout << "\nYou have made bad " << badGuessCount << " guess(es)"
         << (badGuess.empty() ? "" : ": ") << badGuess << endl
         << getHangPicture(badGuessCount) << endl;
}

string deleteEndl( string s){
    int n = s.size();
    if( s[0]=='\n'){
        for(int i = 1; i<n; i++){
            s[i-1] = s[i];
        }
    }
    s[n-1] = '\0';
    return s;
}

static string figure[] = {
"          O \n"
"         /|\\ \n"
"         | | \n",
"          O \n"
"         /|\\ \n"
"         / \\ \n",
"        __O__ \n"
"          | \n"
"         / \\ \n",
"        \\O/ \n"
"          | \n"
"         / \\ \n",
"        __O__ \n"
"          | \n"
"         / \\ \n",
"          O \n"
"         /|\\ \n"
"         / \\ \n" ,

};

string getHangPicture(int badGuessCount)
{
    string picture[] = {"",
"   ------------\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |          |\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\\\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\\\n"
"   |         / \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\\\n"
"   |         / \\\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",

};
    return picture[badGuessCount];
}
