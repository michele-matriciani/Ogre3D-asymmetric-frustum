#include "Filter.h"

#include <vector>
#include <iostream>



Filter::Filter() 
    
{
}



bool Filter::filter(Ogre::Vector2 &cVec, int erMin, int erMax,int gap) {
	int coord = cVec.x;
	int p	  = cVec.y;

	if (coord == 0) {
		cVec.x = p;
		return true;
	}

	int dif = abs( coord - p );
    //int dif = ( coord - p );

	if ( ( dif <= erMin )  ) { 
    	
		coord = p;    
    	
    }
    /*
    else if ( dif >= -erMax && dif <= erMax )  {

    	if ( dif < 0 ) {
    		coord = p - gap;
    	}
    	else {
    		coord = p + gap;
    	}
    	//p = coord;
    	

    }  */
    else {
    	p = coord;
    	
    }  

    cVec.x = coord;
    cVec.y= p;

    return true;

}



