#pragma once
#include "ParserHelper.h"
#include "ProbabilityUtility.h"
/*

Written by Dom Amato 4/20/17

*/

/*
Additional Corpuses from https://github.com/nlp-compromise/nlp-corpus
*/

int main(int argc, char* argv[])
{
	if (argc < 4) {
		cout << "Not enough arguments, need to specify a training file, test data, and an output file";
		return -1;
	}
	else {
		bool debug = (argc > 4 && strcmp(argv[4], "-d") == 0);
		ifstream trainingData;
		trainingData.open(argv[1]);
		ofstream output(argv[3]);

		if (trainingData.is_open()) {
			ProbabilityUtility probabilities;

			output << "************* Training Data (" << argv[1] << ") *************" << endl;

			cout << "Reading File" << endl;
			string text;
			while (!trainingData.eof()) {
				string line;
				getline(trainingData, line);
				text.append(line + "\n");
			}
			cout << "Parsing Sentences" << endl;
			auto sentences = ParserHelper::parseSentences(text);

			output << "Total Sentences: " << sentences.size() << endl;

			cout << "Parsing Unigrams" << endl;
			vector<string> allUnigrams;
			for (auto sentence : sentences) {
				for (auto unigram : ParserHelper::parseUnigrams(sentence)) {
					allUnigrams.push_back(unigram);
				}
			}

			output << endl << "======== Unigrams =======" << endl << "Total Unigrams: " << allUnigrams.size() << endl;

			cout << "Mapping Unigram Frequency" << endl;
			auto unigramMap = ParserHelper::parseNGramFrequency(allUnigrams);

			output << "Total Unique Unigrams: " << unigramMap.size() << endl;

			cout << "Creating Unigram Distributions" << endl;
			probabilities.createUnigramDistribution(unigramMap);

			if (debug) {
				cout << "Enumerating Unigram Probabilities" << endl;
				output << "Unigram Probabilities:" << endl;
				for (auto unigram : unigramMap) {
					output << unigram.first << " -> " << float2log(probabilities.getUnigramProbability(unigram.first)) << endl;
				}
			}

			cout << "Parsing and Mapping Bigrams" << endl;
			map<string, map<string, int>> bigramMap;
			for (auto sentence : sentences) {
				ParserHelper::parseBigrams(bigramMap, sentence);
			}

			output << endl << "======== Bigrams =======" << endl << "Total Bigram Stems: " << bigramMap.size() << endl;

			int uniqueBigrams = 0;
			int totalBigrams = 0;
			for (auto bi1 : bigramMap) {
				for (auto bi2 : bi1.second) {
					uniqueBigrams++;
					totalBigrams += bi2.second;
				}
			}

			output << "Total Bigrams: " << totalBigrams << endl << "Total Unique Bigrams: " << uniqueBigrams << endl;

			cout << "Creating Bigram Distributions" << endl;
			probabilities.createBigramDistributions(bigramMap);

			if (debug) {
				cout << "Enumerating Bigram Probabilities" << endl;
				output << "Bigram Probabilities:" << endl;
				for (auto entry : bigramMap) {
					for (auto innerEntry : entry.second) {
						output << entry.first << " " << innerEntry.first << " -> " << float2log(probabilities.getBigramProbability(entry.first, innerEntry.first)) << endl;
					}
				}


				cout << "Enumerating Sentences and Probabilities" << endl;
				output << endl << "======== Sentences =======" << endl;
				for (auto sentence : sentences) {
					output << "Sentence: " << sentence << endl;
					auto sentenceUnigrams = ParserHelper::parseUnigrams(sentence);
					output << "Unigram Prob - " << probabilities.getSentenceUnigramProb(sentenceUnigrams) << endl;
					output << "Bigram Prob - " << probabilities.getSentenceBigramProb(sentenceUnigrams) << endl;
				}
			}

			cout << "Testing Data" << endl;
			ifstream testData;
			testData.open(argv[2]);

			if (testData.is_open()) {
				cout << "Reading Testing Data" << endl;
				string testText = "";
				while (!testData.eof()) {
					string line;
					getline(testData, line);
					testText.append(line + "\n");
				}

				cout << "Enumerating Testing Data Sentences and Probabilities" << endl;
				output << endl << "************* Test Data (" << argv[2] << ") *************" << endl;

				auto testSentences = ParserHelper::parseSentences(testText);
				output << "Total Sentences: " << testSentences.size() << endl;

				int totalTestTokens = 0;
				int sentCount = 0;
				stringstream ss;
				double unigramTotalProbability = 0;
				double bigramTotalProbability = 0;
				for (auto sentence : testSentences) {
					ss << "Sentence "<< sentCount++ <<": " << sentence << endl;
					auto sentenceUnigrams = ParserHelper::parseUnigrams(sentence);
					totalTestTokens += sentenceUnigrams.size();
					auto unigramProb = probabilities.getSentenceUnigramProb(sentenceUnigrams);
					unigramTotalProbability += unigramProb;
					auto bigramProb = probabilities.getSentenceBigramProb(sentenceUnigrams);
					bigramTotalProbability += bigramProb;
					ss << "Unigram Prob - " << unigramProb << endl;
					ss << "Bigram Prob - " << bigramProb << endl << endl;
				}
				output << "Total Tokens: " << totalTestTokens << endl;

				output << endl << "======== Test Sentences =======" << endl;
				output << ss.str();

				output << "==========================" << endl << "* Probability:" << endl << "-Average unigram probability : " << (unigramTotalProbability/ testSentences.size()) << endl << "-Average bigram probability : " << (bigramTotalProbability / testSentences.size()) << endl;
			}
			else {
				cout << "Could not open test file, does it exist or did you misspell the file name?";
				return -1;
			}
			cout << "Generating Random Sentences" << endl;
			output << endl << "************* Random Sentences *************" << endl;
			output << endl << "======== Unigram Generated Sentences =======" << endl;
			for (int i = 0; i < 10; i++) {
				output << probabilities.generateUnigramSentence() << endl;
			}

			output << endl << "======== Bigram Generated Sentences =======" << endl;
			for (int i = 0; i < 10; i++) {
				output << probabilities.generateBigramSentence() << endl;
			}

		}
		else {
			cout << "Could not open training file, does it exist or did you misspell the file name?";
			return -1;
		}
	}
	cout << "Finished!" << endl;
	return 0;
}
