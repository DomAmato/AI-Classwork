#pragma once
#include "ProbabilityUtility.h"
#include "ParserUtility.h"

/*

Written by Dom Amato 4/24/17

*/

int main(int argc, char* argv[])
{
	ifstream trainingData;
	trainingData.open("count_2w.txt");
	ofstream output("output.txt");

	if (trainingData.is_open()) {
		ProbabilityUtility probabilities;

		map<string, map<string, unsigned long>> bigramMap;
		cout << "Reading File" << endl;

		string line;
		smatch stringMatcher;
		regex regexPattern("\\s");

		cout << "Parsing and Mapping Bigrams" << endl;
		while (!trainingData.eof()) {
			getline(trainingData, line);
			string prevToken = "";
			while (regex_search(line, stringMatcher, regexPattern)) {
				if (prevToken == "") {
					prevToken = stringMatcher.prefix().str();
				}
				else {
					map<string, unsigned long> innerMap;
					innerMap.emplace(stringMatcher.prefix().str(), stoul(stringMatcher.suffix().str()));
					bigramMap.emplace(prevToken, innerMap);
				}
				line = stringMatcher.suffix().str();
			}
		}

		cout << "Creating Bigram Distributions" << endl;
		probabilities.createBigramDistributions(bigramMap);

		ifstream testData;
		testData.open("HG-heldout50.txt");

		if (testData.is_open()) {
			cout << "Reading Testing Data" << endl;
			string testText = "";
			while (!testData.eof()) {
				string line;
				getline(testData, line);
				testText.append(line + "\n");
			}

			cout << "Enumerating Testing Data Sentences and Probabilities" << endl;

			auto testSentences = ParserUtility::parseSentences(testText);

			regexPattern = regex("\\w+"); //any word will do

			int sentCount = 0;
			for (auto sentence : testSentences) {
				output << "Actual Sentence " << sentCount++ << ": " << sentence << endl;
				auto sentTokens = ParserUtility::parseTokens(sentence);
				int lastWord = sentTokens.size() - 1;
				for (auto rit = sentTokens.rbegin(); rit != sentTokens.rend(); ++rit) {
					lastWord--;
					if (regex_search(*rit, stringMatcher, regexPattern)) {
						break;
					}
				}
				output << "Actual Word: " << sentTokens.at(lastWord < sentTokens.size() - 1 ? lastWord + 1 : sentTokens.size() - 1) << endl;
				output << "Most Likely Prediction: " << probabilities.getMostLikelyToken(sentTokens.at(lastWord < sentTokens.size() - 1 ? lastWord : sentTokens.size() - 1)) << endl << endl;
			}

		}
		else {
			cout << "Could not open test file, does it exist or did you misspell the file name?";
			return -1;
		}
	}
	else {
		cout << "Could not open training file, does it exist or did you misspell the file name?";
		return -1;
	}
	cout << "Finished!" << endl;
	return 0;
}