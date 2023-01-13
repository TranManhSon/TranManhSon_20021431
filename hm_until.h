#ifndef	VECTOR_H
#define	VECTOR_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cctype>
#include <fstream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <SDL.h>
#include <windows.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

using namespace std;

static Mix_Chunk* sound_guess = NULL;
//static Mix_Chunk* sound_win = NULL;

const char fileName[] = "file10Topic.txt";

//xet tu va chu de
struct Topic{
    string nameTopic;
    string wordListTopic[40];
    void readFile( ifstream &file);
};

void choosePosSuggest(int x,const string& word , string& guessedWord);
void renderGame(string guessedWord, int badGuessCount, string badGuess, Topic arr[], int x);
void readWordList( const string& fileName, Topic arr[], vector<string> wordListFull);
void printGameOverInfo(string guessedWord, string word, int badGuessCount, string badGuess, Topic arr[], int x);
void update(string& guessedWord, const string& word, char guess);
string chooseWord(Topic arr[], vector<string> wordListFull);
string getHangPicture(int badGuessCount);
bool contains(string word, char guess);
string normalize(const string& s);
string deleteEndl(string s);
void textColor(int x);
void clearScreen();
char readAGuess();
bool Init();

#endif
