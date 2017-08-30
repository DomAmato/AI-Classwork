#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	string line;
	int strLn = 0;
	map<string, int> letterMap;

	//set gender to male
	gender = true;

	//create a random seed for the random number generator
	random_device rdev{};
	default_random_engine e{ rdev() };
	generator = e;

	//load the boy name distribution
	ifstream file(ofToDataPath("male_names.txt"));

	//initialize the mappings for every combination of letters
	for (char a = 'a'; a <= 'z'; a++) {
		for (char b = 'a'; b <= 'z'; b++) {
			for (char c = 'a'; c <= 'z'; c++) {
				letterMap.emplace(ofToString(a) + ofToString(b) + ofToString(c), 0);
			}
			letterMap.emplace("_" + ofToString(a) + ofToString(b), 0);
			//end of name mapping
			letterMap.emplace(ofToString(a) + ofToString(b) + "_", 0);
		}
		letterMap.emplace("__" + ofToString(a), 0);
		//final letter of name mapping
		//letterMap.emplace(ofToString(a) + "__", 0);
	}

	//read in the file
	while (getline(file, line))
	{
		//convert to lower case
		line = ofToLower(line);
		//whats the longest name in the file 
		strLn = strLn < line.length() ? line.length() : strLn;
		//add to the set of names
		boysNames.insert(line);
		//handle special cases
		//beginning of names
		letterMap.at("__" + line.substr(0, 1))++;
		letterMap.at("_" + line.substr(0, 2))++;
		//end of names
		//letterMap.at(line.substr(line.length() - 1) + "__")++;
		letterMap.at(line.substr(line.length() - 2) + "_")++;

		//only if a name is longer than 2 can we substring this way
		if (line.length() > 2) {
			for (int i = 0; i < line.length() - 3; i++) {
				letterMap.at(line.substr(i, 3))++;
			}
		}
	}

	//iterate through all the mappings and then create discrete distributions
	vector<int> vec1;
	for (char a = 'a'; a <= 'z'; a++) {
		vector<int> vec2;
		int totalAmt1 = 0; //total number of values in this coding
		for (char b = 'a'; b <= 'z'; b++) {
			vector<int> vec3;
			int totalAmt2 = 0; //total number of values in this coding
			for (char c = 'a'; c <= 'z'; c++) {
				int occurences2 = letterMap.at(ofToString(a) + ofToString(b) + ofToString(c));
				totalAmt2 += occurences2;
				vec3.push_back(occurences2);
			}
			//check for end of names
			vec3.push_back(letterMap.at(ofToString(a) + ofToString(b) + "_"));
			totalAmt2 += vec3.back();
			//discrete distributions need to have at least 1 element that isn't 0
			//but its possible that every 3 letter combination for a distribution had 0
			//so check before generating
			if (totalAmt2 > 0) {
				boyNameCharDist.emplace(ofToString(a) + ofToString(b), discrete_distribution<>(vec3.begin(), vec3.end()));
			}
			//beginning of name combinations
			int occurences = letterMap.at("_" + ofToString(a) + ofToString(b));
			totalAmt1 += occurences;
			vec2.push_back(occurences);
		}
		//see previous comment about discrete distributions
		if (totalAmt1 > 0) {
			boyNameCharDist.emplace("_" + ofToString(a), discrete_distribution<>(vec2.begin(), vec2.end()));
		}
		//first letter in a name
		vec1.push_back(letterMap.at("__" + ofToString(a)));
	}
	boyNameCharDist.emplace("__", discrete_distribution<>(vec1.begin(), vec1.end()));

	//close the previous file
	file.close();
	//now load the girl name distribution
	file.open(ofToDataPath("female_names.txt"));

	//clear the previous mappings
	letterMap.clear();

	//initialize the mappings for every combination of letters
	for (char a = 'a'; a <= 'z'; a++) {
		for (char b = 'a'; b <= 'z'; b++) {
			for (char c = 'a'; c <= 'z'; c++) {
				letterMap.emplace(ofToString(a) + ofToString(b) + ofToString(c), 0);
			}
			letterMap.emplace("_" + ofToString(a) + ofToString(b), 0);
			//end of name mapping
			letterMap.emplace(ofToString(a) + ofToString(b) + "_", 0);
		}
		letterMap.emplace("__" + ofToString(a), 0);
		//final letter of name mapping
		//letterMap.emplace(ofToString(a) + "__", 0);
	}

	//read in the file
	while (getline(file, line))
	{
		//convert to lower case
		line = ofToLower(line);
		//whats the longest name in the file 
		strLn = strLn < line.length() ? line.length() : strLn;
		//add to the set of names
		girlsNames.insert(line);
		//handle special cases
		//beginning of names
		letterMap.at("__" + line.substr(0, 1))++;
		letterMap.at("_" + line.substr(0, 2))++;
		//end of names
		//letterMap.at(line.substr(line.length() - 1) + "__")++;
		letterMap.at(line.substr(line.length() - 2) + "_")++;

		//only if a name is longer than 2 can we substring this way
		if (line.length() > 2) {
			for (int i = 0; i < line.length() - 3; i++) {
				letterMap.at(line.substr(i, 3))++;
			}
		}
	}

	vec1.clear();
	for (char a = 'a'; a <= 'z'; a++) {
		vector<int> vec2;
		int totalAmt1 = 0; //total number of values in this coding
		for (char b = 'a'; b <= 'z'; b++) {
			vector<int> vec3;
			int totalAmt2 = 0; //total number of values in this coding
			for (char c = 'a'; c <= 'z'; c++) {
				int occurences2 = letterMap.at(ofToString(a) + ofToString(b) + ofToString(c));
				totalAmt2 += occurences2;
				vec3.push_back(occurences2);
			}
			//check for end of names
			vec3.push_back(letterMap.at(ofToString(a) + ofToString(b) + "_"));
			totalAmt2 += vec3.back();
			//discrete distributions need to have at least 1 element that isn't 0
			//but its possible that every 3 letter combination for a distribution had 0
			//so check before generating
			if (totalAmt2 > 0) {
				girlNameCharDist.emplace(ofToString(a) + ofToString(b), discrete_distribution<>(vec3.begin(), vec3.end()));
			}
			//beginning of name combinations
			int occurences = letterMap.at("_" + ofToString(a) + ofToString(b));
			totalAmt1 += occurences;
			vec2.push_back(occurences);
		}
		//see previous comment about discrete distributions
		if (totalAmt1 > 0) {
			girlNameCharDist.emplace("_" + ofToString(a), discrete_distribution<>(vec2.begin(), vec2.end()));
		}
		//first letter in a name
		vec1.push_back(letterMap.at("__" + ofToString(a)));
	}
	girlNameCharDist.emplace("__", discrete_distribution<>(vec1.begin(), vec1.end()));

	//slider for minimum length
	nameMinSlider.setup(5, 3, strLn - 2, 40, 40, 200);
	nameMinSlider.setID(0);

	//slider for maximum length
	nameMaxSlider.setup(8, 6, strLn + 2, 300, 40, 200);
	nameMaxSlider.setID(1);

	//slider for how many results to generate
	resultsSlider.setup(5, 1, 9, 375, 420, 220);
	resultsSlider.setID(2);

	//load fonts
	displayFont.load(OF_TTF_SANS, 16);
	displayBigFont.load(OF_TTF_SERIF, 32);

	//allow users to seed the beginning of a name
	nameSeed.setup();
	nameSeed.setFont(displayFont);
	nameSeed.setPlaceholderText("Seed Name");
	nameSeed.setBounds(600, 40, 175, 30);
	nameSeed.setVerticalPadding(0);

	//button to generate names
	generateBtn.setButtonStyle(UI_BUTTON_ROUNDED_RECT);
	generateBtn.setFont(displayFont);
	generateBtn.setTextColor(ofColor::cadetBlue);
	generateBtn.setHoverColor(ofColor::antiqueWhite);
	generateBtn.setTitle("Generate");
	generateBtn.setID(1);

	//what gender are we generating names for
	genderBtn.setButtonStyle(UI_BUTTON_ROUNDED_RECT);
	genderBtn.setFont(displayFont);
	genderBtn.setTextColor(ofColor::cadetBlue);
	genderBtn.setHoverColor(ofColor::antiqueWhite);
	genderBtn.setTitle("Switch");
	genderBtn.setID(2);

	ofBackground(ofColor::cadetBlue);

	ofAddListener(nameMinSlider.sliderEvent, this, &ofApp::sliderChanged);
	ofAddListener(nameMaxSlider.sliderEvent, this, &ofApp::sliderChanged);
	ofAddListener(resultsSlider.sliderEvent, this, &ofApp::sliderChanged);
	ofAddListener(nameSeed.textChanged, this, &ofApp::textChanged);
	ofAddListener(generateBtn.buttonEvent, this, &ofApp::buttonClicked);
	ofAddListener(genderBtn.buttonEvent, this, &ofApp::buttonClicked);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255);
	displayFont.drawString("Set Min Name Length: " + ofToString(nameMinSlider.getValue()), 40, 25);
	nameMinSlider.draw();

	displayFont.drawString("Set Max Name Length: " + ofToString(nameMaxSlider.getValue()), 300, 25);
	nameMaxSlider.draw();

	nameSeed.draw();

	ofSetColor(ofColor::antiqueWhite);
	ofRectRounded(40, 90, 600, 300, 20);
	ofRectRounded(40, 460, 600, 125, 20);

	ofSetColor(ofColor::black);
	auto it = namesList.begin();
	for (int i = 0; i < 7; i++) {
		if (namesList.size() > i) {
			displayBigFont.drawString(*it++, 45, 125 + (40 * i));
		}
	}
	if (namesList.size() > 6) {
		for (int i = 0; i < 7; i++) {
			if (namesList.size() > i + 7) {
				displayBigFont.drawString(*it++, 225, 125 + (40 * i));
			}
		}
	}

	if (namesList.size() > 13) {
		for (int i = 0; i < 7; i++) {
			if (namesList.size() > i + 14) {
				displayBigFont.drawString(*it++, 405, 125 + (40 * i));
			}
		}
	}

	auto it2 = validNames.begin();
	for (int i = 0; i < 3; i++) {
		if (validNames.size() > i) {
			displayBigFont.drawString(*it2++, 90, 495 + (40 * i));
		}
	}
	if (validNames.size() > 3) {
		for (int i = 0; i < 3; i++) {
			if (validNames.size() > i + 3) {
				displayBigFont.drawString(*it2++, 270, 495 + (40 * i));
			}
		}
	}

	if (validNames.size() > 6) {
		for (int i = 0; i < 3; i++) {
			if (validNames.size() > i + 6) {
				displayBigFont.drawString(*it2++, 450, 495 + (40 * i));
			}
		}
	}

	ofSetColor(255);

	displayFont.drawString("# to Generate: " + ofToString(resultsSlider.getValue()), 400, 410);
	resultsSlider.draw();

	displayBigFont.drawString("Novel Names:", 45, 440);
	generateBtn.draw(650, 410, 130, 35);
	displayFont.drawString("Switch Gender\n    Names:", 650, 200);
	genderBtn.draw(650, 240, 130, 35);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::sliderChanged(const int & sliderID) {
	switch (sliderID) {
	case 0:
		//make sure the max slider isnt less than 3 more than the min
		if (nameMaxSlider.getValue() - 2 <= nameMinSlider.getValue()) {
			nameMaxSlider.setValue(nameMinSlider.getValue() + 3);
		}
		break;
	case 1:
		//make sure the min slider isnt less than 3 less than the max
		if (nameMaxSlider.getValue() - 2 <= nameMinSlider.getValue()) {
			nameMinSlider.setValue(nameMaxSlider.getValue() - 3);
		}
		break;
	default:
		break;
	}
}

