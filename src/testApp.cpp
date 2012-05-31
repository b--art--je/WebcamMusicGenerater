#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//Webcam.initGrabber(1024, 768);
	//Webcam.videoSettings();
	
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
	
}

//--------------------------------------------------------------
void testApp::update(){
	
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
			filtered.getPixels() [i] = ofInRange(hue.getPixels()[i], findHue-5, findHue+5) ? 255 : 0;
		}
		
		filtered.flagImageChanged();
		

		//run the contour finder on the filtered image to find blobs with a certain hue
		contours.findContours(filtered, 50, w*h/2, 1, false);
		
	}	
}

//--------------------------------------------------------------
void testApp::draw() {
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
		ofCircle(contours.blobs[i].centroid.x, contours.blobs[i].centroid.y, 20);	
	}
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

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
	//calculate local mouse x,y in image

	int mx = x % w;
	int my = y % h;
	
	//get hue value on mouse position
	findHue = hue.getPixels()[my*w+mx];

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