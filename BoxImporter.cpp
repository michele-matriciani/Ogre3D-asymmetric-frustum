#include "BoxImporter.h"
#include "DotSceneLoader.h"
#include "face.h"
//-------------------------------------------------------------------------------------
BoxImporter::BoxImporter(void)
{
}
 
//-------------------------------------------------------------------------------------
BoxImporter::~BoxImporter(void)
{
}
 
//-------------------------------------------------------------------------------------

Ogre::Matrix4 calcViewMatrix(Ogre::Vector3, Ogre::Vector3);
Ogre::Matrix4 calcFrustum(Ogre::Real,Ogre::Real,Ogre::Real,
                          Ogre::Real,Ogre::Real,Ogre::Real );
Ogre::Vector3 centerScene(Ogre::Real,Ogre::Real,
                          Ogre::Real,Ogre::Real,Ogre::Real );

void BoxImporter::createCamera(void)
{
	// create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // set its position, direction  
    mCamera->setPosition(Ogre::Vector3(0,0,1));
    mCamera->lookAt(Ogre::Vector3(0,0,-2));

    // set the near clip distance
    mCamera->setNearClipDistance(1);

    // create a default camera controller
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}
 
//-------------------------------------------------------------------------------------
void BoxImporter::createViewports(void)
{
	// Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);

    // set Background color
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));


    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) 
    		/ Ogre::Real(vp->getActualHeight()));

    xScreen = vp->getActualWidth();
    yScreen = vp->getActualHeight();
}
 
//-------------------------------------------------------------------------------------
void BoxImporter::createScene(void)
{
    
    setup.readSetup();

    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0,0,0));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);//SHADOWTYPE_STENCIL_ADDITIVE);
    
    Ogre::String sceneName = setup.getSceneName();

    
    Ogre::DotSceneLoader loader;
    std::cerr << "BEGIN PARSING XML" << std::endl;

    sceneNode = mSceneMgr->getRootSceneNode()->
        createChildSceneNode("SceneNode");

    if (!loader.parseDotScene(sceneName,"General", mSceneMgr, sceneNode)) {
        setup.loadDefaultScene();
        std::cout << "Loading default scene" << std::endl;
        
        if (!loader.parseDotScene(setup.getSceneName(),"General", mSceneMgr, sceneNode)) {
            
            throw Ogre::Exception(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "Error loading default scene","BoxImporter::getValue");//,"Error loading default scene");
    
        }
    }
        
    
    std::cerr << "END PARSING XML" << std::endl;
   
 

    

    width =  ( setup.getR() - setup.getL() )/2;
    height = ( setup.getT() - setup.getB() )/2;
    beginScene = setup.getN();

    nP = setup.getNP();
    sceneLength = nP + setup.getF() + 20;

    Ogre::Vector3 translVec = centerScene(setup.getL(),setup.getR(),
        setup.getT(),setup.getB(),beginScene-nP);

    sceneNode->translate(translVec ,Ogre::Node::TS_LOCAL);

      if (setup.getLight()) {
        // Add new Light
        pointLight = mSceneMgr->createLight("pointLight");
        pointLight->setType(Ogre::Light::LT_POINT);
        pointLight->setPosition(Ogre::Vector3(0, 0, -nP+5));

        //Set light color
        pointLight->setDiffuseColour(setup.getDifCol());
        pointLight->setSpecularColour(setup.getSpecCol());
   }

    xCam = setup.getXCam();
    yCam = setup.getYCam();

    
    Ogre::SceneNode *camNode = mSceneMgr->getRootSceneNode()->
            createChildSceneNode("Cam");
    camNode->attachObject(mCamera);
}

void BoxImporter::createFrameListener(void)
{   

    BaseApplication::createFrameListener();

    

    ar      = yScreen/xScreen; 
    xr      = xScreen/xCam;
    yr      = yScreen/yCam;


    // Populate the camera container
    mCamNode = mCamera->getParentSceneNode();
    
    
    mMove = 0.08;
    nMove = 0.2;
    lMove = 0.3;
    
    // init vectors
    face = Ogre::Vector3(0,0,0);
    eye = face;
    target = Ogre::Vector3(0, 0, -1);
    mDirection = Ogre::Vector3::ZERO;

    xCam /= 2;
    yCam = yCam/2 -80;

    prevX = xCam;
    prevY = yCam;
    prevZ = nP;

    flX = new FluidFilter(4,prevX);
    flY = new FluidFilter(4,prevY);
    flZ = new FluidFilter(4,prevZ);


    init(); // init Opencv module

}


