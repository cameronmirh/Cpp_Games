// Cameron Mirhossaini

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "strlib.h"
#include "lexicon.h"
#include "filelib.h"
#include "queue.h"

using namespace std;

void printIntro();
Lexicon createLexicon(string);
bool isLegalWord(string, string, Lexicon&);
Stack<string> createWordLadder(string, string, Lexicon&);
Set<string> createPossibleWords(Stack<string>&, Lexicon&, Set<string>&);
string printPath(Stack<string>&);


int main() {
    printIntro();
    string file = getLine("Dictionary file name? ");
    while(!fileExists(file)) {
        cout << "Invalid File. Try again." <<endl;
        file = getLine("Dictionary file name? ");
        cout << "" << endl;
    }
    Lexicon dictionary = Lexicon(file);
    string word1 = toLowerCase(getLine("Word #1 (or Enter to quit): " ));
    string word2 = toLowerCase(getLine("Word #2 (or Enter to quit): " ));

    while(word1 != "quit" && word1 != "" && word2 != "quit" && word2 != "") {

        cout << "" << endl;
        if (isLegalWord(word1, word2, dictionary)) {
            Stack<string> thePathway = createWordLadder(word1, word2, dictionary);

            if(thePathway.isEmpty()) {
                cout << "There was no viable pathway." << endl;
            } else {
                cout << "A ladder from " << word2 << " back to " << word1 << " :" << endl;
                cout << printPath(thePathway) << endl;
            }
            cout << "" << endl;
        }

        word1 = toLowerCase(getLine("Word #1 (or Enter to quit): " ));
        word2 = toLowerCase(getLine("Word #2 (or Enter to quit): " ));

    }
    cout << "Have a nice day." << endl;
    return 0;
}



string printPath(Stack<string> & thePathway) {
    string ladder;
    int stackSize = thePathway.size();
    for(int i = 0; i < stackSize; i++) {
        string word = thePathway.pop();
        ladder += word + " ";
    }
    return ladder;
}


Stack<string> createWordLadder(string word1, string word2, Lexicon & dictionary) {
    Set<string> allWordSet;
    Queue<Stack<string>> pathQueue;
    Stack<string> first;
    first.push(word1);
    pathQueue.enqueue(first);
    while(!pathQueue.isEmpty()) {
        //dequeue pathway
        Stack<string> potentialPath = pathQueue.dequeue();

        //create list of all possible pathways
        Set<string> potentialPathSet = createPossibleWords(potentialPath, dictionary, allWordSet);

        //checks each pathway
        for(string ladderWord : potentialPathSet) {
            if(ladderWord == word2) {
                potentialPath.push(ladderWord);
                return potentialPath;
            } else {
                Stack<string> copyStack = potentialPath;
                copyStack.push(ladderWord);
                pathQueue.enqueue(copyStack);
            }
        }


    }

    Stack<string> noStack;
    return noStack;
}



//creates set of all possible english words
Set<string> createPossibleWords(Stack<string> & potentialPath, Lexicon & dictionary, Set<string> & allWordSet) {
    Set<string> potentialPathSet;
    string ladderWord = potentialPath.peek();

    for(int i = 0; i < ladderWord.size(); i++) {
        for(char letter = 'a'; letter <= 'z'; letter++) {
            ladderWord = potentialPath.peek();
            ladderWord[i] = letter;
            if(dictionary.contains(ladderWord) && !allWordSet.contains(ladderWord)) {
                potentialPathSet.add(ladderWord);
                allWordSet.add(ladderWord);
            }
        }
    }

    return potentialPathSet;

}


//checks if words comply with rules
bool isLegalWord(string word1, string word2, Lexicon & dictionary) {
    if(word1.length() != word2.length()) {
        cout << "The two words must be the same length." << endl;
        return false;
    }

    if(!dictionary.contains(word1) || !dictionary.contains(word2)) {
        cout << "The two words must be found in the dictionary." << endl;
        return false;
    }

    if(word1 == word2) {
        cout << "The two words must be different." <<endl;
        return false;
    }

    return true;
}



void printIntro() {
    cout << "Welcome to CS 106B Word Ladder." <<endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << "" << endl;
}
