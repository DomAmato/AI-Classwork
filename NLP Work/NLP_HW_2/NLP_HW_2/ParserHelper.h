#pragma once

#include "Dependencies.h"

/*

Written by Dom Amato 4/7/17

*/

class ParserHelper {
public:
	static vector<string> parseSentences(string text);

	static vector<string> parseUnigrams(string text);
	static vector<string> parseBigrams(vector<string> unigrams, string text);
	static void parseBigrams(map<string, map<string, int>> &bigramMap, string text);

	static map<string, int> parseNGramFrequency(vector<string> words);
	static map<int, vector<string>> remapFrequency(map<string, int> freqMap);
private:
	static string removeTitlePeriods(string text) {
		vector<string> monikers;
		smatch stringMatcher;
		regex regexPattern("(?:[MDSJmdsj]r|[MDSJmdsj]rs|[MDSJmdsj]s|[Mm]iss|[Pp]hd)\\.");
		string textToParse = text;
		while (regex_search(textToParse, stringMatcher, regexPattern)) {
			monikers.push_back(stringMatcher.str());
			textToParse = stringMatcher.suffix().str();
		}
		for (auto moniker : monikers) {
			text.erase(text.find(moniker)+ moniker.length()-1, 1);
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

