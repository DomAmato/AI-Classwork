#pragma once

#include <regex>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

/*

Written by Dom Amato 4/7/17

*/

using namespace std;

class Homework1 {
public:
	static pair<int, string> parseParagraphs(ifstream & file);

	static int parseSentences(string text);

	static vector<string> parseWords(string text);

	static map<string, int> parseWordFrequency(vector<string> words);
	static map<int, vector<string>> remapFrequency(map<string, int> freqMap);
private:
	static string removeTitles(string text) {
		vector<string> monikers;
		smatch stringMatcher;
		regex regexPattern("(?:[MDSJmdsj]r|[MDSJmdsj]rs|[MDSJmdsj]s|[Mm]iss|[Pp]hd)\\.");
		string textToParse = text;
		while (regex_search(textToParse, stringMatcher, regexPattern)) {
			monikers.push_back(stringMatcher.str());
			textToParse = stringMatcher.suffix().str();
		}
		for (auto moniker : monikers) {
			text.erase(text.find(moniker), moniker.length());
		}
		return text;
	}

	static string extractAcronyms(string text, vector<string> & acronyms) {
		smatch stringMatcher;
		regex regexPattern("\\b((?!![A-Za-z])([A-Za-z])\\.)+");
		string textToParse = text;
		while (regex_search(textToParse, stringMatcher, regexPattern)) {
			acronyms.push_back(stringMatcher.str());
			textToParse = stringMatcher.suffix().str();
		}
		for (auto acronym : acronyms) {
			text.erase(text.find(acronym), acronym.length());
		}

		return text;
	}

	static pair<string, string> parseContraction(string word) {
		auto subStr = word.substr(max(int(word.find_first_of("'") - 1), 0));
		string prefix;
		string suffix;
		if (subStr == "n't") {
			prefix = word.substr(0, word.find_first_of("'") - 1);
			suffix = "not";
		}
		else {
			prefix = word.substr(0, word.find_first_of("'"));
			subStr = subStr.substr(min(int(word.find_first_of("'") + 1), 2));
			if (subStr == "ll") {
				suffix = "will";
			}
			else if (subStr == "ve") {
				suffix = "have";
			}
			else if (subStr == "d") {
				suffix = "would";
			}
			else if (subStr == "re") {
				suffix = "are";
			}
			else if (subStr == "m") {
				suffix = "am";
			}
			else if (subStr == "s") {
				string lowerP = prefix;
				transform(lowerP.begin(), lowerP.end(), lowerP.begin(), ::tolower);
				if (lowerP == "he" || lowerP == "she" || lowerP == "it") {
					suffix = "is";
				}
				else {
					suffix = "'s";
				}
			}
		}
		return pair<string, string>(prefix, suffix);
	}
};

