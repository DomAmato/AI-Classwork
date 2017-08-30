#include "Homework1.h"

/*

Written by Dom Amato 4/7/17

*/

pair<int, string> Homework1::parseParagraphs(ifstream & file) {
	string text = "";
	int counter = 0;
	while (!file.eof()) {
		string line;
		getline(file, line);
		if (line.length() == 0) {
			counter++;
		}
		text.append(line + '\n');
	}
	return pair<int, string>(counter, text);
}

int Homework1::parseSentences(string text) {
	int counter = 0;
	smatch stringMatcher;
	regex regexPattern("\\b[)]*[.!?]+[\"]*( |\n|\r)");
	string textToParse = removeTitles(text);
	while (regex_search(textToParse, stringMatcher, regexPattern)) {
		counter++;
		textToParse = stringMatcher.suffix().str();
	}
	return counter;
}

vector<string> Homework1::parseWords(string text) {
	vector<string> words;
	smatch stringMatcher;
	regex regexPattern("(((\\w+[\\w',.-]*)\\b)|([^\\s\\w]))");
	string textToParse = extractAcronyms(text, words);
	while (regex_search(textToParse, stringMatcher, regexPattern)) {
		if (stringMatcher.str().find("'") != string::npos && stringMatcher.str().length() > 1) {
			auto parsedC = parseContraction(stringMatcher.str());
			words.push_back(parsedC.first);
			words.push_back(parsedC.second);
		}
		else {
			words.push_back(stringMatcher.str());
		}
		textToParse = stringMatcher.suffix().str();
	}
	return words;
}

map<string, int> Homework1::parseWordFrequency(vector<string> words) {
	map<string, int> frequencyMap;
	for (auto word : words) {
		if (frequencyMap.count(word) > 0) {
			frequencyMap.at(word)++;
		}
		else {
			frequencyMap.emplace(word, 1);
		}
	}
	return frequencyMap;
}

map<int, vector<string>> Homework1::remapFrequency(map<string, int> freqMap) {
	map<int, vector<string>> frequencyMap;

	for (auto entry : freqMap) {
		if (frequencyMap.count(entry.second) > 0) {
			frequencyMap.at(entry.second).push_back(entry.first);
		}
		else {
			vector<string> v;
			v.push_back(entry.first);
			frequencyMap.emplace(entry.second, v);
		}
	}

	return frequencyMap;
}