void ofApp::textChanged(string & text)
{
	//limit the length to the min slider
	//otherwise the seed name wont allow for much variety in generation
	if (text.length() > nameMinSlider.getValue()) {
		nameSeed.setText(text.substr(0, nameMinSlider.getValue()));
	}

	namesList.clear();

	//only convert once, this slows down significantly if we do it every time
	string lower = ofToLower(text);

	//search boys names that contain the seed characters
	for (auto name : boysNames) {
		//is the name greater or equal to min length and the inverse for the max
		if (name.find(lower) != string::npos && name.length() >= nameMinSlider.getValue() && name.length() <= nameMaxSlider.getValue()) {
			namesList.insert(name);
		}
	}

	//search girls names that contain the seed characters
	for (auto name : girlsNames) {
		//is the name greater or equal to min length and the inverse for the max
		if (name.find(lower) != string::npos && name.length() >= nameMinSlider.getValue() && name.length() <= nameMaxSlider.getValue()) {
			namesList.insert(name);
		}
	}
}

void ofApp::buttonClicked(const pair<bool, int> & button)
{
	//generate names
	if (button.second == 1) {
		if (button.first) {
			//if for some reason the generator cannont generate the desired amount of names
			//we should give up after so many tries, its likely that someone input a seed
			//that has 0 variations possible
			int counter = 0;
			validNames.clear();
			//while the total valid names is less than the required output
			while (validNames.size() < resultsSlider.getValue() && counter < 500) {
				//prime the name generator
				string name = "__" + nameSeed.getText();
				//names longer than this will get tossed anyways and lets make sure we dont
				//generate infinitely long names
				while (name.length() < nameMaxSlider.getValue() + 4) {
					//since we are using chars we can just add to them to change the letter
					int mutator = characterMutator(name);
					//mutators can be between 0-25 ('a' + 25 = 'z') for valid letters
					//anything else means end of name so we break
					if (mutator < 26) {
						name.append(ofToString(static_cast<char>('a' + mutator)));
					}
					else {
						break;
					}
				}
				//even if the naem isnt valid we increment the total generated
				counter++;
				//is the name valid
				nameValidator(name);
			}
		}
	}
	//switching gender generator, switch colors to let user know
	else if (button.second == 2) {
		if (button.first) {
			gender = !gender;
			if (gender) {
				ofBackground(ofColor::cadetBlue);
				generateBtn.setTextColor(ofColor::cadetBlue);
				genderBtn.setTextColor(ofColor::cadetBlue);
			}
			else {
				ofBackground(ofColor::salmon);
				generateBtn.setTextColor(ofColor::salmon);
				genderBtn.setTextColor(ofColor::salmon);
			}
		}
	}

}


