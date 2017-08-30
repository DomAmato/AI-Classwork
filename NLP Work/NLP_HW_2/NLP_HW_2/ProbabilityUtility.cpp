#include "ProbabilityUtility.h"

/*

Written by Dom Amato 4/20/17

*/

void ProbabilityUtility::createUnigramDistribution(map<string, int> unigramMap)
{
	vector<int> unigramCounts;
	for (auto entry : unigramMap) {
		unigramVector.push_back(entry.first);
		unigramCounts.push_back(entry.second);
	}
	unigrams = discrete_distribution<>(unigramCounts.begin(), unigramCounts.end());
}

void ProbabilityUtility::createBigramDistributions(map<string, map<string, int>> bigramMap)
{
	vector<int> bigramCounts;
	for (auto entry : bigramMap) {
		for (auto innerEntry : entry.second) {
			if (bigramVector.count(entry.first) > 0) {
				bigramVector.at(entry.first).push_back(innerEntry.first);
			}
			else {
				vector<string> bigramInnerVec;
				bigramInnerVec.push_back(innerEntry.first);
				bigramVector.emplace(entry.first, bigramInnerVec);
			}
			bigramCounts.push_back(innerEntry.second);
		}
		bigrams.emplace(entry.first, discrete_distribution<>(bigramCounts.begin(), bigramCounts.end()));
		bigramCounts.clear();
	}
}

double ProbabilityUtility::getSentenceUnigramProb(vector<string> sentence)
{
	double logOdds = 0;
	for (auto uniG : sentence) {
		auto odds = getUnigramProbability(uniG);
		if (odds != 0) { //need to make sure its not 0 odds
			logOdds += float2log(odds);
		}
	}
	return log2float(logOdds);
}

double ProbabilityUtility::getSentenceBigramProb(vector<string> sentence)
{
	double logOdds = 0;
	string prevWord = "";
	for (auto word : sentence) {
		if (prevWord != "") {
			auto odds = getBigramProbability(prevWord, word);
			if (odds != 0) { //need to make sure its not 0 odds
				logOdds += float2log(odds);
			}
		}
		prevWord = word;
	}
	return log2float(logOdds);
}

string ProbabilityUtility::generateUnigramSentence()
{
	//there is no guarantee unigrams can ever reach the end symbol </s> so lets limit our length
	string sentence = "<s>";
	for (int i = 0; i < 100; i++) {
		string word = unigramVector.at(unigrams(generator));
		if (word != "<s>") {
			sentence += word + " ";
			if (word == "</s>") {
				break;
			}
		}
	}
	return sentence;
}

string ProbabilityUtility::generateBigramSentence()
{
	//It is possible that bigrams can infnintely loop, lets limit it
	string sentence = "<s>";
	string prevWord = "<s>";
	for (int i = 0; i < 100; i++) {
		string word = bigramVector.at(prevWord).at(bigrams.at(prevWord)(generator));
		if (word != "<s>") {
			sentence += word + " ";
			prevWord = word;
			if (word == "</s>") {
				break;
			}
		}
	}
	return sentence;
}
