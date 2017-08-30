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

	double getBigramProbability(string prevWord, string word) {
		int counter = 0;
		if (bigramVector.count(prevWord) > 0) {
			for (auto biG : bigramVector.at(prevWord)) {
				if (biG == word) {
					return bigrams.at(prevWord).probabilities().at(counter);
				}
				counter++;
			}
		}
		return 0;
	}

	string getMostLikelyToken(string prevToken) {
		int index = 0;
		int highestProbIndex = 0;
		double highestProb = 0;
		if (bigrams.count(prevToken) > 0) {
			for (auto biProb : bigrams.at(prevToken).probabilities()) {
				if (biProb > highestProb) {
					highestProb = biProb;
					highestProbIndex = index;
				}
				index++;
			}
			return bigramVector.at(prevToken).at(highestProbIndex);
		}
		return "NULL - The previous token was not in the training corpus";
	}

	void createBigramDistributions(map<string, map<string, unsigned long>> bigrams);

private:
	map<string, vector<string>> bigramVector;
	map<string, discrete_distribution<unsigned long>> bigrams;
	default_random_engine generator;
};