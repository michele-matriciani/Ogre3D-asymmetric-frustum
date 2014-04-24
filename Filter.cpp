#include "Filter.h"

#include <vector>
#include <iostream>



Filter::Filter() 
    
{
}



void Filter::filter(Ogre::Vector2 &cVec, int erMin, int erMax) {
	int coord = cVec.x;
	int p	  = cVec.y;

	if (coord == 0) {
		cVec.x = p;
		return;
	}

	int dif = abs( coord - p );
    

	if ( dif <= erMin){   	
		cVec.x = p;     	
    }
    else {
    	cVec.y = coord;	
    }  

}



