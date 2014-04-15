#include "Filter.h"

#include <vector>
#include <iostream>



Filter::Filter() 
    
{
}



bool Filter::filter(Ogre::Vector2 &cVec, int erMin, int erMax) {
	int coord = cVec.x;
	int p	  = cVec.y;

	if (coord == 0) {
		cVec.x = p;
		return true;
	}

	int dif = abs( coord - p );
    

	if ( dif <= erMin ) {   	
		cVec.x = p;     	
    }
    else {
    	cVec.y = coord;	
    }  

    return true;

}



