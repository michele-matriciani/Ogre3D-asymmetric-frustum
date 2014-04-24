#include "FluidFilter.h"

#include <iostream>


FluidFilter::FluidFilter(int step,int begin):
    n     (step),
    gap   (begin),
    count (0),
    last  (begin),
    lastCoord (begin)

{ 
}

Ogre::Real FluidFilter::interpolate(Ogre::Real c) {
  
    // std::cout << last << " , " << c 
    // << " , " << lastR << " , " << gap << std::endl; 
 
    if ( lastCoord == c ) {

        if (count == 1) {
            std::cout << "CIAO" << std::endl;
            count--;
            last = c;
        }
        else if ( count > 0) {
            count--;
            last += gap;
        }
    }

    else {

        lastCoord = c;
        count = n;
        gap   =  ( c - last ) / n ;
        last += gap;
    }
    
    return last;
}