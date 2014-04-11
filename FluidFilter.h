#pragma once

#include <OgreVector2.h>
//#include <queue>

 	class FluidFilter {
 		public:
 			FluidFilter(int,int,int);

 			bool interpolate(int);
 			Ogre::Real getLast();
 			void init(int);

 		private:
 			int n;
 			int m;
 			std::queue<Ogre::Real> q;
 	};
 