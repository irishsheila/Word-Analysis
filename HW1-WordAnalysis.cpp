// ==========================================
// Created: August 31, 2016
// Sheila Doherty
// Homeword Assignment #1
// Instructor: Elizabeth Boese
// Description: Counts unique words in a file
// outputs the top N most common words
// ==========================================


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std; 


struct wordItem //creates the stuct & its variables
{
	string word;
	int count = 0;
};

void getStopWords (char *ignoreWordFileName, vector<string>& _vecIgnoreWords);
/*
 * Function name: getStopWords
 * Purpose: read stop word list from file and store into vector
 * 	@param ignoreWordFile - filename (the file storing ignore words)
 * 	@param _vecIgnoreWords - store ignore words from the file (pass by reference)
 * 	@return - none
 * Note: The number of words is fixed to 50
 */


bool isCommonWord (string word, vector<string>& _vecIgnoreWords);
/*
 * Function name: isCommonWord
 * Purpose: to see if a word is a common word
 * 	@param word - a word (which you want to check if it is a common word)
 * 	@param _vecIgnoreWords - the vector type of string storing ignore/common words
 * 	@return - true (if word is a common words, or false (otherwise))
*/


void printTopN (wordItem wordItemList[], int topN);
/*
 * Function name: printTopN
 * Purpose: to print the top N high frequency words
 * 	@param wordItemList - a pointer that points to an array of wordItems
 * 	@param topN - the number of top frequency words to print
 * 	@return - none
*/

bool isArrayWord (string currentWord, wordItem* &trackerArray, int arrayCounter);
/*
 * Function name: isArrayWord
 * Purpose: check array to see if a word is anywhere in the array
 * 	@param currentWord - the word to check the array for
 * 	@param wordItem* &trackerArray - a pointer that points to the address of the array of wordItems
 * 	@return - true (if word is in the array, or false (otherwise))
*/

void dblArraySize (struct wordItem* &trackerArray, int arrayCounter, int arrayMax);
/*
 * Function name: dblArraySize
 * Purpose: to dynamically double an array if it is full
 * 	@param wordItem* &trackerArray - a pointer that points to the address of the array
 * 	@param arrayCounter - the current size of the array
 *  @param arrayMax - the new size of the array
 * 	@return - none
*/

void BubbleSortArray (wordItem* &trackerArray, int arrayCounter);
/*
 * Function name: BubbleSortArray
 * Purpose: to sort the array from greatest to least value
 * 	@param wordItem* &trackerArray - a pointer that points to the address of the array
 * 	@param arrayCounter - the current size of the array
 * 	@return - none
*/

void getTotalNumberNonCommmonWords (int totalWords);
/*
 * Function name: getTotalNumberNonCommonWords
 * Purpose: prints out the total number of non-common words
 * 	@param totalWords: the amount of non-common words
 * 	@return - none
*/


const int STOPWORD_LIST_SIZE = 50; //limits the stopwords list size to 50



