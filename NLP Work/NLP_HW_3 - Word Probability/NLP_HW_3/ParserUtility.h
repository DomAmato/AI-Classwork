#pragma once
#include "Dependencies.h"


class ParserUtility {
public:
	static vector<string> parseSentences(string text) {
		vector<string> sentences;
		smatch stringMatcher;
		regex regexPattern("\\b[)]*[.!?]+[\"]*( +(?=[a-z]?[A-Z0-9])|\n|\r)"); //this handles bicaps like iPod
		string textToParse = removeTitlePeriods(text);
		while (regex_search(textToParse, stringMatcher, regexPattern)) {
			sentences.push_back(stringMatcher.prefix().str() + (stringMatcher.str().back() == '\n' ? stringMatcher.str().substr(0, stringMatcher.str().size() - 1) : stringMatcher.str()));
			textToParse = stringMatcher.suffix().str();
		}
		return sentences;
	}

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
			text.erase(text.find(moniker) + moniker.length() - 1, 1);
		}
		return text;
	}

	static vector<string> parseTokens(string text) {
		vector<string> words;
		smatch stringMatcher;
		regex regexPattern("<s>|<\\/s>|\\b(?:[a-zA-Z]\\.){2,}|(((\\w+[\\w',.-]*)\\b)|([^\\s\\w]))");
		while (regex_search(text, stringMatcher, regexPattern)) {
			if (stringMatcher.str().find("'") != string::npos && stringMatcher.str().length() > 1) {
				auto parsedC = parseContraction(stringMatcher.str());
				words.push_back(parsedC.first);
				words.push_back(parsedC.second);
			}
			else {
				words.push_back(stringMatcher.str());
			}
			text = stringMatcher.suffix().str();
		}
		return words;
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