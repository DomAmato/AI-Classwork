#pragma once
#include "ParserHelper.h"
#include "ProbabilityUtility.h"
#include "ThreadPool.h"

/*

Written by Dom Amato 5/8/17

*/

void runViterbi(vector<pair<vector<string>, vector<string>>> * sentenceTags, vector<string> &sentence, ProbabilityUtility * probabilities) {
	sentenceTags->push_back(make_pair(sentence, probabilities->viterbiAlgorithm(sentence)));
}

void runViterbiWithKnownTags(vector<tuple<vector<string>, vector<int>, vector<string>>> * sentenceTags, vector<string> &sentence, vector<int> &tags, ProbabilityUtility * probabilities) {
	sentenceTags->push_back(make_tuple(sentence, tags, probabilities->viterbiAlgorithm(sentence)));
}
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

		ofstream debugOut;
		if (debug) {
			debugOut.open("debug_output.txt");
		}

		if (trainingData.is_open()) {
			ParserHelper parser;
			cout << "=============== Training Data ===============" << endl;
			cout << "Parsing data";
			parser.parseTrainingData(trainingData);
			cout << " - Finished" << endl;
			if (debug) {
				auto transitionMatrix = parser.getTransitionMatrix();

				cout << "Enumerating data";
				debugOut << "\t";
				for (auto tag : transitionMatrix) {
					int tagIndex = tag.first;
					debugOut << getTagFromIndex(tagIndex) << " | ";
				}
				debugOut << endl;


				for (auto tag : transitionMatrix) {
					debugOut << getTagFromIndex(tag.first) << "\t";
					for (auto innerTag : tag.second) {
						debugOut << innerTag.second << " | ";
					}
					debugOut << endl;
				}

				debugOut << endl << endl;
				auto emissionMatrix = parser.getEmissionMatrix();

				for (auto token : emissionMatrix) {
					debugOut << token.first << "\t";
					int index = 0;
					for (auto innerTag : token.second) {
						if (innerTag > 0) {
							debugOut << getTagFromIndex(index) << " | ";
						}
						index++;

					}
					debugOut << endl;
				}
				cout << " - Finished" << endl;
			}

			ProbabilityUtility probabilities;

			cout << "Converting data to probabilities";
			probabilities.convertEmissionMatrix(parser.getEmissionMatrix());
			probabilities.convertTransitionMatrix(parser.getTransitionMatrix());
			cout << " - Finished" << endl;

			if (debug) {
				cout << "Enumerating Probability data";
				for (auto tag : possibleTags) {
					debugOut << tag << "\t";
					auto probs = probabilities.getTransitionProbabilities(getTagIndex(tag));
					for (auto prob : probs) {
						debugOut << prob << " | ";
					}
					debugOut << endl;
				}
				debugOut << endl;

				for (auto token : parser.getEmissionMatrix()) {
					debugOut << token.first << "\t";
					auto probs = probabilities.getEmissionProbabilities(token.first);
					for (auto prob : probs) {
						debugOut << prob << " | ";
					}
					debugOut << endl;
				}
				cout << " - Finished" << endl;
			}

			cout << "=============== Testing Data ===============" << endl;
			ifstream testData;
			testData.open(argv[2]);

			if (testData.is_open()) {
				cout << "Parsing Testing Data";
				parser.parseTestingData(testData);
				cout << " - Finished" << endl;

				cout << "Running Viterbi Algorithm on " << parser.getSentences().size() << " sentences";

				

				if (parser.getSentenceTags().size() > 0) {
					//threadpool eats up all available threads may slow down computer for several minutes
					ThreadPool pool(8);

					//because of concurrency issues we need to pair up the proper tags with everything else
					vector<tuple<vector<string>, vector<int>, vector<string>>> sentenceTags;
					for (int i = 0; i < parser.getSentences().size(); i++) {
						pool.enqueue(runViterbiWithKnownTags, &sentenceTags, parser.getSentences().at(i), parser.getSentenceTags().at(i), &probabilities);
					}

					pool.~ThreadPool();

					cout << " - Finished" << endl;

					if (debug) {
						debugOut << "=============== Wrongly Tagged Words ===============" << endl;
					}
					int totalTags = 0;
					int correctTags = 0;
					cout << "Comparing against known tags";
					for (auto tag : sentenceTags) {
						if (get<0>(tag).size() > 0) {
							for (int j = 0; j < get<0>(tag).size() - 1; j++) {
								//we add the "</s> tag on the end so drop it
								totalTags++;
								bool correctTag = getTagFromIndex(get<1>(tag).at(j)) == get<2>(tag).at(j);
								if (correctTag) {
									correctTags++;
								}
								output << get<0>(tag).at(j) << " " << getTagFromIndex(get<1>(tag).at(j)) << " " << get<2>(tag).at(j) << endl;
								if (debug) {
									if (!correctTag) {
										debugOut << get<0>(tag).at(j) << " - " << getTagFromIndex(get<1>(tag).at(j)) << " - " << get<2>(tag).at(j) << endl;
									}
								}
							}
							output << endl;
						}
					}
					if (debug) {
						debugOut << endl << "Accuracy:\nTotal Tags: " << totalTags << "\nCorrectly Tagged: " << correctTags << "\nOverall Accuracy: " << (double(correctTags) / double(totalTags)) << endl;
					}
				}
				else {
					//threadpool eats up all available threads may slow down computer for several minutes
					ThreadPool pool(8);

					vector<pair<vector<string>, vector<string>>> sentenceTags;
					for (auto sentence : parser.getSentences()) {
						pool.enqueue(runViterbi, &sentenceTags, sentence, &probabilities);
					}

					pool.~ThreadPool();

					cout << " - Finished" << endl;
					cout << "Tagging untagged words";
					for (auto tag : sentenceTags) {
						if (tag.first.size() > 0) {
							for (int i = 0; i < tag.first.size() - 1; i++) {
								//we add the "</s> tag on the end so drop it
								output << tag.first.at(i) << " " << tag.second.at(i) << endl;
								if (debug) {
									debugOut << tag.first.at(i) << " - " << tag.second.at(i) << endl;
								}
							}
							output << endl;
							if (debug) {
								debugOut << endl;
							}
						}
					}
					cout << " - Finished" << endl;
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
	}
	cout << "Finished!" << endl;
	return 0;
}