bool BoxImporter::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if (mWindow->isClosed()) return false;
    if (mShutDown) return false;
    mKeyboard->capture();
    mMouse->capture();
    mTrayMgr->frameRenderingQueued(evt);

    int coordX,coordY,coordZ;
    
    Ogre::Real x,y,z;
    getFaceCoord( &coordX, &coordY, &coordZ );
    
    if (coordX != 0 && coordY != 0 && coordZ != 0) {
        Ogre::Vector2 fX = Ogre::Vector2( coordX, prevX );
        Ogre::Vector2 fY = Ogre::Vector2( coordY, prevY );
        Ogre::Vector2 fZ = Ogre::Vector2( coordZ, prevZ );

        filter.filter(fX,7,1);
        filter.filter(fY,6,1);
        filter.filter(fZ,10,5);
        


        setX(fX,coordX);
        setY(fY,coordY);
        setZ(fZ,coordZ);
        
        

        x = flX->interpolate(coordX);
        y = flY->interpolate(coordY);
        z = flZ->interpolate(coordZ);

    }
    else {
        
        x = flX->interpolate(prevX);
        y = flY->interpolate(prevY);
        z = flZ->interpolate(prevZ);
    }
    
    // Ogre::Real provaX = flX->getLast(); 
    // Ogre::Real provaY = flY->getLast();
    // Ogre::Real provaZ = flZ->getLast();

    // Ogre::Real z = (Ogre::Real)provaZ / 200 ;
    
    // // con -320.0f non e' allineato
    // Ogre::Real x = -((( (Ogre::Real)(provaX) - xCam ) / 15.0f  ) /(xr*ar));

    // Ogre::Real y = -((( (Ogre::Real)(provaY) - yCam ) / 15.0f  ) /(yr*ar));
    
    std::cout << xr << " , " << xScreen << " , " <<
         xCam << " , " << std::endl;
    
    z = z / 150.0f ;
    
    // // con -320.0f non e' allineato
    x = -( ( ( x - xCam ) / 15.0f ) / (xr*ar) );
    y = -( ( ( y - yCam ) / 15.0f ) / (yr*ar) );  

    face.x = x;
    face.y = y;
    face.z = z;
    


    Ogre::Real n = -face.z + nP;
    Ogre::Real f = n + sceneLength;

    r = width - face.x;
    l = r - 2*width; 
    t = height - face.y;
    b = t - 2*height;


    eye = Ogre::Vector3(-face.x, -face.y, -face.z);

    
    target = Ogre::Vector3( -face.x, -face.y, -n);//r - width, t - height, -n);

     
    Ogre::Vector3 translVec = Ogre::Vector3(
            -face.x*2,// + r - width,
            -face.y*2,// + t - height,
            0
    );
  
    Ogre::Matrix4 translMat = Ogre::Matrix4::IDENTITY;
    translMat.makeTrans(translVec);

    Ogre::Matrix4 view = calcViewMatrix(eye,target);
    Ogre::Matrix4 frustum = calcFrustum(l,r,t,b,n,f);


    mCamera->setCustomViewMatrix(true, translMat*view);
    mCamera->setCustomProjectionMatrix(true,frustum);
   
    
    return true;
}

bool BoxImporter::mouseMoved( const OIS::MouseEvent& evt )
{

    return true;
}

bool BoxImporter::mousePressed( const OIS::MouseEvent& evt, 
            OIS::MouseButtonID id )
{
    
    switch (id)
    {
    case OIS::MB_Left:
     
        break;
    default:
        break;
    }

    return true;
}

bool BoxImporter::mouseReleased( const OIS::MouseEvent& evt,
     OIS::MouseButtonID id ){return true;}


