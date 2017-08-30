#include "ParserHelper.h"

/*

Written by Dom Amato 4/7/17

*/

vector<string> ParserHelper::parseSentences(string text) {
	vector<string> sentences;
	smatch stringMatcher;
	regex regexPattern("\\b[)]*[.!?]+[\"]*( +(?=[a-z]?[A-Z0-9])|\n|\r)"); //this handles bicaps like iPod
	string textToParse = removeTitlePeriods(text);
	while (regex_search(textToParse, stringMatcher, regexPattern)) {
		sentences.push_back("<s>" + stringMatcher.prefix().str() + (stringMatcher.str().back() == '\n' ? stringMatcher.str().substr(0, stringMatcher.str().size() - 1) : stringMatcher.str()) + "</s>");
		textToParse = stringMatcher.suffix().str();
	}
	return sentences;
}

vector<string> ParserHelper::parseUnigrams(string text) {
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

vector<string> ParserHelper::parseBigrams(vector<string> unigrams, string text)
{
	vector<string> words;
	smatch stringMatcher;
	string prevWord = "";
	regex regexPattern("<s>|<\\/s>|\\b(?:[a-zA-Z]\\.){2,}|(((\\w+[\\w',.-]*)\\b)|([^\\s\\w]))");
	while (regex_search(text, stringMatcher, regexPattern)) {
		if (stringMatcher.str().find("'") != string::npos && stringMatcher.str().length() > 1) {
			auto parsedC = parseContraction(stringMatcher.str());
			if (prevWord != "") {
				words.push_back(prevWord + "_" + parsedC.first);
				words.push_back(parsedC.first + "_" + parsedC.second);
			}
			prevWord = parsedC.second;
		}
		else {
			if (prevWord != "") {
				words.push_back(prevWord + "_" + stringMatcher.str());
			}
			prevWord = stringMatcher.str();
		}

		text = stringMatcher.suffix().str();
	}
	return words;
}

void ParserHelper::parseBigrams(map<string, map<string, int>> &bigramMap, string text)
{
	smatch stringMatcher;
	string prevWord = "";
	regex regexPattern("<s>|<\\/s>|\\b(?:[a-zA-Z]\\.){2,}|(((\\w+[\\w',.-]*)\\b)|([^\\s\\w]))");
	while (regex_search(text, stringMatcher, regexPattern)) {
		if (stringMatcher.str().find("'") != string::npos && stringMatcher.str().length() > 1) {
			auto parsedC = parseContraction(stringMatcher.str());
			if (prevWord != "") {
				if (bigramMap.count(prevWord) > 0) {
					if (bigramMap.at(prevWord).count(parsedC.first) > 0) {
						bigramMap.at(prevWord).at(parsedC.first)++;
					}
					else {
						bigramMap.at(prevWord).emplace(parsedC.first, 1);
					}
				}
				else {
					map<string, int> innerMap;
					innerMap.emplace(parsedC.first, 1);
					bigramMap.emplace(prevWord, innerMap);
				}
				if (bigramMap.count(parsedC.first) > 0) {
					if (bigramMap.at(parsedC.first).count(parsedC.second) > 0) {
						bigramMap.at(parsedC.first).at(parsedC.second)++;
					}
					else {
						bigramMap.at(parsedC.first).emplace(parsedC.second, 1);
					}
				}
				else {
					map<string, int> innerMap;
					innerMap.emplace(parsedC.second, 1);
					bigramMap.emplace(parsedC.first, innerMap);
				}
			}
			else {
				if (bigramMap.count(parsedC.first) > 0) {
					if (bigramMap.at(parsedC.first).count(parsedC.second) > 0) {
						bigramMap.at(parsedC.first).at(parsedC.second)++;
					}
					else {
						bigramMap.at(parsedC.first).emplace(parsedC.second, 1);
					}
				}
				else {
					map<string, int> innerMap;
					innerMap.emplace(parsedC.second, 1);
					bigramMap.emplace(parsedC.first, innerMap);
				}
			}
			prevWord = parsedC.second;
		}
		else {
			if (prevWord != "") {
				if (bigramMap.count(prevWord) > 0) {
					if (bigramMap.at(prevWord).count(stringMatcher.str()) > 0) {
						bigramMap.at(prevWord).at(stringMatcher.str())++;
					}
					else {
						bigramMap.at(prevWord).emplace(stringMatcher.str(), 1);
					}
				}
				else {
					map<string, int> innerMap;
					innerMap.emplace(stringMatcher.str(), 1);
					bigramMap.emplace(prevWord, innerMap);
				}
			}
			prevWord = stringMatcher.str();
		}

		text = stringMatcher.suffix().str();
	}
}

map<string, int> ParserHelper::parseNGramFrequency(vector<string> words) {
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

map<int, vector<string>> ParserHelper::remapFrequency(map<string, int> freqMap) {
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

