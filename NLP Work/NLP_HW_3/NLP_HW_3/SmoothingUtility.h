#pragma once
#include "Dependencies.h"
/*
void addOneSmoothing(map<string, vector<int>> emissionMatrix, map<int, map<int, int>> transmissionMatrix) {
	//ADD ONE to all tables
	for (int i = 0; i<possibleTags.size(); i++) {
		for (int j = 0; j<possibleTags.size(); j++) {
			transmissionMatrix[i][j] += 1;
			storage.tagCountTable[i] += 1;
			storage.tagCountTable[j] += 1;
		}
	}
	for (int i = 0; i<emissionMatrix.size(); i++) {
		for (int j = 0; j<possibleTags.size(); j++) {
			storage.wordTagCountTable[i][j] += 1;
			storage.wordCountTable[i] += 1;
			storage.tagCountTable[j] += 1;
		}
	}
	//END ADD ONE
	for (int i = 0; i<possibleTags.size(); i++) {
		for (int j = 0; j<possibleTags.size(); j++) {
			//probability table(i|j) = count(j,i) / count(j)
			//probability table(t(k)|t(k-1)) = count(t(k-1),t(k)) / count(t(k-1))
			storage.tagProbTable[i][j] = (double)transmissionMatrix[i][j] / (double)storage.tagCountTable[j];
		}
	}
	for (auto token: emissionMatrix) {
		for (int j = 0; j<possibleTags.size(); j++) {
			//probability table(i|j) = count(j,i) / count(j)
			//probability table(w(k)|t(k)) = count(t(k),w(k)) / count(t(k))
			emissionMatrix[token.first][j] = (double)storage.wordTagCountTable[i][j] / (double)storage.tagCountTable[j];
		}
	}
	return;
}

void wittenBellSmoothing() {
	//Begin Initialize T and Z for t(k) and t(k-1) Bigram Probability
	int T_Tag[possibleTags.size()];
	int Z_Tag[possibleTags.size()];
	memset(T_Tag, 0, sizeof(T_Tag));
	memset(Z_Tag, 0, sizeof(Z_Tag));

	for (int j = 0; j<possibleTags.size(); j++) {
		for (int k = 0; k<possibleTags.size(); k++) {
			if (transmissionMatrix[k][j]>0) {
				T_Tag[j]++;
			}
			else {
				Z_Tag[j]++;
			}
		}
	}
	//End Init

	//Calculate t(k) and t(k-1)  Bigram Probability
	for (int i = 0; i<possibleTags.size(); i++) {
		for (int j = 0; j<possibleTags.size(); j++) {
			if (transmissionMatrix[i][j]>0) {
				//if count(i,j) is greater than zero, execute the formula
				storage.tagProbTable[i][j] = (double)transmissionMatrix[i][j] / ((double)storage.tagCountTable[j] + (double)T_Tag[j]);
			}
			else {
				//if count(i,j) is equal to zero, execute the formula
				storage.tagProbTable[i][j] = (double)T_Tag[j] / ((double)Z_Tag[j] * ((double)storage.tagCountTable[j] + (double)T_Tag[j]));
			}
		}
	}
	//end calculate

	//Begin Initialize T and Z for w(k) and t(k) Probability
	int T_WordTag[possibleTags.size()];
	int Z_WordTag[possibleTags.size()];
	memset(T_WordTag, 0, sizeof(T_WordTag));
	memset(Z_WordTag, 0, sizeof(Z_WordTag));

	for (int j = 0; j<possibleTags.size(); j++) {
		for (int k = 0; k<emissionMatrix.size(); k++) {
			if (storage.wordTagCountTable[k][j]>0) {
				T_WordTag[j]++;
			}
			else {
				Z_WordTag[j]++;
			}
		}
	}
	//End Init

	//Calculate w(k) and t(k) Probability
	for (int i = 0; i<emissionMatrix.size(); i++) {
		for (int j = 0; j<possibleTags.size(); j++) {
			if (storage.wordTagCountTable[i][j]>0) {
				//if count(i,j) is greater than zero, execute the formula
				emissionMatrix[i][j] = (double)storage.wordTagCountTable[i][j] / ((double)storage.tagCountTable[j] + (double)T_WordTag[j]);
			}
			else {
				//if count(i,j) is equal to zero, execute the formula
				emissionMatrix[i][j] = (double)T_WordTag[j] / ((double)Z_WordTag[j] * ((double)storage.tagCountTable[j] + (double)T_WordTag[j]));
			}
		}
	}
	//end calculate
	return;
}

void kneserNeySmoothing() {
	double D = 0.085;

	double alphaTag[possibleTags.size()];
	double alphaWordTag[possibleTags.size()];

	double PContinuationTag[possibleTags.size()];
	double PContinuationWordTag[emissionMatrix.size()];


	//BEGIN calculate continuation probability for tag bigram
	int tagNumerator[possibleTags.size()];
	memset(tagNumerator, 0, sizeof(tagNumerator));

	for (int i = 0; i<possibleTags.size(); i++) {
		int continuationNumerator = 0;
		for (int j = 0; j<possibleTags.size(); j++) {
			if (transmissionMatrix[i][j]>0) {
				continuationNumerator += 1;
			}
		}
		//add-one smoothing for the numerator for <s>, zero words precedes that character.
		if (continuationNumerator == 0) {
			tagNumerator[i] = 1;
		}
		else {
			tagNumerator[i] = continuationNumerator;
		}
	}
	int continuationDenominator = 0;
	for (int i = 0; i<possibleTags.size(); i++) {
		continuationDenominator += tagNumerator[i];
	}
	//setting up Continuation probability
	for (int i = 0; i<possibleTags.size(); i++) {
		PContinuationTag[i] = (double)tagNumerator[i] / (double)continuationDenominator;
	}
	//END calculate


	//BEGIN calculate continuation probability for Word and Tag
	int wordTagNumerator[emissionMatrix.size()];
	memset(wordTagNumerator, 0, sizeof(wordTagNumerator));

	for (int i = 0; i<emissionMatrix.size(); i++) {
		int continuationNumerator = 0;
		for (int j = 0; j<possibleTags.size(); j++) {
			if (storage.wordTagCountTable[i][j]>0) {
				continuationNumerator += 1;
			}
		}
		//add-one smoothing for the numerator for <s>, zero words precedes that character.
		if (continuationNumerator == 0) {
			wordTagNumerator[i] = 1;
		}
		else {
			wordTagNumerator[i] = continuationNumerator;
		}
	}

	continuationDenominator = 0;
	for (int i = 0; i<emissionMatrix.size(); i++) {
		continuationDenominator += wordTagNumerator[i];
	}

	//setting up Continuation probability
	for (int i = 0; i<emissionMatrix.size(); i++) {
		PContinuationWordTag[i] = (double)wordTagNumerator[i] / (double)continuationDenominator;
	}
	//END calculate


	//calculate alpha for Tags Bigram (normalization factor)
	for (int j = 0; j<possibleTags.size(); j++) {
		double totalBigram = 0;
		for (int i = 0; i<possibleTags.size(); i++) {
			if (transmissionMatrix[i][j]>0) {
				storage.tagProbTable[i][j] = ((double)transmissionMatrix[i][j] - D) / (double)storage.tagCountTable[j];
				totalBigram += storage.tagProbTable[i][j];
			}
		}
		alphaTag[j] = (1.0 - totalBigram) / (1.0 - PContinuationTag[j]);
	}

	//calculate alpha for Word and Tag (normalization factor)
	for (int j = 0; j<possibleTags.size(); j++) {
		double totalBigram = 0;
		for (int i = 0; i<emissionMatrix.size(); i++) {
			if (storage.wordTagCountTable[i][j]>0) {
				emissionMatrix[i][j] = ((double)storage.wordTagCountTable[i][j] - D) / (double)storage.tagCountTable[j];
				totalBigram += emissionMatrix[i][j];
			}
		}
		alphaWordTag[j] = (1.0 - totalBigram) / (1.0 - PContinuationWordTag[j]);
	}




	//real probability calculation for tag given the preceding tag
	for (int i = 0; i<possibleTags.size(); i++) {
		for (int j = 0; j<possibleTags.size(); j++) {
			if (transmissionMatrix[i][j]>0) {
				storage.tagProbTable[i][j] = ((double)transmissionMatrix[i][j] - D) / (double)storage.tagCountTable[j];
			}
			else {
				storage.tagProbTable[i][j] = alphaTag[j] * PContinuationTag[i];
			}
		}
		//cout << PContinuationTag[i] << endl;
	}
	//real probability calculation for Word given Tag
	for (int i = 0; i<emissionMatrix.size(); i++) {
		for (int j = 0; j<possibleTags.size(); j++) {
			if (storage.wordTagCountTable[i][j]>0) {
				emissionMatrix[i][j] = ((double)storage.wordTagCountTable[i][j] - D) / (double)storage.tagCountTable[j];
			}
			else {
				emissionMatrix[i][j] = alphaWordTag[j] * PContinuationWordTag[i];
			}
		}
	}

	return;

}
*/