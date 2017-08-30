#pragma once

#include "Dependencies.h"

/*

Written by Dom Amato 5/8/17

*/

class ParserHelper
{
private:
	vector<vector<string>> sentences;
	vector<vector<int>> sentenceTags;
	regex regexPattern = regex("\\s");
	smatch stringMatcher;

	//matrix of possibilities of tags transitions
	map<int, map<int, int>> transmissionMatrix;

	//matrix of possibilities of tags emissions
	map<string, vector<int>> emissionMatrix;

	void initTransitionMatrix() {
		for (auto tag : possibleTags) {
			map<int, int> innerMap;
			for (auto innerTag : possibleTags) {
				innerMap.emplace(getTagIndex(innerTag), 0);
			}
			transmissionMatrix.emplace(getTagIndex(tag), innerMap);
		}
	}
public:
	ParserHelper() {
		initTransitionMatrix();
	}

	vector<vector<string>> getSentences() {
		return sentences;
	}

	vector<vector<int>> getSentenceTags() {
		return sentenceTags;
	}

	map<int, map<int, int>> getTransitionMatrix() {
		return transmissionMatrix;
	}

	map<string, vector<int>> getEmissionMatrix() {
		return emissionMatrix;
	}

	void parseTrainingData(ifstream & trainingData) {
		int prevTag = getTagIndex("<s>"); //since we have a finite set of tags we can keep an index
		int tag = 0;
		while (!trainingData.eof()) {
			string token = "";
			string line;
			getline(trainingData, line);
			bool parseTag = false;
			while (regex_search(line, stringMatcher, regexPattern)) {
				if (parseTag) {
					tag = getTagIndex(stringMatcher.prefix().str());
				}
				else {
					token = stringMatcher.prefix().str();
					parseTag = true;
				}
				line = stringMatcher.suffix().str();
			}

			if (line != "") {
				//if the token exists increment the tag occurence
				if (emissionMatrix.count(token) > 0) {
					emissionMatrix.at(token).at(tag)++;
				}//emplace a new vector with 1 value incremented at that token
				else {
					vector<int> emptyTagVector(possibleTags.size(), 0);
					emptyTagVector.at(tag)++;
					emissionMatrix.emplace(token, emptyTagVector);
				}
				transmissionMatrix.at(prevTag).at(tag)++;
				prevTag = tag;
			}
			else {
				transmissionMatrix.at(prevTag).at(getTagIndex("</s>"))++;
				prevTag = getTagIndex("<s>");
			}
		}
	}

	void parseTestingData(ifstream & trainingData) {
		string line;
		getline(trainingData, line);
		if (regex_search(line, stringMatcher, regexPattern)) {
			parseTestingDataWithTags(line, trainingData);
		}
		else {
			parseTestingDataWithoutTags(line, trainingData);
		}
	}

	void parseTestingDataWithoutTags(string line, ifstream & trainingData) {
		vector<string> sentence;
		if (line != "") {
			sentence.push_back(line);
		}
		while (!trainingData.eof()) {
			getline(trainingData, line);
			if (line != "") {
				sentence.push_back(line);
			}
			else {
				sentences.push_back(sentence);
				sentence.clear();
			}
		}
	}

	void parseTestingDataWithTags(string line, ifstream & trainingData) {
		int prevTag = 0; //since we have a finite set of tags we can keep an index
		int tag = 0;
		string token = "";
		bool parseTag = false;
		vector<string> sentence;
		vector<int> tags;

		while (regex_search(line, stringMatcher, regexPattern)) {
			if (parseTag) {
				tag = getTagIndex(stringMatcher.prefix().str());
			}
			else {
				token = stringMatcher.prefix().str();
				parseTag = true;
			}
			line = stringMatcher.suffix().str();
		}

		if (line != "") {
			sentence.push_back(token);
			tags.push_back(tag);
			prevTag = tag;
		}
		else {
			sentenceTags.push_back(tags);
			sentences.push_back(sentence);
			tags.clear();
			sentence.clear();
			prevTag = 0;
		}

		while (!trainingData.eof()) {
			
			token = "";
			getline(trainingData, line);
			parseTag = false;
			while (regex_search(line, stringMatcher, regexPattern)) {
				if (parseTag) {
					tag = getTagIndex(stringMatcher.prefix().str());
				}
				else {
					token = stringMatcher.prefix().str();
					parseTag = true;
				}
				line = stringMatcher.suffix().str();
			}

			if (line != "") {
				sentence.push_back(token);
				tags.push_back(tag);
				prevTag = tag;
			}
			else {
				sentenceTags.push_back(vector<int>(tags));
				sentences.push_back(vector<string>(sentence));
				tags.clear();
				sentence.clear();
				prevTag = 0;
			}
		}
	}
};