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
    /*
    Ogre::PSSMShadowCameraSetup* pssmSetup = new Ogre::PSSMShadowCameraSetup();
    pssmSetup->calculateSplitPoints(4, mCamera->getNearClipDistance(), 100);
    Ogre::ShadowCameraSetupPtr shadowCamera(pssmSetup);
    */
    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0,0,0));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);//SHADOWTYPE_STENCIL_ADDITIVE);
   /* mSceneMgr->setShadowCasterRenderBackFaces(true);
    //mSceneMgr->setShadowTextureCasterMaterial("Ogre/DepthShadowmap/Caster/Float");//"PSSM/shadow_caster");
    //mSceneMgr->setShadowTextureReceiverMaterial("Ogre/DepthShadowmap/Receiver/Float");
    mSceneMgr->setShadowTextureReceiverMaterial(Ogre::StringUtil::BLANK);
    mSceneMgr->setShadowTextureSelfShadow(true);
    mSceneMgr->setShadowTextureSettings(4096, 3, Ogre::PF_FLOAT32_R);
    mSceneMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_POINT, 3);
    mSceneMgr->setShadowFarDistance(100.0f);
    mSceneMgr->setShadowTextureFadeStart(0.7f);
    mSceneMgr->setShadowTextureFadeEnd(0.9f);
    mSceneMgr->setShadowTextureSelfShadow(true);
    mSceneMgr->setShadowCameraSetup(shadowCamera);*/
    
    // Add entity
    /*
    Ogre::Entity* entNinja = mSceneMgr->createEntity(
        "Ninja", "ninja.mesh");
    entNinja->setCastShadows(true);
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->
    attachObject(entNinja);
*/
    Ogre::DotSceneLoader loader;
    std::cerr << "INIZIO PARSING XML" << std::endl;

    sceneNode = mSceneMgr->getRootSceneNode()->
    createChildSceneNode("SceneNode");
    loader.parseDotScene("living-room.scene","General", mSceneMgr, sceneNode);

    std::cerr << "FINE PARSING XML" << std::endl;
   
   
    /*Ogre::Entity* floor = mSceneMgr->getEntity("floor");
    floor->setMaterialName("myMaterials/Specular");
    //floor->setMaterialName("Ogre/shadow/depth/integrated/pssm");


    Ogre::Entity* tavolo = mSceneMgr->getEntity("tavolo_1");
    tavolo->setMaterialName("myMaterials/TransparentTexture");
    */
    
    //Ogre::Entity* lampadario= mSceneMgr->getEntity("Cone.024");
    
    //tavolo2->setMaterialName("myMaterials/Floor");
    
    
    pointLight = mSceneMgr->getLight("Lamp");
    Ogre::Vector3 lampPos =  pointLight->getPosition();
    std::cout << pointLight->getPosition()<< std::endl;
    mSceneMgr->destroyLight("Lamp");

    // Add new Light

    pointLight = mSceneMgr->createLight("pointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    //pointLight->setPosition(Ogre::Vector3(0, 2, -4));
    pointLight->setPosition(lampPos);
    

    
    //Set light color
    pointLight->setDiffuseColour(1.0, 1.0, 1.0);

    pointLight->setSpecularColour(1.0, 1.0, 1.0);

    
    Ogre::SceneNode *camNode = mSceneMgr->getRootSceneNode()->
            createChildSceneNode("Cam");
    camNode->attachObject(mCamera);
}

void BoxImporter::createFrameListener(void)
{
    BaseApplication::createFrameListener();

    xCam = 640;
    yCam = 480;

    ar      = yScreen/xScreen; 
    xr      = xScreen/xCam;
    yr      = yScreen/yCam;

    width = 8;
    height = width*ar;

    // Populate the camera container
    mCamNode = mCamera->getParentSceneNode();
    
    zScene = 2;
    nP = 11;
    sceneNode->translate(0,-height,-nP ,Ogre::Node::TS_LOCAL);
    pointLight->setPosition(pointLight->getPosition()+
                Ogre::Vector3(0,-height,0));
    // set the rotation and move speed
    mRotate = 0.13;
    mMove = 0.08;
 
    
    // set initial direction
    eye = Ogre::Vector3(0,0,0);
    positionV = eye;
    target = Ogre::Vector3(0, 0, -1);
    mDirection = Ogre::Vector3::ZERO;

    prevX = 320;
    prevY = 240;
    prevZ = 1;

    flX = new FluidFilter(3,10,prevX);
    flY = new FluidFilter(3,10,prevY);
    flZ = new FluidFilter(3,50,prevZ);


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
    getFaceCoord( &coordX, &coordY, &coordZ );
    
    if (coordX != 0 && coordY != 0 && coordZ != 0) {
        Ogre::Vector2 fX = Ogre::Vector2( coordX, prevX);
        Ogre::Vector2 fY = Ogre::Vector2( coordY, prevY);
        Ogre::Vector2 fZ = Ogre::Vector2( coordZ, prevZ);

        filter.filter(fX,9,12,1);
        filter.filter(fY,9,12,1);
        filter.filter(fZ,9,5,2);
        


        setX(fX,coordX);
        setY(fY,coordY);
        setZ(fZ,coordZ);
        
        flX->interpolate(coordX);
        flY->interpolate(coordY);
        flZ->interpolate(coordZ);

    }
    else {
        /*
        coordX = prevX;
        coordY = prevY;
        coordZ = prevZ;
        */

        flX->interpolate(prevX);
        flY->interpolate(prevY);
        flZ->interpolate(prevZ);    
    }
    Ogre::Real provaX = flX->getLast(); 
    Ogre::Real provaY = flY->getLast();
    Ogre::Real provaZ = flZ->getLast();

    Ogre::Real z = (Ogre::Real)provaZ / 200 ;
    
    // con -320.0f non e' allineato
    Ogre::Real x = -((( (Ogre::Real)(provaX) - 320.0f ) / 15.0f  ) /(xr*ar));

    Ogre::Real y = -((( (Ogre::Real)(provaY) - 160.0f ) / 15.0f  ) /(yr*ar));
    
    eye.x = x;
    eye.y = y;
    eye.z = z;
    
    /*std::cout << x << " , " << y << " , " <<
         z << " , " << std::endl;
    */
    

    Ogre::Real n = -eye.z + nP+ zScene;
    Ogre::Real f = n + 100;

    r = width - eye.x;
    l = r - 2*width; 
    t = height - eye.y;
    b = t - 2*height;


    positionV = Ogre::Vector3(-eye.x, -eye.y, -eye.z);

    
    target = Ogre::Vector3( r - width, t - height, -n);

     
    Ogre::Vector3 transl = Ogre::Vector3(
            -eye.x*2,// + r - width,
            -eye.y*2,// + t - height,
            0
    );
    
    Ogre::Matrix4 trans = Ogre::Matrix4::IDENTITY;
    trans.makeTrans(transl);

    Ogre::Matrix4 view = calcViewMatrix(positionV,target);
    Ogre::Matrix4 frustum = calcFrustum(l,r,t,b,n
        ,f);

    

    mCamera->setCustomViewMatrix(true, trans*view);
    mCamera->setCustomProjectionMatrix(true, frustum);
    //pointLight->setPosition(positionV);
    
    return true;
}

