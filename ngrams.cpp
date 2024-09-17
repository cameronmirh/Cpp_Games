// Cameron Mirhossaini

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "strlib.h"
#include "queue.h"
#include "map.h"
#include "random.h"


//bugs:
//x remainder when getting all words
//wrapping at end of the file

using namespace std;

void printIntro();
Map<Queue<string>, Vector<string>> createMap(string&, int&);
string createFirstQueue(ifstream &, string, int,  Queue<string> &);
void addToMap(Map<Queue<string>, Vector<string>> &,  Queue<string> &, string);
int printRandomWords(Map<Queue<string>, Vector<string> > &, int);
void constructString(int, int , Map<Queue<string>, Vector<string>> &);




int main() {
    printIntro();
    string file = getLine("Input file name? ");
    while(!fileExists(file)) {
        cout << "Invalid File. Try again." <<endl;
        file = getLine("Input file name? ");
        cout << "" << endl;
    }
    int N = getInteger("Value of N? ");
    while(N < 2) {
        cout << "Invalid File. Try again." <<endl;
        N = getInteger("Value of N? ");
        cout << "" << endl;
    }

    Map<Queue<string>, Vector<string>> probabilityMap = createMap(file, N);
   int x = printRandomWords(probabilityMap, N);
   while(x != 0) {
       x = printRandomWords(probabilityMap, N);
   }

    cout << "Exiting." << endl;
    return 0;
}

//finds first window randomly
int printRandomWords( Map<Queue<string>, Vector<string>> & probabilityMap, int N) {
    int numWords = getInteger("# of random words to generate (0 to quit)?");
    if(numWords == 0) return 0;
    constructString(numWords, N, probabilityMap);
    return -1;
}

//adds window and key to string and prints
void constructString(int numWords, int N, Map<Queue<string>, Vector<string>> & probabilityMap) {
    int itrtns = numWords / N;
    string line = "...";
    for(int i = 0; i < itrtns; i++) {
        Vector<Queue<string>> keys = probabilityMap.keys();
        int index = randomInteger(0, keys.size() - 1);
        Queue<string> prefixQueue = keys.get(index);
        Queue<string> prefixQueueCopy = prefixQueue;
        string prefix;
        for(int m = 0; m < N - 1; m++) {
            prefix = prefix + prefixQueue.dequeue() + " ";
        }
        Vector<string> values = probabilityMap.get(prefixQueueCopy);
        index = randomInteger(0, values.size() - 1);
        string suffix = values[index];
        line = line + prefix + suffix + " ";
    }


//adds remainder of words
    if(numWords % N != 0) {
        int remainder = numWords % N;
        int counter = 0;
        Vector<Queue<string>> keys = probabilityMap.keys();
        int index = randomInteger(0, keys.size() - 1);
        Queue<string> prefixQueue = keys.get(index);
        string prefix;
        while(counter < remainder) {
            line = line + prefixQueue.dequeue() + " ";
            counter++;
        }
    }
    cout << line + "..." << endl;
    cout << "" << endl;
}

//compiles text and compiles it into map
Map<Queue<string>, Vector<string>> createMap(string & file, int & N) {
    Map<Queue<string>, Vector<string>> probabilityMap;
    ifstream stream;
    Queue<string> firstWindow;
    string firstWord = createFirstQueue(stream, file, N, firstWindow);

    //use this copy for wrapping
    Queue<string> copyWindow = firstWindow;
    addToMap(probabilityMap, copyWindow, firstWord);

    //adds rest of words to map
    string word;
    while(stream >> word) {
        addToMap(probabilityMap, copyWindow, word);
    }

    //this is where wrapping occurs
    Queue<string> lastQueue;
    lastQueue.enqueue(word);

    for(int i = 0; i < N - 2; i++) {
        lastQueue.enqueue(firstWindow.dequeue());
    }
    addToMap(probabilityMap, lastQueue, word);
    return probabilityMap;

}



//adds current window and word to map
//removes last item on queue
//adds latest item to queue
void addToMap(Map<Queue<string>, Vector<string>> & probabilityMap,  Queue<string> & window, string word) {
    if(probabilityMap.containsKey(window)) {
        Vector<string> wordList = probabilityMap.get(window);
        wordList.add(word);
        probabilityMap.add(window, wordList);
    } else {
        Vector<string> wordList;
        wordList.add(word);
        probabilityMap.add(window, wordList);
    }
    window.dequeue();
    window.enqueue(word);
}




//creates the first window of the document
string createFirstQueue(ifstream & stream, string file, int N,  Queue<string> & firstWindow) {
    openFile(stream, file);
    for(int i = 0; i < N - 1; i++) {
        string word;
        stream >> word;
        firstWindow.enqueue(word);
    }
    string word;
    stream >> word;

    return word;
}


void printIntro() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << "" << endl;
}
