#include "ofMain.h"
#include "testApp.h"
#include "common.h"

//========================================================================
int main()
{
	ofSetupOpenGL(WIDTH,HEIGHT,OF_WINDOW);
	ofRunApp(new testApp());

}