bool BoxImporter::mouseMoved( const OIS::MouseEvent& evt )
{
  /*  if (evt.state.buttonDown(OIS::MB_Right))
    {

        mCamNode->yaw(Ogre::Degree(-mRotate * evt.state.X.rel), 
            Ogre::Node::TS_WORLD);
        mCamNode->pitch(Ogre::Degree(-mRotate * evt.state.Y.rel), 
            Ogre::Node::TS_LOCAL);
    }*/

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
            finalize();
            mShutDown = true;
            break;
        default:
            break;

        // forward
        case OIS::KC_UP:
            pointLight->setPosition(pointLight->getPosition() + 
                        Ogre::Vector3(0,0.3f,0));
            break;
        case OIS::KC_W:
            width += mMove;
            height = width*ar;
            break;
        // backward
        case OIS::KC_DOWN:
        pointLight->setPosition(pointLight->getPosition() + 
                        Ogre::Vector3(0,-0.3f,0));
            break;
        case OIS::KC_S:
            width -= mMove;
            height = width*ar;
            break;
        // left
        case OIS::KC_LEFT:
            pointLight->setPosition(pointLight->getPosition() + 
                        Ogre::Vector3(-0.3f,0,0));
            break;
        case OIS::KC_A:
            eye.x -= mMove;
            break;
        // right
        case OIS::KC_RIGHT:
            pointLight->setPosition(pointLight->getPosition() + 
                        Ogre::Vector3(0.3f,0,0));
            break;
        case OIS::KC_D:
            eye.x += mMove;
            break;
        //down
        case OIS::KC_PGDOWN:
            pointLight->setPosition(pointLight->getPosition() + 
                           Ogre::Vector3(0,0,0.3f));
            break;
        case OIS::KC_E:
            nP -= 0.2;
            sceneNode->translate(0,0,0.2 ,Ogre::Node::TS_LOCAL);
            break;
        // up
        case OIS::KC_PGUP:
            pointLight->setPosition(pointLight->getPosition() + 
                            Ogre::Vector3(0,0,-0.3f));
            break;
        case OIS::KC_Q:
            nP += 0.2;
            sceneNode->translate(0,0,-0.2 ,Ogre::Node::TS_LOCAL);
            break;

    }   
    return true;
}

bool BoxImporter::keyReleased( const OIS::KeyEvent& evt )
{
    //Reset directions
    switch (evt.key)
        {
        case OIS::KC_UP:
        case OIS::KC_W:
            mDirection.z = 0;
            break;
         
        case OIS::KC_DOWN:
        case OIS::KC_S:
            mDirection.z = 0;
            break;
         
        case OIS::KC_LEFT:
        case OIS::KC_A:
            mDirection.x = 0;
            break;
         
        case OIS::KC_RIGHT:
        case OIS::KC_D:
            mDirection.x = 0;
            break;
         
        case OIS::KC_PGDOWN:
        case OIS::KC_E:
            mDirection.y = 0;
            break;
         
        case OIS::KC_PGUP:
        case OIS::KC_Q:
            mDirection.y = 0;
            break;
         
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
        0, 0, 0, 1
    );
    
    return viewMatrix;
}

Ogre::Matrix4 calcFrustum(Ogre::Real l,Ogre::Real r,
                          Ogre::Real t,Ogre::Real b,
                          Ogre::Real n,Ogre::Real f ){

    Ogre::Matrix4 frustum = Ogre::Matrix4(
        2*n/(r-l),         0,  (l+r)/(r-l),         0,
                0, 2*n/(t-b),  (t+b)/(t-b),         0,
                0,         0,   f/(n-f), (n*f)/(n-f),
                0,         0,           -1,         0
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
