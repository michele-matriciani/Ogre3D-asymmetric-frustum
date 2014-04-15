#include "FluidFilter.h"

#include <iostream>


FluidFilter::FluidFilter(int step,int begin):
    n     (step),
    gap   (begin),
    count (0),
    last  (begin),
    lastR (begin)

{ 
}

Ogre::Real FluidFilter::interpolate(Ogre::Real c) {
  
    // std::cout << last << " , " << c 
    // << " , " << lastR << " , " << gap << std::endl; 
 
    if ( lastR == c ) {

        if (count == 1) {
            count--;
            last = c;
        }
        else if ( count > 0) {
            count--;
            last += gap;
        }
    }

    else {

        lastR = c;
        count = n;
        gap   =  ( c - last ) / n ;
        last += gap;
    }
    
    return last;
}