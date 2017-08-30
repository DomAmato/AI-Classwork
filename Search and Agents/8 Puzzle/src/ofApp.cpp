#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(ofColor::darkSlateBlue);

	buttonFont.loadFont(OF_TTF_SANS, 16);
	puzzleFont.loadFont(OF_TTF_SANS, 28);

	initalP = EASY;

	successOrder.setOrder(Orders::E);

	difficulty = "Easy";
	order = "E";

	searchType = "Breadth-First Search";

	run.setButtonStyle(UI_BUTTON_ROUNDED_RECT);
	run.setColor(ofColor::forestGreen);
	run.setHoverColor(ofColor::cadetBlue);
	run.setTextColor(ofColor::antiqueWhite);
	run.setTitle("Run");
	run.setFont(buttonFont);
	run.setID(1);

	successorOrderMenu.setPosition(30, 30);
	successorOrderMenu.setAutoSizing(true);
	successorOrderMenu.setHeight(30);
	successorOrderMenu.setButtonTitle("Set Selector Order");
	successorOrderMenu.addMenuItem("A");
	successorOrderMenu.addMenuItem("B");
	successorOrderMenu.addMenuItem("C");
	successorOrderMenu.addMenuItem("D");
	successorOrderMenu.addMenuItem("E");
	successorOrderMenu.addMenuItem("F");
	successorOrderMenu.addMenuItem("G");
	successorOrderMenu.addMenuItem("H");
	successorOrderMenu.addMenuItem("I");
	successorOrderMenu.addMenuItem("J");
	successorOrderMenu.addMenuItem("K");
	successorOrderMenu.addMenuItem("L");
	successorOrderMenu.addMenuItem("M");
	successorOrderMenu.addMenuItem("N");
	successorOrderMenu.addMenuItem("O");
	successorOrderMenu.addMenuItem("P");
	successorOrderMenu.addMenuItem("Q");
	successorOrderMenu.addMenuItem("R");
	successorOrderMenu.addMenuItem("S");
	successorOrderMenu.addMenuItem("T");
	successorOrderMenu.addMenuItem("U");
	successorOrderMenu.addMenuItem("V");
	successorOrderMenu.addMenuItem("W");
	successorOrderMenu.addMenuItem("X");
	successorOrderMenu.setID(0);

	difficultyMenu.setPosition(30, 70);
	difficultyMenu.setAutoSizing(true);
	difficultyMenu.setHeight(30);
	difficultyMenu.setButtonTitle("Set Difficulty Level");
	difficultyMenu.addMenuItem("Easy");
	difficultyMenu.addMenuItem("Medium");
	difficultyMenu.addMenuItem("Hard");
	difficultyMenu.setID(1);

	searchTypeMenu.setPosition(30, 110);
	searchTypeMenu.setAutoSizing(true);
	searchTypeMenu.setHeight(30);
	searchTypeMenu.setButtonTitle("Set Search Type");
	searchTypeMenu.addMenuItem("Breadth-First Search");
	searchTypeMenu.addMenuItem("Depth-First Search");
	searchTypeMenu.addMenuItem("Iterative Deepening");
	searchTypeMenu.addMenuItem("Uniform Cost");
	searchTypeMenu.addMenuItem("Greedy Best First");
	searchTypeMenu.addMenuItem("A* Misplaced");
	searchTypeMenu.addMenuItem("A* Manhatten");
	searchTypeMenu.addMenuItem("A* Hybrid");
	searchTypeMenu.setID(2);

	next.setButtonStyle(UI_BUTTON_ROUNDED_RECT);
	next.setColor(ofColor::lightGrey);
	next.setHoverColor(ofColor::darkGrey);
	next.setTextColor(ofColor::black);
	next.setTitle(">>");
	next.setFont(buttonFont);
	next.setID(2);

	back.setButtonStyle(UI_BUTTON_ROUNDED_RECT);
	back.setColor(ofColor::lightGrey);
	back.setHoverColor(ofColor::darkGrey);
	back.setTextColor(ofColor::black);
	back.setTitle("<<");
	back.setFont(buttonFont);
	back.setID(3);

	ofAddListener(successorOrderMenu.menuEvent, this, &ofApp::UIMenuSelection);
	ofAddListener(difficultyMenu.menuEvent, this, &ofApp::UIMenuSelection);
	ofAddListener(searchTypeMenu.menuEvent, this, &ofApp::UIMenuSelection);

	ofAddListener(run.buttonEvent, this, &ofApp::UIButPressed);
	ofAddListener(next.buttonEvent, this, &ofApp::UIButPressed);
	ofAddListener(back.buttonEvent, this, &ofApp::UIButPressed);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	back.draw(ofGetWidth() * .45, 360, 50, 35);
	next.draw(ofGetWidth() * .3 + 300, 360, 50, 35);

	run.draw(ofGetWidth() - 150, ofGetHeight() - 55, 120, 35);

	ofSetLineWidth(3);
	ofSetColor(ofColor::black);

	buttonFont.drawString("Length: " + ofToString(length), 30, 170);
	buttonFont.drawString("Cost: " + ofToString(cost), 30, 200);
	buttonFont.drawString("Time: " + ofToString(time), 30, 230);
	buttonFont.drawString("Space: " + ofToString(space), 30, 260);
	buttonFont.drawString("Run Time: " + ofToString(runTime), 30, 290);

	buttonFont.drawString("Difficulty: " + difficulty, 30, 380);
	buttonFont.drawString("Order: " + order, 30, 410);
	buttonFont.drawString("Search Type: " + searchType, 30, 440);

	double gridX = ofGetWidth() * .41;

	ofSetColor(ofColor::antiqueWhite);
	ofRect(gridX, 30, 300, 300);
	ofSetColor(ofColor::black);

	double heightOffset = puzzleFont.getLineHeight() / 3;
	double widthOffset = puzzleFont.stringWidth("8") / 2;

	if (traverseP != NULL) {
		puzzleFont.drawString(to_string((*traverseP)[0][0]), gridX + 50 - widthOffset, 80 + heightOffset);
		puzzleFont.drawString(to_string((*traverseP)[0][1]), gridX + 150 - widthOffset, 80 + heightOffset);
		puzzleFont.drawString(to_string((*traverseP)[0][2]), gridX + 250 - widthOffset, 80 + heightOffset);
		puzzleFont.drawString(to_string((*traverseP)[1][0]), gridX + 50 - widthOffset, 180 + heightOffset);
		puzzleFont.drawString(to_string((*traverseP)[1][1]), gridX + 150 - widthOffset, 180 + heightOffset);
		puzzleFont.drawString(to_string((*traverseP)[1][2]), gridX + 250 - widthOffset, 180 + heightOffset);
		puzzleFont.drawString(to_string((*traverseP)[2][0]), gridX + 50 - widthOffset, 280 + heightOffset);
		puzzleFont.drawString(to_string((*traverseP)[2][1]), gridX + 150 - widthOffset, 280 + heightOffset);
		puzzleFont.drawString(to_string((*traverseP)[2][2]), gridX + 250 - widthOffset, 280 + heightOffset);
	}

	ofNoFill();
	ofRect(gridX, 30, 300, 300);
	ofRect(gridX, 30, 300, 200);
	ofRect(gridX, 30, 300, 100);
	ofRect(gridX, 30, 200, 300);
	ofRect(gridX, 30, 100, 300);
	ofFill();

	ofSetColor(255);

	searchTypeMenu.draw();
	difficultyMenu.draw();
	successorOrderMenu.draw();
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

