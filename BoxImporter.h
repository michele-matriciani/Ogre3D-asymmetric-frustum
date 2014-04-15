#ifndef __BoxImporter_h_
#define __BoxImporter_h_

#include "BaseApplication.h"
#include "Filter.h"
#include "FluidFilter.h"
#include "Setup.h"

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


    Ogre::Real mMove;            // The movement constant
    Ogre::Real lMove;            // The light movement constant
    Ogre::Real nMove;            // The near plane movement constant
    
    Ogre::SceneNode *mCamNode;   // The SceneNode the camera is currently attached to
    Ogre::Vector3 mDirection;    // Value to move in the correct direction
    Ogre::SceneNode *sceneNode = NULL;   // The SceneNode the camera is currently attached to
    Ogre::Light* pointLight = NULL;

    Ogre::Vector3 face, eye, target;
    Ogre::Real r, l, t, b, width, height;

    Ogre::Vector3 pTransl;

    int prevX, prevY, prevZ;

    Ogre::Real xCam, yCam;  //CAM resolution
    Ogre::Real xScreen, yScreen;  //SCENE resolution
    Ogre::Real ar, xr, yr; //Aspect Ratio

    Ogre::Real nP, beginScene, sceneLength; // near plane and z distance of scene from camera 
    Filter filter;
    FluidFilter *flX, *flY, *flZ;
    Setup setup;
};


#endif // #ifndef __BoxImporter_h_