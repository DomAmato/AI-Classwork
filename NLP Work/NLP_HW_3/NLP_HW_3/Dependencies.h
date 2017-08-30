#pragma once

#include <regex>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <random>
#include <thread>

/*

Written by Dom Amato 4/18/17

*/

using namespace std;

vector<string> possibleTags = { "UNK", "<s>", "</s>", "#", "$", "''", "(", ")", ",", ".", ":", "``", "CC", "CD", "DT", "EX", "FW", "IN", "JJ", "JJR", "JJS", "LS", "MD", "NN", "NNP", "NNPS", "NNS", "PDT", "POS", "PRP", "PRP$", "RB", "RBR", "RBS", "RP", "SYM", "TO", "UH", "VB", "VBD", "VBG", "VBN", "VBP", "VBZ", "WDT", "WP", "WP$", "WRB"};

string getTagFromIndex(int index) {
	return possibleTags.at(index);
}

int getTagIndex(string tag) {
	int index = 0;
	for (auto ptag : possibleTags) {
		if (ptag == tag) {
			return index;
		}
		index++;
	}
	return 0; //the unknown tag
}

static double float2log(double val) {
	return log(val);
}

static double log2float(double val) {
	return exp(val);
}