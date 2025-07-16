# bucket sort

# top k frequent elements

# heap or bucket sort

# neetcode.io

/*

# document = "Practice makes perfect. you'll only

# get Perfect by practice. just practice!"

  
  

# [ ["practice", "3"], ["perfect", "2"],

# ["makes", "1"], ["youll", "1"], ["only", "1"],

# ["get", "1"], ["by", "1"], ["just", "1"] ]

  

# {{"practice": 3}, {"make": 1}, {"perfect": 2}, {"youll", "1"}, ...}

# {:{3 "practice", "hello"}, {1: "make"}, {2: "perfect"}, {1: "youll"}, ...}

# result.append({currentLargestPair.value: .key})

"""

result.append(largest)

length = dic.size

while length:

if dic[result[-1].value-1] exist and dic[result[-1].value-1] > 0:

result.append(dic[result[-1].value-1]...)

dic[result[-1].value-1] = -dic[result[-1].value-1]

length--

"""

*/

  

#include <iostream>

#include <string>

#include <vector>

  

using namespace std;

  

#include <string>

#include <algorithm>

#include <cctype>

  

#include <string>

#include <vector>

#include <algorithm>

#include <cctype>

  

std::vector<std::string> cleanAndSeparateWords(const std::string& text) {

std::vector<std::string> words;

std::string currentWord;

for (char c : text) {

if (std::isalnum(c)) {

currentWord += std::tolower(c);

} else if (!currentWord.empty()) {

// Hit a separator and we have a word

words.push_back(currentWord);

currentWord.clear();

}

}

// Don't forget the last word if text doesn't end with separator

if (!currentWord.empty()) {

words.push_back(currentWord);

}

return words;

}

  
  

// For your wordCountEngine function context:

vector<vector<string>> wordCountEngine(const string& document) {

// Clean the document

std::vector<std::string> cleanedList = cleanAndSeparateWords(document);

  

std::map<string, int> countMap {};

for (auto w: cleanedList)

{

countMap[w] += 1;

}

  

int largest = 0;

std::map<int, std::vector> countMapR {};

for (auto pair: countMap)

{

if (pair.second > largest)

{

largest = pair.second;

}

countMapR[pair.second] = pair.first;

}

  

std::vector<std::pair<std::string, std::string>> result();

int current = 0;

{3: [hello. how], 1: no}

  

while (largest > 0)

{

if (countMapR.find(largest) != countMapR.end() )

{

result.insert({countMapR[largest], std::to_String(largest)})

  

}

largest--;

}

  
  

return result;

}

  

int main() {

return 0;

}