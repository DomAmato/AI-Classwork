#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	bg.loadImage("woodPattern.jpg");
	ofSetCircleResolution(360);

	font.load(OF_TTF_SANS, 36);
	btnFont.load(OF_TTF_SANS, 24);
	menuFont.load(OF_TTF_SANS, 21);

	reset.setButtonStyle(UI_Button_Style::UI_BUTTON_ROUNDED_RECT);
	reset.setTitle("Reset");
	reset.setColor(ofColor::antiqueWhite);
	reset.setHoverColor(ofColor::goldenRod);
	reset.setTextColor(ofColor(.08 * 255, .08 * 255, .08 * 255));
	reset.setFont(btnFont);
	reset.setID(0);

	start.setButtonStyle(UI_Button_Style::UI_BUTTON_ROUNDED_RECT);
	start.setTitle("Begin");
	start.setColor(ofColor::antiqueWhite);
	start.setHoverColor(ofColor::paleGreen);
	start.setTextColor(ofColor(.08 * 255, .08 * 255, .08 * 255));
	start.setFont(btnFont);
	start.setID(5);

	ai1PlyDn.setButtonStyle(UI_Button_Style::UI_BUTTON_ROUNDED_RECT);
	ai1PlyDn.setTitle("v");
	ai1PlyDn.setColor(ofColor::antiqueWhite);
	ai1PlyDn.setHoverColor(ofColor::paleVioletRed);
	ai1PlyDn.setTextColor(ofColor(.08 * 255, .08 * 255, .08 * 255));
	ai1PlyDn.setFont(btnFont);
	ai1PlyDn.setID(1);

	ai1PlyUp.setButtonStyle(UI_Button_Style::UI_BUTTON_ROUNDED_RECT);
	ai1PlyUp.setTitle("^");
	ai1PlyUp.setColor(ofColor::antiqueWhite);
	ai1PlyUp.setHoverColor(ofColor::paleGreen);
	ai1PlyUp.setTextColor(ofColor(.08 * 255, .08 * 255, .08 * 255));
	ai1PlyUp.setFont(btnFont);
	ai1PlyUp.setID(2);

	ai2PlyDn.setButtonStyle(UI_Button_Style::UI_BUTTON_ROUNDED_RECT);
	ai2PlyDn.setTitle("v");
	ai2PlyDn.setColor(ofColor::antiqueWhite);
	ai2PlyDn.setHoverColor(ofColor::paleVioletRed);
	ai2PlyDn.setTextColor(ofColor(.08 * 255, .08 * 255, .08 * 255));
	ai2PlyDn.setFont(btnFont);
	ai2PlyDn.setID(3);

	ai2PlyUp.setButtonStyle(UI_Button_Style::UI_BUTTON_ROUNDED_RECT);
	ai2PlyUp.setTitle("^");
	ai2PlyUp.setColor(ofColor::antiqueWhite);
	ai2PlyUp.setHoverColor(ofColor::paleGreen);
	ai2PlyUp.setTextColor(ofColor(.08 * 255, .08 * 255, .08 * 255));
	ai2PlyUp.setFont(btnFont);
	ai2PlyUp.setID(4);

	gameType.setButtonTitle("Game Mode");
	gameType.setPosition(ofGetWidth()*.835, 50);
	gameType.setButtonFont(menuFont);
	gameType.setHeight(35);
	gameType.addMenuItem("PvP");
	gameType.addMenuItem("PvAI");
	gameType.addMenuItem("AIvP");
	gameType.addMenuItem("AIvAI");

	ofAddListener(reset.buttonEvent, this, &ofApp::UIButPressed);
	ofAddListener(start.buttonEvent, this, &ofApp::UIButPressed);
	ofAddListener(ai1PlyDn.buttonEvent, this, &ofApp::UIButPressed);
	ofAddListener(ai1PlyUp.buttonEvent, this, &ofApp::UIButPressed);
	ofAddListener(ai2PlyDn.buttonEvent, this, &ofApp::UIButPressed);
	ofAddListener(ai2PlyUp.buttonEvent, this, &ofApp::UIButPressed);

	ofAddListener(gameType.menuEvent, this, &ofApp::UIMenuSelection);

	currentplayer = false;
	numPlysAI1 = 5;
	numPlysAI2 = 3;

	gamemode = 0;
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (beginAI) {
		miniMax.performMiniMaxAB(board, currentplayer, currentplayer ? numPlysAI1 : numPlysAI2);
		currentplayer = !currentplayer;
	}

	bg.draw(0, 0, ofGetWidth(), ofGetHeight());

	ofFill();
	ofSetColor(ofColor::antiqueWhite);
	ofRectRounded(80, 80, ofGetWidth()*.75, ofGetHeight()*.85, 15);
	ofRectRounded(ofGetWidth()*.835, 140, ofGetWidth()*.25, 280, 15);
	ofNoFill();
	ofSetColor(ofColor::saddleBrown);
	ofSetLineWidth(4);
	ofRectRounded(100, 100, ofGetWidth()*.75 - 40, ofGetHeight()*.85 - 40, 15);

	double xspacing = (ofGetWidth()*.75 - 40) / 9;
	double yspacing = (ofGetHeight()*.85 - 40) / 9;
	for (int i = 1; i < 9; i++) {
		ofLine(100 + xspacing*i, 100, 100 + xspacing*i, ofGetHeight()*.85 + 60);
		ofLine(100, 100 + yspacing*i, ofGetWidth()*.75 + 60, 100 + yspacing*i);
	}

	ofFill();
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (board.getSpace(i, 8 - j) != 0) {
				ofSetColor(80, 80, 80, 60);
				ofCircle(100 + xspacing*i + (xspacing / 2) + 3, 100 + yspacing*(j)+(yspacing / 2) + 3, yspacing / 2);
				ofSetColor(board.getSpace(i, 8 - j) == 1 ? ofColor::whiteSmoke : ofColor(.08 * 255, .08 * 255, .08 * 255));
				ofCircle(100 + xspacing*i + (xspacing / 2), 100 + yspacing*(j)+(yspacing / 2), yspacing / 2 - 3);
			}
		}
	}

	auto score = board.getScore();

	ofSetColor(80, 80, 80, 80);
	font.drawString("Black:\n  " + ofToString(score.first), ofGetWidth()*.85 + 2, 202);
	font.drawString("White:\n  " + ofToString(score.second), ofGetWidth()*.85 + 2, 322);

	ofSetColor(.08 * 255, .08 * 255, .08 * 255);
	font.drawString("Black:\n  " + ofToString(score.first), ofGetWidth()*.85, 200);
	if (gamemode > 0) {
		font.drawString(ofToString(numPlysAI1), numPlysAI1 > 9 ? ofGetWidth()*.89 : ofGetWidth()*.905, 485);
	}
	ofSetColor(ofColor::whiteSmoke);
	font.drawString("White:\n  " + ofToString(score.second), ofGetWidth()*.85, 320);

	if (gamemode == 3) {
		font.drawString(ofToString(numPlysAI2), numPlysAI2 > 9 ? ofGetWidth()*.89 : ofGetWidth()*.905, 555);
		ofSetColor(255);
		start.draw(ofGetWidth()*.845, 580, 140, 50);
		ai2PlyDn.draw(ofGetWidth()*.845, 520, 45, 35);
		ai2PlyUp.draw(ofGetWidth()*.945, 520, 45, 35);
	}
	ofSetColor(255);
	if (gamemode > 0) {
		ai1PlyDn.draw(ofGetWidth()*.845, 450, 45, 35);
		ai1PlyUp.draw(ofGetWidth()*.945, 450, 45, 35);
	}
	reset.draw(ofGetWidth()*.845, 650, 140, 50);

	gameType.draw();

	if (aiDeciding) {
		ofSetColor(80, 80, 80, 120);
		ofRectRounded(130, ofGetHeight() *.325, ofGetWidth() *.675, ofGetHeight() *.3, 15);
		ofSetColor(ofColor::whiteSmoke);
		string loading = "AI Considering Next Move....";
		loading = loading.substr(0, ofGetElapsedTimeMillis() % (50 * loading.size() - 1) / 50);
		font.drawString(loading, ofGetWidth() * .175, ofGetHeight() *.5);
	}
	ofSetColor(255);
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
	if (button == 0 && !board.isFull() && !aiDeciding) {
		if (y > 100 && y < ofGetHeight()*.85 + 60) {
			int c = floor((x - 100) / ((ofGetWidth()*.75 - 40) / 9));
			if (c >= 0 && c <= 8) {
				board.placeInColumn(c);
				if (gamemode > 0) {
					aiDeciding = true;
					searchThread = new thread([&]() {
						miniMax.performMiniMaxAB(board, !currentplayer, numPlysAI1);
						aiDeciding = false;
					});
				}
			}
		}
	}
	/*if (button == 2 && !board.isFull()) {
		if (y > 100 && y < ofGetHeight()*.85 + 60) {
			int c = floor((x - 100) / ((ofGetWidth()*.75 - 40) / 9));
			if (c >= 0 && c <= 8) {
				board.placeInColumn(c);
				miniMax.performMiniMax(board, true, numPlysAI1);
			}
		}
	}*/

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

void ofApp::UIButPressed(const pair<bool, int> & state) {
	switch (state.second) {
	case 0:
		if (state.first) {
			board.clearBoard();
			currentplayer = false;
			if (gamemode > 1) {
				board.placeInColumn(ofRandom(8));
				currentplayer = true;
			}
			beginAI = false;
		}
		break;
	case 1:
		if (state.first) {
			numPlysAI1--;
		}
		break;
	case 2:
		if (state.first) {
			numPlysAI1++;
		}
		break;
	case 3:
		if (state.first) {
			numPlysAI2--;
		}
		break;
	case 4:
		if (state.first) {
			numPlysAI2++;
		}
		break;
	case 5:
		beginAI = true;
		break;
	}

}

void ofApp::UIMenuSelection(const tuple<int, string, int> & selection) {
	gamemode = get<2>(selection);
	board.clearBoard();
	currentplayer = false;
	if (gamemode > 1) {
		board.placeInColumn(ofRandom(8));
		currentplayer = true;
	}
}