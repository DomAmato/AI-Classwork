#pragma once

#include "ofMain.h"
#include "Assignment2.h"
#include "ofxUIUtils.h"

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
		void UIButPressed(const pair<bool, int>& state);
		void UIMenuSelection(const tuple<int, string, int>& selection);
		void gotMessage(ofMessage msg);
		
		ofImage bg;
		Board board;

		ofTrueTypeFont font;
		ofTrueTypeFont btnFont;
		ofTrueTypeFont menuFont;

		ofxDropDownMenu gameType;

		ofxUIButton reset;
		ofxUIButton start;
		ofxUIButton ai1PlyUp;
		ofxUIButton ai1PlyDn;
		ofxUIButton ai2PlyUp;
		ofxUIButton ai2PlyDn;

		Assignment2 miniMax;

		bool currentplayer;
		bool beginAI;

		int numPlysAI1;
		int numPlysAI2;

		int gamemode;

		bool aiDeciding;

		thread * searchThread;
};
