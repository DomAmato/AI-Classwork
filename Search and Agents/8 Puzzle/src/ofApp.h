#pragma once

#include "ofMain.h"
#include "ofxUIUtils.h"
#include "ofBitmapFont.h"
#include "Assignment1.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofTrueTypeFont buttonFont;
		ofTrueTypeFont puzzleFont;
		ofxDropDownMenu successorOrderMenu;
		ofxDropDownMenu difficultyMenu;
		ofxDropDownMenu searchTypeMenu;
		ofxUIButton run;

		ofxUIButton next, back;

		const Puzzle * result, * traverseP;
		Puzzle initalP;

		thread * searchThread;

		SuccessorOrder successOrder;

		int length, cost, time, space;
		double runTime;

		string order, difficulty, searchType;

		Assignment1 a;

		void UIMenuSelection(const tuple<int, string, int> & selection);
		void UIButPressed(const pair<bool, int> & state);
};