void ofApp::UIMenuSelection(const tuple<int, string, int> & selection) {
	switch (get<0>(selection)) {
	case(0):
		order = get<1>(selection);
		if (get<1>(selection) == "A") {
			successOrder.setOrder(Orders::A);
		}
		if (get<1>(selection) == "B") {
			successOrder.setOrder(Orders::B);
		}
		if (get<1>(selection) == "C") {
			successOrder.setOrder(Orders::C);
		}
		if (get<1>(selection) == "D") {
			successOrder.setOrder(Orders::D);
		}
		if (get<1>(selection) == "E") {
			successOrder.setOrder(Orders::E);
		}
		if (get<1>(selection) == "F") {
			successOrder.setOrder(Orders::F);
		}
		if (get<1>(selection) == "G") {
			successOrder.setOrder(Orders::G);
		}
		if (get<1>(selection) == "H") {
			successOrder.setOrder(Orders::H);
		}
		if (get<1>(selection) == "I") {
			successOrder.setOrder(Orders::I);
		}
		if (get<1>(selection) == "J") {
			successOrder.setOrder(Orders::J);
		}
		if (get<1>(selection) == "K") {
			successOrder.setOrder(Orders::K);
		}
		if (get<1>(selection) == "L") {
			successOrder.setOrder(Orders::L);
		}
		if (get<1>(selection) == "M") {
			successOrder.setOrder(Orders::M);
		}
		if (get<1>(selection) == "N") {
			successOrder.setOrder(Orders::N);
		}
		if (get<1>(selection) == "O") {
			successOrder.setOrder(Orders::O);
		}
		if (get<1>(selection) == "P") {
			successOrder.setOrder(Orders::P);
		}
		if (get<1>(selection) == "Q") {
			successOrder.setOrder(Orders::Q);
		}
		if (get<1>(selection) == "R") {
			successOrder.setOrder(Orders::R);
		}
		if (get<1>(selection) == "S") {
			successOrder.setOrder(Orders::S);
		}
		if (get<1>(selection) == "T") {
			successOrder.setOrder(Orders::T);
		}
		if (get<1>(selection) == "U") {
			successOrder.setOrder(Orders::U);
		}
		if (get<1>(selection) == "V") {
			successOrder.setOrder(Orders::V);
		}
		if (get<1>(selection) == "W") {
			successOrder.setOrder(Orders::W);
		}
		if (get<1>(selection) == "X") {
			successOrder.setOrder(Orders::X);
		}
		break;
	case(1): //difficulty
		difficulty = get<1>(selection);
		if (get<2>(selection) == 0) {
			initalP = EASY;
		}
		else if (get<2>(selection) == 1) {
			initalP = MEDIUM;
		}
		else if (get<2>(selection) == 2) {
			initalP = HARD;
		}
		break;
	case(2):
		searchType = get<1>(selection);
		break;
	}
}

