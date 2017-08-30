#include "Homework1.h"

/*

Written by Dom Amato 4/7/17

*/

int main(int argc, char* argv[])
{
	if (argc < 3) {
		cout << "Not enough arguments, need to specify a input file and output file";
		return -1;
	}
	else {
		ifstream input;
		input.open(argv[1]);
		ofstream output(argv[2]);

		if (input.is_open()) {
			auto text = Homework1::parseParagraphs(input);
			cout << "Parsing Paragraphs" << endl;
			output << "Total Paragraphs: " << text.first << endl;
			cout << "Parsing Sentences" << endl;
			output << "Total Sentences: " << Homework1::parseSentences(text.second) << endl;
			cout << "Parsing Words" << endl;
			auto totalWords = Homework1::parseWords(text.second);
			output << "Total Words: " << totalWords.size() << endl;
			cout << "Determining Word Frequency" << endl;
			auto totalUniqueWords = Homework1::parseWordFrequency(totalWords);
			output << "Total Unique Words: " << totalUniqueWords.size() << endl << endl << "===================================================" << endl << endl;

			auto remappedTotals = Homework1::remapFrequency(totalUniqueWords);

			cout << "Writing Word Frequency" << endl;
			for (auto freqWords = remappedTotals.rbegin(); freqWords != remappedTotals.rend(); ++freqWords) {
				for (auto word : freqWords->second) {
					output << freqWords->first << ": " << word << endl;
				}
			}
			output.close();
		}
		else {
			cout << "Could not open input file, does it exist or did you misspell the file name?";
			return -1;
		}
	}
	cout << "Success!";
	return 0;
}
