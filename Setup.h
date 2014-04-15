#pragma once

#include <OgreVector3.h>

#include <OgreColourValue.h>


 	class Setup {
 		public:
 			Setup();

 			void loadDefaultScene();
 			void readSetup();

 			Ogre::String getSceneName();
 			Ogre::Real getL();
 			Ogre::Real getR();
 			Ogre::Real getT();
 			Ogre::Real getB();
 			Ogre::Real getN();
 			Ogre::Real getF();
 			Ogre::Real getNP();
 			bool getLight();
 			int getXCam();
 			int getYCam();
 			Ogre::ColourValue getDifCol();
 			Ogre::ColourValue getSpecCol();

 			//Ogre::Real interpolate(Ogre::Real);
 			

 		private:
 			bool light;
 			Ogre::Real l,r,t,b,n,f,nP;
 			Ogre::String sceneName;
 			int xCam, yCam;
 			Ogre::ColourValue difCol,specCol;
 
 			
 	};
