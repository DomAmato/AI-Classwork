#pragma once

#include "ofMain.h"
#include "ofxUIUtils.h"
#include <random>

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

		void sliderChanged(const int & sliderID);
		void textChanged(string & text);
		void buttonClicked(const pair<bool, int> & button);
		void nameValidator(string name);

		int characterMutator(string name);

		bool gender; //true boy, false girl
		
		set<string> boysNames;
		set<string> girlsNames;
		set<string> namesList;

		ofxUIIntSlider nameMinSlider;
		ofxUIIntSlider nameMaxSlider;
		ofxUIIntSlider resultsSlider;

		ofTrueTypeFont displayFont;
		ofTrueTypeFont displayBigFont;

		ofxTextInputField nameSeed;

		ofxUIButton generateBtn;
		ofxUIButton genderBtn;

		default_random_engine generator;
		vector<string> validNames;

		map<string, discrete_distribution<>> boyNameCharDist;
		map<string, discrete_distribution<>> girlNameCharDist;
};
