#include "ProbabilityUtility.h"

/*

Written by Dom Amato 4/20/17

*/
void ProbabilityUtility::createBigramDistributions(map<string, map<string, unsigned long>> bigramMap)
{
	vector<unsigned long> bigramCounts;
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
		bigrams.emplace(entry.first, discrete_distribution<unsigned long>(bigramCounts.begin(), bigramCounts.end()));
		bigramCounts.clear();
	}
}