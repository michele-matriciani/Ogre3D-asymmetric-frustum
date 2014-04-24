#pragma once

#include <OgreVector2.h>
//#include <queue>

 	class FluidFilter {
 		public:
 			FluidFilter(int,int);

 			Ogre::Real interpolate(Ogre::Real);
 			

 		private:
 			int n,count,lastCoord;
 			Ogre::Real gap,last;
 			
 			
 	};
 