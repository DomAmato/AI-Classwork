#pragma once

#include "Dependencies.h"

/*

Written by Dom Amato 4/20/17

*/

class ProbabilityUtility
{
public:
	ProbabilityUtility() {
		//create a random seed for the random number generator
		random_device rdev{};
		default_random_engine e{ rdev() };
		generator = e;
	}
	
	vector<double> getUnigramProbibilities() {
		return unigrams.probabilities();
	}

	double getUnigramProbability(string unigram) {
		int counter = 0;
		for (auto unig : unigramVector) {
			if (unig == unigram) {
				return unigrams.probabilities().at(counter);
			}
			counter++;
		}
		return 0;
	}

	double getBigramProbability(string prevWord, string word) {
		int counter = 0;
		if (bigramVector.count(prevWord) > 0) {
			for (auto biG : bigramVector.at(prevWord)) {
				if (biG == word) {
					//auto probs = bigrams.at(prevWord).probabilities();
					//cout << "Bigram Probability: " << probs.at(counter) << endl;
					return bigrams.at(prevWord).probabilities().at(counter);
				}
				counter++;
			}
		}
		return 0;
	}

	void createUnigramDistribution(map<string, int> unigrams);
	void createBigramDistributions(map<string, map<string, int>> bigrams);

	double getSentenceUnigramProb(vector<string> sentence);
	double getSentenceBigramProb(vector<string> sentence);

	string generateUnigramSentence();
	string generateBigramSentence();

private:
	vector<string> unigramVector;
	map<string, vector<string>> bigramVector;
	discrete_distribution<> unigrams;
	map<string, discrete_distribution<>> bigrams;
	default_random_engine generator;
};