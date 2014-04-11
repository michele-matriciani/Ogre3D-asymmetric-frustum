#include "FluidFilter.h"

#include <vector>
#include <iostream>



FluidFilter::FluidFilter(int step,int max,int begin):
    n(step),
    m(max)

{ init(begin);
}
/*
bool FluidFilter::interpolate(int c) {
    
    std::cout << q.back() << " , " << c << std::endl;
    q.push(c);
    return true;
}
*/

bool FluidFilter::interpolate(int c) {
    

    Ogre::Real last = q.back();
    std::cout << last << " , " << c << std::endl;
    

    if ( last == c ) {
        if (q.size() < 2)
        q.push(last);
    }

    // else if ( abs( c - (int)last ) > m ) {
        
    //     q.push(c);
    // }

    else {
        int num = n;
        
        int size = q.size();
        if( size > 5)
            num = n/3;
        else if ( size > 3)
            num = n/2;
        
        Ogre::Real gap = ( (Ogre::Real)(c) - last ) / num;
        
        for (int i = 1; i < num; i++) {
            q.push(q.back() + gap );
            //q.push( last + (i)*gap );    
        }   
        q.push(c);

    }
    return true;

}

Ogre::Real FluidFilter::getLast() {
    Ogre::Real ret = q.front();
    q.pop();
    std::cout << "SIZE = " << q.size() << std::endl;
    return ret;
}

void FluidFilter::init(int a) {
    q.push(a);
}