bool BoxImporter::keyPressed( const OIS::KeyEvent& evt )
{
    switch (evt.key)
    {   
        // exit
        case OIS::KC_ESCAPE: 
            finalize(); //close OpenCv resources
            mShutDown = true;
            break;
        default:
            break;

        // forward
        case OIS::KC_UP:
            if ( pointLight != NULL )
                pointLight->setPosition(pointLight->getPosition() + 
                        Ogre::Vector3(0,lMove,0));
            break;
        case OIS::KC_W:
            width += mMove;
            height = width*ar;
            break;
        // backward
        case OIS::KC_DOWN:

            if ( pointLight != NULL )
                pointLight->setPosition(pointLight->getPosition() + 
                        Ogre::Vector3(0,-lMove,0));
            break;
        case OIS::KC_S:
            width -= mMove;
            height = width*ar;
            break;
        // left
        case OIS::KC_LEFT:

            if ( pointLight != NULL )
                pointLight->setPosition(pointLight->getPosition() + 
                          Ogre::Vector3(-lMove,0,0));
            break;
        // case OIS::KC_A:
        //     eye.x -= mMove;
        //     break;
        // right
        case OIS::KC_RIGHT:
            if ( pointLight != NULL )
                pointLight->setPosition(pointLight->getPosition() + 
                        Ogre::Vector3(lMove,0,0));
            break;
        // case OIS::KC_D:
        //     eye.x += mMove;
        //     break;
        //down
        case OIS::KC_PGDOWN:
            if ( pointLight != NULL)
                pointLight->setPosition(pointLight->getPosition() + 
                           Ogre::Vector3(0,0,lMove));
            break;
        case OIS::KC_E:
            nP -= nMove;
            sceneNode->translate(0,0,nMove ,Ogre::Node::TS_LOCAL);
            break;
        // up
        case OIS::KC_PGUP:
            if ( pointLight != NULL)
                pointLight->setPosition(pointLight->getPosition() + 
                            Ogre::Vector3(0,0,-lMove));
            break;
        case OIS::KC_Q:
            nP += nMove;
            sceneNode->translate(0,0,-nMove ,Ogre::Node::TS_LOCAL);
            break;

    }   
    return true;
}

bool BoxImporter::keyReleased( const OIS::KeyEvent& evt )
{
    //Reset directions
    switch (evt.key)
        {
            
        default:
            break;
    }

    return true;
}

Ogre::Matrix4 calcViewMatrix(Ogre::Vector3 eye, Ogre::Vector3 target){
    
    Ogre::Vector3 up = Ogre::Vector3(0, 1, 0);
    Ogre::Vector3 z = (eye - target);    // The "forward" vector.
    z.normalise();
    Ogre::Vector3 x = (up.crossProduct(z));
    x.normalise(); // The "right" vector.
    Ogre::Vector3 y = z.crossProduct(x);     // The "up" vector.
 
    // Create a 4x4 view matrix from the right, up, forward and eye position vectors
    Ogre::Matrix4 viewMatrix = Ogre::Matrix4(
        x.x, x.y, x.z, -(x.dotProduct(eye)),
        y.x, y.y, y.z, -(y.dotProduct(eye)),
        z.x, z.y, z.z, -(z.dotProduct(eye)),
          0,   0,   0,                    1
    );
    
    return viewMatrix;
}

Ogre::Matrix4 calcFrustum(Ogre::Real l,Ogre::Real r,
                          Ogre::Real t,Ogre::Real b,
                          Ogre::Real n,Ogre::Real f ){

    Ogre::Matrix4 frustum = Ogre::Matrix4(
        2*n/(r-l),         0, (l+r)/(r-l),           0,
                0, 2*n/(t-b), (t+b)/(t-b),           0,
                0,         0,     f/(n-f), (n*f)/(n-f),
                0,         0,          -1,           0
    );

    return frustum;
}

void BoxImporter::setX(Ogre::Vector2 & f, int &coordX) {
    coordX  = f.x;
    prevX   = f.y;
}

void BoxImporter::setY(Ogre::Vector2 & f, int &coordY) {
    coordY  = f.x;
    prevY   = f.y;
}
void BoxImporter::setZ(Ogre::Vector2 & f, int &coordZ) {
    coordZ  = f.x;
    prevZ   = f.y;
}

Ogre::Vector3 centerScene(Ogre::Real l, Ogre::Real r,
                             Ogre::Real t, Ogre::Real b,
                             Ogre::Real n) {

    Ogre::Real midX = (r+l)/2;
    Ogre::Real midY = (t+b)/2;

    return  Ogre::Vector3(-midX, -midY, n);
}



// MAIN
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        BoxImporter app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
