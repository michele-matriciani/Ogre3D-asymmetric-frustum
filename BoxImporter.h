#ifndef __BoxImporter_h_
#define __BoxImporter_h_

#include "BaseApplication.h"
#include "Filter.h"
#include "FluidFilter.h"

class BoxImporter : public BaseApplication
	{
public:
	BoxImporter(void);
	virtual ~BoxImporter(void);
 
protected:
	virtual void createScene(void);
	virtual void createCamera(void);
	virtual void createViewports(void);

	virtual void createFrameListener(void);
	// Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt );

    //OIS::MouseListener
	virtual bool mouseMoved( const OIS::MouseEvent& evt );
	virtual bool mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent& evt, 
    		OIS::MouseButtonID id );

    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent& evt );
    virtual bool keyReleased( const OIS::KeyEvent& evt );

    virtual void setX( Ogre::Vector2 & f, int & coordX ) ;
    virtual void setY( Ogre::Vector2 & f, int & coordX  );
    virtual void setZ( Ogre::Vector2 & f, int & coordX  );


	Ogre::Real mRotate;          // The rotate constant
    Ogre::Real mMove;            // The movement constant
    Ogre::SceneNode *mCamNode;   // The SceneNode the camera is currently attached to
    Ogre::Vector3 mDirection;    // Value to move in the correct direction
    Ogre::SceneNode *sceneNode;   // The SceneNode the camera is currently attached to
    Ogre::Light* pointLight;

    Ogre::Vector3 eye,positionV, target;
    Ogre::Real r,l,t,b,width,height;

    Ogre::Vector3 pTransl;

    int prevX, prevY, prevZ;

    Ogre::Real xCam, yCam;  //CAM resolution
    Ogre::Real xScreen, yScreen;  //SCENE resolution
    Ogre::Real ar,xr,yr; //Aspect Ratio

    Ogre::Real nP,zScene; // near plane and z distance of scene from camera 
    Filter filter;
    FluidFilter *flX, *flY, *flZ;
};


#endif // #ifndef __BoxImporter_h_