void ofApp::UIButPressed(const pair<bool, int> & state) {
	switch (state.second) {
	case(1)://run
		if (state.first) {
			if (searchType == "Breadth-First Search") {
				searchThread = new thread([&]() {
					auto val = a.breadthFirst(initalP, successOrder);
					result = get<0>(val);
					traverseP = result;
					length = get<2>(val);
					cost = get<1>(val);
					time = get<3>(val);
					space = get<5>(val);
					runTime = get<4>(val);
				});
			}
			else if (searchType == "Depth-First Search") {
				searchThread = new thread([&]() {
					auto val = a.depthFirst(initalP, successOrder);
					result = get<0>(val);
					traverseP = result;
					length = get<2>(val);
					cost = get<1>(val);
					time = get<3>(val);
					space = get<5>(val);
					runTime = get<4>(val);
				});
			}
			else if (searchType == "Iterative Deepening") {
				searchThread = new thread([&]() {
					auto val = a.iterativeDeepening(initalP, successOrder);
					result = get<0>(val);
					traverseP = result;
					length = get<2>(val);
					cost = get<1>(val);
					time = get<3>(val);
					space = get<5>(val);
					runTime = get<4>(val);
				});
			}
			else if (searchType == "Uniform Cost") {
				searchThread = new thread([&]() {
					auto val = a.uniformCost(initalP, successOrder);
					result = get<0>(val);
					traverseP = result;
					length = get<2>(val);
					cost = get<1>(val);
					time = get<3>(val);
					space = get<5>(val);
					runTime = get<4>(val);
				});
			}
			else if (searchType == "Greedy Best First") {
				searchThread = new thread([&]() {
					auto val = a.bestFirst(initalP, successOrder);
					result = get<0>(val);
					traverseP = result;
					length = get<2>(val);
					cost = get<1>(val);
					time = get<3>(val);
					space = get<5>(val);
					runTime = get<4>(val);
				});
			}
			else if (searchType == "A* Misplaced") {
				searchThread = new thread([&]() {
					auto val = a.aStar1(initalP, successOrder);
					result = get<0>(val);
					traverseP = result;
					length = get<2>(val);
					cost = get<1>(val);
					time = get<3>(val);
					space = get<5>(val);
					runTime = get<4>(val);
				});
			}
			else if (searchType == "A* Manhatten") {
				searchThread = new thread([&]() {
					auto val = a.aStar2(initalP, successOrder);
					result = get<0>(val);
					traverseP = result;
					length = get<2>(val);
					cost = get<1>(val);
					time = get<3>(val);
					space = get<5>(val);
					runTime = get<4>(val);
				});
			}
			else if (searchType == "A* Hybrid") {
				searchThread = new thread([&]() {
					auto val = a.aStar3(initalP, successOrder);
					result = get<0>(val);
					traverseP = result;
					length = get<2>(val);
					cost = get<1>(val);
					time = get<3>(val);
					space = get<5>(val);
					runTime = get<4>(val);
				});
			}
		}
		break;
	case(2):
		if (state.first) {//next
			if (traverseP->getParent() != NULL && traverseP->getParent() != nullptr) {
				traverseP = traverseP->getParent();
			}
		}
		break;
	case(3):
		if (state.first) {
			const Puzzle * tempPuzzle = result;
			while (tempPuzzle != traverseP && tempPuzzle->getParent() != traverseP) {
				tempPuzzle = tempPuzzle->getParent();
			}
			traverseP = tempPuzzle;
		}
		break;
	}
}