int main(int argc, char* argv[]) 
{
	 vector<string> _vecIgnoreWords(STOPWORD_LIST_SIZE);

	 if (argc != 4) //if 4 arguments aren't entered, generates this message & ends program
     {
         cout << "Usage: ";
         cout << argv[0] << " <number of words> <filename.txt> <ignorefilename.txt>";
         cout << endl;
         return 0;
     }

	 getStopWords (argv[3], _vecIgnoreWords); //calls function that loades the vector with the text file words

	 ifstream inToFile;
     string placeholder;
     int arrayMax = 100;
     int arrayCounter = 0;
     int doubledArrayCounter = 0;
     int totalNonCommon = 0;
     int wordsToOutput = atoi(argv[1]);

     wordItem* trackerArray = new wordItem[arrayMax];  //pointer

     inToFile.open (argv[2]); //opens file
     
     if (inToFile.is_open())
     {
        cout << "File to analyze is open!" << endl;

        while (inToFile >> placeholder) //accesses file word by word
        {
            if ((isCommonWord(placeholder, _vecIgnoreWords) == false) && (isArrayWord(placeholder, trackerArray, arrayCounter) == false)) //if word is not in the stopwords vector or in the array already
            {
                totalNonCommon = totalNonCommon + 1; //adds one to the total number of non-common words
                if (arrayMax > arrayCounter) //checks to see if there is enough room in the array
                {
                    trackerArray[arrayCounter].word = placeholder; //sets the word in the array
                    trackerArray[arrayCounter].count = trackerArray[arrayCounter].count + 1; //adds 1 to the count in the array
                }
                else //if there isn't enough room in the array, then double the array first
                {
                    arrayMax = arrayMax * 2;
                    doubledArrayCounter = doubledArrayCounter + 1; //adds 1 to the number of times the array is doubled

                    dblArraySize(trackerArray, arrayCounter, arrayMax); //calls the function to double the array

                    trackerArray[arrayCounter].word = placeholder; //onces array is doubled, sets the next word in the array
                    trackerArray[arrayCounter].count = trackerArray[arrayCounter].count + 1; //adds 1 to the count in the array
                }
                arrayCounter = arrayCounter + 1; //adds 1 to the arrayCounter to track where the next word needs to be input
            }
            else if((isCommonWord(placeholder, _vecIgnoreWords) == false) && (isArrayWord(placeholder, trackerArray, arrayMax) == true)) //if the word is not in the vector, but it is already in the array
            {
                totalNonCommon = totalNonCommon + 1; //adds 1 to the total non-commons words counter
                for (int i = 0; i < arrayCounter; i++) //searchs the array for the word and adds 1 to the counter of the word
                    {
                        if (placeholder == trackerArray[i].word)
                        {
                            trackerArray[i].count = trackerArray[i].count + 1;
                        }
                    }
            }
        }
        inToFile.close();
     }
     else
     {
         cout << "File not opened! Try again!" << endl;
     }

     BubbleSortArray(trackerArray, arrayCounter); //calls the function to sort the array from largest to smallest value

     printTopN(trackerArray, wordsToOutput); //prints the topN words as assigned in the call to main

	 cout << "#" << endl;
     cout << "Array doubled: " <<  doubledArrayCounter << endl;
     cout << "#" << endl;
     cout << "Unique non-common words: " << arrayCounter << endl;
     cout << "#" << endl;
     getTotalNumberNonCommmonWords(totalNonCommon);

    return 0;
 }

 void getStopWords (char *ignoreWordFileName, vector<string>& _vecIgnoreWords)
{
     ifstream inFile;
     string data;
     inFile.open (ignoreWordFileName); //opens text file
     if (inFile.is_open())
     {
        cout << "Stop Words is open!" << endl;
        while(inFile >> data) //loads file word by word into the vector
        {
            _vecIgnoreWords.push_back(data); //helped from YouTube video on vectors: programminghelporg
        }
        inFile.close();
     }
     else
     {
         cout << "Stop Words File not opened! Try again!" << endl;
     }
    return;
 }

 bool isCommonWord (string word, vector<string>& _vecIgnoreWords) //searches the list from beginning to end for the word
 {
    if (find(_vecIgnoreWords.begin(), _vecIgnoreWords.end(), word) !=_vecIgnoreWords.end()) //help from cplusplus.com
    {
        return true;
    }
    else
    {
        return false;
    }
 }

bool isArrayWord (string currentWord, wordItem* &trackerArray, int arrayCounter) //help from cplusplus.com
 {
    for (int i = 0; i < arrayCounter; i++) //searches the array from beginning and returns true if found
    {
        if (trackerArray[i].word == currentWord)
        {
            return true;
        }
    }
    return false;
 }

void dblArraySize (struct wordItem* &trackerArray, int arrayCounter, int arrayMax) //helped from http://stackoverflow.com/questions/35108584/array-of-structs-doubling-algorithm-c
{
    wordItem* temp = new wordItem[arrayMax]; //creates new bigger array - helped from http://www.fredosaurus.com/notes-cpp/newdelete/55dynexample.html

    for (int x = 0; x < arrayCounter; x++)
    {
        temp[x] = trackerArray[x]; //copies the old array values into the new array
    }

    delete [] trackerArray; //frees old array memory
    trackerArray = temp; //now trackerArray points to the new array
}

void BubbleSortArray (wordItem* &trackerArray, int arrayCounter) //code adapted from http://mathbits.com/MathBits/CompSci/Arrays/Bubble.htm
{
    int i, j, flag = 1;
    wordItem temp;
    int numLength = arrayCounter; //part of filled array to sort
    for (i = 1; (i <= numLength) && flag; i++)
    {
        flag = 0;
        for (j=0; j < (numLength-1); j++)
        {
            if (trackerArray[j+1].count>trackerArray[j].count) //switches ascending order if the right number is larger than the left
            {
                temp = trackerArray[j];
                trackerArray[j] = trackerArray[j+1];
                trackerArray[j+1]=temp;
                flag = 1; //indicates that a swap occured
            }
        }
    }
    return;
}

void printTopN (wordItem wordItemList[], int topN)
{
    for (int m = 0; m < topN; m++)
    {
         cout << wordItemList[m].count << " - " << wordItemList[m].word << endl;
    }
}

void getTotalNumberNonCommmonWords (int totalWords)
{
    cout <<"Total non-common words: " << totalWords << endl;
}
