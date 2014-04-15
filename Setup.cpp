#include "Setup.h"
#include "MyConfig.h"

#include <iostream>



Setup::Setup()
	
{ 

}

void Setup::loadDefaultScene() {
	l = -8;
	r =  8;
	t =  9;
	b =  0;
	n =  2;
	f =  12;
	nP = 11;

	light = true;
	sceneName = "living-room.scene";

	xCam = 640;
	yCam = 480;
	difCol = Ogre::ColourValue(1.0,1.0,1.0);
	specCol = Ogre::ColourValue(1.0,1.0,1.0);
}

void Setup::readSetup(){
	bool ok = true;

	sceneName = MyConfig::getInstance().
			getValueAsString("Scene/sceneName");

	l = MyConfig::getInstance().
			getValueAsReal("Scene/leftCoordinate");
		
	r = MyConfig::getInstance().
			getValueAsReal("Scene/rightCoordinate");
		
	t = MyConfig::getInstance().
			getValueAsReal("Scene/upperCoordinate");
		
	b = MyConfig::getInstance().
			getValueAsReal("Scene/lowerCoordinate");
		
	n = MyConfig::getInstance().
			getValueAsReal("Scene/beginCoordinate");
		
	f = MyConfig::getInstance().
			getValueAsReal("Scene/endCoordinate");
		
	nP = MyConfig::getInstance().
			getValueAsReal("Scene/defaultNearPlane");
		
	light = MyConfig::getInstance().
			getValueAsBool("Light/defaultLight");
		
	xCam = MyConfig::getInstance().
			getValueAsInt("WebcamResolution/width");
		
	yCam = MyConfig::getInstance().
			getValueAsInt("WebcamResolution/height");
		
	if (light) {
	difCol = MyConfig::getInstance().
			getValueAsColourValue("Light/diffuseColor");

	specCol = MyConfig::getInstance().
			getValueAsColourValue("Light/specularColor");
	}

}


Ogre::String Setup::getSceneName() {
 	return sceneName;
}
Ogre::Real Setup::getL() {
 	return l;
}
Ogre::Real Setup::getR() {
 	return r;
}
Ogre::Real Setup::getT() {
 	return t;
}
Ogre::Real Setup::getB() {
 	return b;
}
Ogre::Real Setup::getN() {
 	return n;
}
Ogre::Real Setup::getF() {
 	return f;
}
Ogre::Real Setup::getNP() {
 	return nP;
}
bool Setup::getLight() {
 	return light;
}
int Setup::getXCam() {
 	return xCam;
}
int Setup::getYCam() {
 	return yCam;
}
Ogre::ColourValue Setup::getDifCol(){
	return difCol;
}
Ogre::ColourValue Setup::getSpecCol(){
	return specCol;
}