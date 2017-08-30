#pragma once

#include "Dependencies.h"

/*

Written by Dom Amato 5/9/17

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

	void convertTransitionMatrix(map<int, map<int, int>> matrix) {
		for (auto tag : matrix) {
			vector<int> distributions;
			int total = 0;
			for (auto innerTag : tag.second) {
				total += innerTag.second;
				distributions.push_back(innerTag.second);
			}
			if (total > 0) {
				transitionProbabilities.emplace(tag.first, discrete_distribution<>(distributions.begin(), distributions.end()));
			}
		}
	}

	void convertEmissionMatrix(map<string, vector<int>> matrix) {
		for (auto tag : matrix) {
			emissionProbabilities.emplace(tag.first, discrete_distribution<>(tag.second.begin(), tag.second.end()));
		}
	}

	vector<double> getTransitionProbabilities(int tag) {
		if (transitionProbabilities.count(tag) > 0) {
			return transitionProbabilities.at(tag).probabilities();
		}
		return vector<double>(possibleTags.size(), 0.0);
	}

	vector<double> getEmissionProbabilities(string token) {
		return emissionProbabilities.at(token).probabilities();
	}

	vector<string> viterbiAlgorithm(vector<string> &input) {
		if (input.size() > 0) {
			vector< vector<double> > viterbiMatrix;

			if (input.at(input.size() - 1) != "</s>") {
				input.push_back("</s>");
			}

			viterbiMatrix.push_back(vector<double>(possibleTags.size(), -DBL_MAX));
			for (auto tag : possibleTags) {
				if (emissionProbabilities.count(input[0]) > 0) {
					viterbiMatrix[0][getTagIndex(tag)] = log(transitionProbabilities.at(getTagIndex("<s>")).probabilities().at(getTagIndex(tag))) +
						log(emissionProbabilities.at(input[0]).probabilities().at(getTagIndex(tag)));
				} //unknown words here are generally a pain
				else {
					viterbiMatrix[0][getTagIndex(tag)] = log(transitionProbabilities.at(getTagIndex("<s>")).probabilities().at(getTagIndex(tag)));
				}

			}

			for (int obs = 1; obs < input.size(); obs++) {
				viterbiMatrix.push_back(vector<double>(possibleTags.size(), -DBL_MAX));
				for (auto tag : possibleTags) {
					double maxTransitionProb = -DBL_MAX;
					for (auto prev_tag : possibleTags) {
						//some transitions dont work (any tag -> <s>)
						if (transitionProbabilities.count(getTagIndex(prev_tag)) > 0) {
							double transitionProb = transitionProbabilities.at(getTagIndex(prev_tag)).probabilities().at(getTagIndex(tag));
							double hmmTransProb = viterbiMatrix.at(obs - 1).at(getTagIndex(prev_tag)) + log(transitionProb);
							if (maxTransitionProb < hmmTransProb) {
								maxTransitionProb = hmmTransProb;
							}
						}
					}
					double emissionProb = 0.0;
					//unknown words result in an error so make sure the corpus has the word...
					if (emissionProbabilities.count(input[obs]) > 0) {
						emissionProb = emissionProbabilities.at(input[obs]).probabilities().at(getTagIndex(tag));
					}
					viterbiMatrix[obs][getTagIndex(tag)] = (maxTransitionProb <= -DBL_MAX ? 0.0 : maxTransitionProb) + log(emissionProb);
				}
				checkUnknown(viterbiMatrix, obs, input);
			}

			vector<string> tagResult;

			for (auto col : viterbiMatrix) {
				int tag = 0; // the unknown tag
				int index = 0;
				double maxProb = -DBL_MAX;
				for (auto prob : col) {
					if (prob > maxProb) {
						maxProb = prob;
						tag = index;
					}
					index++;
				}
				tagResult.push_back(getTagFromIndex(tag));
			}
			return tagResult;
		}
		return vector<string>();
	}

private:
	map<string, discrete_distribution<>> emissionProbabilities;
	map<int, discrete_distribution<>> transitionProbabilities;
	default_random_engine generator;

	void checkUnknown(vector< vector<double> > &viterbiMatrix, int obs, vector<string> &input) {
		smatch stringMatcher;
		regex regexPattern("[-+,.]*\\d+[,.]*\\d+");
		if (regex_search(input[obs], stringMatcher, regexPattern)) {
			viterbiMatrix[obs][getTagIndex("CD")] = log(1);
			return;
		}

		double maxProb = -DBL_MAX;
		for (auto prob : viterbiMatrix[obs]) {
			if (prob > maxProb) {
				return;
			}
		}

		//if its capitalized its probably a proper noun
		if (isupper(input[obs].at(0))) {
			viterbiMatrix[obs][getTagIndex("NNP")] = log(1);
			return;
		}

		//if the whole word is capitalized it is probably a proper noun
		regexPattern = regex("[A-Z]+[^a-z]");
		if (regex_search(input[obs], stringMatcher, regexPattern)) {
			viterbiMatrix[obs][getTagIndex("NNP")] = log(1);
			return;
		}

		//it should only be possible to get here if we have an unkown tag
		if (obs > 0) {
			regexPattern = regex("(?:[MDSJmdsj]r|[MDSJmdsj]rs|[MDSJmdsj]s|[Mm]iss|[Pp]hd)\\.");
			if (regex_search(input[obs - 1], stringMatcher, regexPattern)) {
				viterbiMatrix[obs][getTagIndex("NNP")] = log(1);
				return;
			}
		}

		for (auto tag : possibleTags) {
			double maxTransitionProb = -DBL_MAX;
			for (auto prev_tag : possibleTags) {
				//some transitions dont work (any tag -> <s>)
				if (transitionProbabilities.count(getTagIndex(prev_tag)) > 0) {
					double transitionProb = transitionProbabilities.at(getTagIndex(prev_tag)).probabilities().at(getTagIndex(tag));
					double hmmTransProb = viterbiMatrix.at(obs - 1).at(getTagIndex(prev_tag)) + log(transitionProb);
					if (maxTransitionProb < hmmTransProb) {
						maxTransitionProb = hmmTransProb;
					}
				}
			}
			if ((input[obs] != "</s>" && tag == "</s>") || tag == "<s>") { //end of sentence tags can only be at the end of sentences and beginning tags should never be possible
				viterbiMatrix[obs][getTagIndex(tag)] = log(0);
			}
			else {
				viterbiMatrix[obs][getTagIndex(tag)] = maxTransitionProb;
			}
		}

		return;
	}
};