/**
Name Validator
checks if the name is greater or equal to minimum length
and less than or equal to the max. It also makes sure the
name is novel and does not already exist in our data

@param name to check for validness
*/
void ofApp::nameValidator(string name)
{
	name = name.substr(2); //drop the leading underscores
	if (name.length() >= nameMinSlider.getValue() && name.length() <= nameMaxSlider.getValue() && boysNames.count(name) == 0 && girlsNames.count(name) == 0) {
		//its valid, lets capitalize the first letter
		name = ofToString(static_cast<char>(toupper(name[0]))) + name.substr(1);
		//push the name to the valid name vector
		validNames.push_back(name);
	}
}

/**
Generate Character Mutator
uses the discrete distribution for the character mapping
to determine the next letter mutator

@param name to check for validness
@return the mutator value
*/
int ofApp::characterMutator(string name) {
	//we only want the last 2 characters of the name
	string subString = name.substr(name.length() - 2);
	if (gender) {
		//its possible the letter combination does not exist
		//in the mapping so lets check to see if it does
		if (boyNameCharDist.count(subString) > 0) {
			//use random number engine to produce our discrete distribution value
			return boyNameCharDist.at(subString)(generator);
		}
		else {
			//our mappings didnt have a valid character combination so
			//use the last letter to determine what follows
			subString = name.substr(name.length() - 1);
			if (boyNameCharDist.count("_" + subString) > 0) {
				return boyNameCharDist.at("_" + subString)(generator);
			}
			else {
				//somehow its not a valid character so return a value 
				//greater than 25 to break the name generation
				return 99;
			}
		}
	}
	else {
		//its possible the letter combination does not exist
		//in the mapping so lets check to see if it does
		if (girlNameCharDist.count(subString) > 0) {
			//use random number engine to produce our discrete distribution value
			return girlNameCharDist.at(subString)(generator);
		}
		else {
			//our mappings didnt have a valid character combination so
			//use the last letter to determine what follows
			subString = name.substr(name.length() - 1);
			if (girlNameCharDist.count("_" + subString) > 0) {
				return girlNameCharDist.at("_" + subString)(generator);
			}
			else {
				//somehow its not a valid character so return a value 
				//greater than 25 to break the name generation
				return 99;
			}
		}
	}
}