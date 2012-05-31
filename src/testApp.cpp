#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	//Webcam
	ofBackground(0, 0, 0);
	
	
	w = 320;
	h = 240;
	
	Webcam.initGrabber(w, h, true);
	//Webcam.videoSettings();
	
	//reserve memory for cv images
	rgb.allocate(w, h);
	hsb.allocate(w, h);
	hue.allocate(w, h);
	sat.allocate(w, h);
	bri.allocate(w, h);
	filtered.allocate(w, h);
	
	
	//Sound
	synth.loadSound("sounds/synth.wav");
	drums.loadSound("sounds/beat.wav");
	vocals.loadSound("sounds/Violet.mp3");
	synth.setVolume(0.75f);
	drums.setVolume(0.75f);
	vocals.setVolume(0.75f);
	font.loadFont("Sudbury_Basin_3D.ttf", 32);
	drums.setMultiPlay(false);
	vocals.setMultiPlay(true);
	
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	//Webcam
	
	
	Webcam.update();
	
	if (Webcam.isFrameNew()) {

		//copy webcam pixels to rgb image
		rgb.setFromPixels(Webcam.getPixels(), w, h);
		
		//mirror horizontal
		rgb.mirror(false, true);
		
		//duplicate rgb
		hsb = rgb;
		
		//convert to hsb
		hsb.convertRgbToHsv();
		

		//store the three channels as crayscale images
		hsb.convertToGrayscalePlanarImages(hue, sat, bri);
		
		//filter image based on the hue value were looking for
		for (int i=0; i<w*h; i++) {
			filtered.getPixels()[i] = ofInRange(hue.getPixels()[i], findHue-5, findHue+5) ? 255 : 0;
		}
		
		filtered.flagImageChanged();
		

		//run the contour finder on the filtered image to find blobs with a certain hue
		contours.findContours(filtered, 50, w*h/2, 1, false);
		
	}	
	
	
	//Sound
	
	//ofBackground(255, 200, 255);
	
	// update the sound playing system
	ofSoundUpdate();
}

//--------------------------------------------------------------
void testApp::draw() {
	
	//Webcam
	
		ofSetColor(255, 255, 255);
	
	//draw all cv images
	rgb.draw(0,0);
	hsb.draw(640,0);
	hue.draw(0,240);
	sat.draw(320,240);
	bri.draw(640,240);
	filtered.draw(0,480);
	contours.draw(0,480);

	
	ofSetColor(255, 0, 0);
	ofFill();
	

	//draw red circels for found blobs
	for (int i=0; i<contours.nBlobs; i++) {
		ofCircle(contours.blobs[i].centroid.x, contours.blobs[i].centroid.y, 10);	
	}

	
	// Sound
	
	char tempStr[255];
	//draw the background colors:
	float widthDiv = ofGetWidth() /3.0f;
	/*ofSetHexColor(0xeeeeee);
	ofRect(0,0,widthDiv,ofGetHeight()); 
	ofSetHexColor(0xffffff);
	ofRect(widthDiv,0,widthDiv,ofGetHeight()); 
	ofSetHexColor(0xdddddd);
	ofRect(widthDiv*2,0,widthDiv,ofGetHeight()); 
	*/
	// draw synth
	if (synth.getIsPlaying()) ofSetHexColor(0xFF0000);
	else ofSetHexColor(0x000000);
	font.drawString("synth !!", 50,50);
	
	ofSetHexColor(0x000000);
	sprintf(tempStr, "click to play\npct done: %f\nspeed: %f\npan: %f", synth.getPosition(),  synth.getSpeed(), synth.getPan());
	ofDrawBitmapString(tempStr, 50,ofGetHeight()-50);
	
	//---------------------------------- drums:
	if (drums.getIsPlaying()) ofSetHexColor(0xFF0000);
	else ofSetHexColor(0x000000);
	font.drawString("drums !!", widthDiv+50,50);
	
	ofSetHexColor(0x000000);
	sprintf(tempStr, "click and drag\npct done: %f\nspeed: %f", drums.getPosition(),  drums.getSpeed());
	ofDrawBitmapString(tempStr, widthDiv+50,ofGetHeight()-50);
	
	//---------------------------------- vocals:
	if (vocals.getIsPlaying()) ofSetHexColor(0xFF0000);
	else ofSetHexColor(0x000000);
	font.drawString("vocals !!", widthDiv*2+50,50);
	
	ofSetHexColor(0x000000);
	sprintf(tempStr, "click to play (multiplay)\npct done: %f\nspeed: %f", vocals.getPosition(),  vocals.getSpeed());
	ofDrawBitmapString(tempStr, widthDiv*2+50,ofGetHeight()-50);
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	//Sound
	// continuously control the speed of the beat sample via drag, 
	// when in the "beat" region:
	float widthStep = ofGetWidth() / 3.0f;
	if (x >= widthStep && x < widthStep*2){
		drums.setSpeed( 0.5f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*1.0f);
	} 

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	//Webcam
	
	//calculate local mouse x,y in image

	int mx = x % w;
	int my = y % h;
	
	//get hue value on mouse position
	findHue = hue.getPixels()[my*w+mx];
	
	 
	
	//Sound
	// Set mousetrigger postitions for sample playing
	float widthStep = ofGetWidth() / 3.0f;
	if (x < widthStep){
		float pct = x / widthStep;
		synth.play();
		synth.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*10);
		synth.setPan(pct);
	} else if (x >= widthStep && x < widthStep*2){
		drums.play();
	} else {
		vocals.play();
		vocals.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*3);
		vocals.setPan((float)x / (float)ofGetWidth());	
	}


}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}