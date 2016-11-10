# Word-Analysis
Array Doubling with Dynamic Memory Assignment for CSCI-2270

CSCI-2270
Assignment 1 UPDATE Fri Sep 2 5pm in YELLOW
Instructor: Boese
Word Analysis
Array Doubling with Dynamic Memory
Objectives
• Read a file with unknown size and store in a vector
• Loop through a vector
• Store, search, and iterate through data in an array of struct
• Use array doubling via dynamic memory to increase the size of the array
Background
There are several fields in computer science that aim to understand how people use
language. This can include analyzing the most frequently used words by certain
authors, and then going one step further to ask a question such as: “Given what we
know about Hemingway’s language patterns, do we believe Hemingway wrote this
lost manuscript?”
In this assignment, we’re going to do a basic introduction to document analysis by
determining the number of unique words and the most frequently used words in
two documents. If you enjoy this, take elective courses on Natural-Language Processing.
What your program needs to do
There is one test file on the website – HW1-HungerGames_edit.txt that contain the
full text from Hunger Games Book 1. We have pre-processed the file to remove all
punctuation and down-cased all words. We will test on a different file!
Your program will calculate the following information on any text file:
• The top n words (n is also a command-line argument) and the number of
times each word was found
• The total number of unique words in the file
• The total number of words in the file
• The number of array doublings needed to store all unique words in the file
Example:
Your program takes three command-line arguments: the number of most common
words to print out, the name of the file to process, and the stop word list file.
Running your program using:
./a.out 10 HW1-HungerGames_edit.txt HW1-ignoreWords.txt
would return the 10 most common words in the file HW1-HungerGames_edit.txt and
should produce the following results:
682 - is
492 - peeta
479 - its
431 - im
427 - can
414 - says
379 - him
368 - when
367 - no
356 - are
#
Array doubled: 7
#
Unique non-common words: 7682
#
Total non-common words: 59157
Program Specifications
The following are requirements for your program:
• Read in the number of most common words to process from the first
command-line argument.
• Read in the name of the file to process from the second command-line
argument.
• Write a function named getStopwords that takes the name of the file and a
reference to a vector as parameters (returns void). Read in the file for a list of
the top 50 most common words to ignore (Table 1).
These are commonly referred to as ‘stopwords’ in NLP (Natural Language
Processing). (Create this file yourself)
o The file will have one word per line, and always have exactly 50 words
in the file.
o Your function will update the vector passed to it with a list of the
words from the file.
• Store the unique words found in the file that are not in the stopword list in a
dynamically allocated array.
o Call a function to check if the word is a stopword first, and then ignore
it.
o Use an array of structs to store each unique word (variable name
word) and a count (variable name count) of how many times it
appears in the text file.
o Use the array-doubling algorithm to increase the size of your
array
§ We don’t know ahead of time how many unique words the
input file will have, so you don’t know how big the array should
be. Start with an array size of 100, and double the size as
words are read in from the file and the array fills up with new
words. Use dynamic memory allocation to create your array,
copy the values from the current array into the new array, and
then free the memory used for the current array.
• Output the top n most frequent words
Write a function named printTopN that takes a reference to the array of
structs and the value of n to determine the top n words in the array.
Generate an array of the n top items sorted from most frequent to least
frequent and print these out from most to least.
Hint: sort the list before calling printTopN.
• Output the number of times you had to double the array.
• Output the number of unique non-common words.
• Output the total number of non-common words.
o Write a function named getTotalNumberNonCommonWords that
takes a reference to your array of unique non-common words and
returns the total number of words found.
• Format your output the following way (and reference the example above).
o When you output the top n words in the file, the output needs to be in
order, with the most frequent word printed first. The format for the
output needs to be:
Count - Word
#
Array doubled: <number of array doublings>
#
Unique non-common words: <number of unique words>
#
Total non-common words: <total number of words>
• Generate the output with these commands:
cout << numCount << ” – “ << word << endl;
cout << ”#” << endl;
cout << ”Array doubled: “ << numDoublings << endl;
cout << ”#”<<endl;
cout << ”Unique non-common words: “ << numUniqueWords;
cout << “#” << endl;
cout << “Total non-common words: “;
cout << getTotalNumberNonCommonWords(yourarray) << endl;
• Include a comment block at the top of the .cpp file with your name,
assignment number, date and course instructor.
• Make sure your code is commented enough to describe what it is doing.
Note: some of you might wonder why we’re using array doubling instead of using C++
Vectors for storing the uncommon words. In this assignment, you’re doing what
happens behind-the-scenes with a Vector.
• Use the following starter code (type it in so you get more practice setting up
programs):
// ==========================================
// Created: August 23, 2016
// @author
//
// Description: Counts unique words in a file
// outputs the top N most common words
// ==========================================
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
using namespace std;
// struct to store word + count combinations
struct wordItem
{
string word;
int count;
};
/*
* Function name: getStopWords
* Purpose: read stop word list from file and store into vector
* @param ignoreWordFile - filename (the file storing ignore words)
* @param _vecIgnoreWords - store ignore words from the file (pass by reference)
* @return - none
* Note: The number of words is fixed to 50
*/
void getStopWords(char *ignoreWordFileName, vector<string>& _vecIgnoreWords);
/*
* Function name: isCommonWord
* Purpose: to see if a word is a common word
* @param word - a word (which you want to check if it is a common word)
* @param _vecIgnoreWords - the vector type of string storing
ignore/common words
* @return - true (if word is a common word), or false (otherwise)
*/
bool isCommonWord(string word, vector<string>& _vecIgnoreWords);
/*
* Function name: printTopN
* Purpose: to print the top N high frequency words
* @param wordItemList - a pointer that points to an array of wordItems
* @param topN - the number of top frequency words to print
* @return none
*/
void printTopN(wordItem wordItemList[], int topN);
const int STOPWORD_LIST_SIZE = 50;
// ./a.out 10 HW1-HungerGames_edit.txt HW1-ignoreWords.txt
int main(int argc, char* argv[])
{
vector<string> vecIgnoreWords(STOPWORD_LIST_SIZE);
// verify we have the correct # of parameters, else throw error msg & return
if (argc != 4)
{
cout << "Usage: ";
cout << argv[0] << " <number of words> <filename.txt> <ignorefilename.txt>";
cout << endl;
return 0;
}
/* **********************************
1. Implement your code here.
Read words from HW1-HungerGames_edit.txt, store and count the words.
2. Implement the three functions after the main() function separately.
********************************** */
return 0;
}
void getStopWords(char *ignoreWordFileName, vector<string>& _vecIgnoreWords)
{
return;
}
bool isCommonWord(string word, vector<string>& _vecIgnoreWords)
{
return true;
}
void printTopN(wordItem wordItemList[], int topN)
{
return;
}
Submitting Your Code:
Submit your assignment with the file HW1-WordAnalysis.cpp as a zip file named
HW1_[yourFirstName]_[yourLastName].zip to Moodle.
