#include "Application.h"
#include "CoreConfig.h"

using namespace Ogre;

Application::Application()
{
}

Application::~Application()
{
}

void Application::init()
{
	NameValuePairList params;
	// Initialization
	mRoot = new Root("");
	// Load Resources

	// load plugins
#if (_DEBUG && !__APPLE__)
	mRoot->loadPlugin("RenderSystem_GL_d");
#else
	mRoot->loadPlugin("RenderSystem_GL");
#endif

	

	const RenderSystemList &renderers = mRoot->getAvailableRenderers();
	RenderSystem * renderSystem = nullptr;
	LogManager::getSingletonPtr()->logMessage("Getting available renderers");
	for (auto renderer = renderers.begin(); renderer != renderers.end(); renderer++)
	{
		String name = (*renderer)->getName();
		LogManager::getSingletonPtr()->logMessage(name);
		renderSystem = *renderer;
	}
	if (renderSystem)
	{
		LogManager::getSingletonPtr()->logMessage("Using renderer " + renderSystem->getName());
		mRoot->setRenderSystem(renderSystem);
	}
	else
	{
		LogManager::getSingletonPtr()->logMessage(LML_CRITICAL, "Initializing render system failed. No renderers available.");
	}

	params["macAPI"] = "cocoa";

	// Initialize with render system, no new window (yet)
	mRoot->initialise(false);

	// Create scene manager, render window, and camera
	mSceneManager = mRoot->createSceneManager(ST_GENERIC);
	mRenderWindow = mRoot->createRenderWindow(PROJECT_NAME, 640, 480, false, &params);

	mRoot->addFrameListener(this);
	WindowEventUtilities::addWindowEventListener(mRenderWindow, this);
	mRenderWindow->addListener(this);

	

	try {
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("/lusr/opt/ogre-1.9/share/OGRE/Media", "FileSystem");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("/lusr/opt/ogre-1.9/share/OGRE/Media/fonts", "FileSystem");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("/lusr/opt/ogre-1.9/share/OGRE/Media/materials/scripts", "FileSystem");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("/lusr/opt/ogre-1.9/share/OGRE/Media/materials/textures", "FileSystem");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("/lusr/opt/ogre-1.9/share/OGRE/Media/materials/programs/GLSL", "FileSystem");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("/lusr/opt/ogre-1.9/share/OGRE/Media/models", "FileSystem");
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();	
	} catch (Exception e) {
		std::cout << "Error found" << std::endl;
		std::cout << e.what() << std::endl;
	}

	// Personal Code
	try {
		setupCamera();
		setupLight();
		buildRoom();
		buildBall();

		// Set Random seed
		std::srand(std::time(0));
	} catch (Exception e) {
		std::cout << "Found Errore" << std::endl;
		std::cout << e.what() << std::endl;
	}
	//
}

void Application::buildRoom() {
	// Build Plane and planeMesh
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane(
		"wall",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		length, length, 20, 20,
		true,
		1, 5, 5,
		Ogre::Vector3::UNIT_Z);

	// Create Wall entities
	groundEntity = mSceneManager->createEntity("wall");
	northWallEntity = mSceneManager->createEntity("wall");
	southWallEntity = mSceneManager->createEntity("wall");
	eastWallEntity = mSceneManager->createEntity("wall");
	westWallEntity = mSceneManager->createEntity("wall");
	ceilingEntity = mSceneManager->createEntity("wall");

	// Create wall sceneNodes
	Ogre::SceneNode* groundNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	Ogre::SceneNode* northWallNode = mSceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, (length / 2), (length / 2)));
	Ogre::SceneNode* southWallNode = mSceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, (length / 2), -(length / 2)));
	Ogre::SceneNode* eastWallNode = mSceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3((length / 2), (length / 2), 0));
	Ogre::SceneNode* westWallNode = mSceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-(length / 2), (length / 2), 0));
	Ogre::SceneNode* ceilingNode = mSceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, (length), 0));

	// Orient Walls
	northWallNode->pitch(Ogre::Degree(-90));
	southWallNode->pitch(Ogre::Degree(90));
	eastWallNode->roll(Ogre::Degree(90));
	westWallNode->roll(Ogre::Degree(-90));
	ceilingNode->pitch(Ogre::Degree(180));

	// Attach WallsQuadro 600
	groundNode->attachObject(groundEntity);
	northWallNode->attachObject(northWallEntity);
	southWallNode->attachObject(southWallEntity);
	eastWallNode->attachObject(eastWallEntity);
	westWallNode->attachObject(westWallEntity);
	ceilingNode->attachObject(ceilingEntity);

	// Set Materials
	groundEntity->setMaterialName("Examples/Rockwall");
	northWallEntity->setMaterialName("Examples/Rockwall");
	southWallEntity->setMaterialName("Examples/Rockwall");
	eastWallEntity->setMaterialName("Examples/Rockwall");
	westWallEntity->setMaterialName("Examples/Rockwall");
	ceilingEntity->setMaterialName("Examples/Rockwall");

	// groundNode->showBoundingBox(true);
	// northWallNode->showBoundingBox(true);
	// southWallNode->showBoundingBox(true);
	// eastWallNode->showBoundingBox(true);
	// westWallNode->showBoundingBox(true);
	// ceilingNode->showBoundingBox(true);
}

void Application::buildBall() {
	// Create Ball Entity and Create sceneNode for the ball
	ballEntity = mSceneManager->createEntity("sphere.mesh");
	ballNode = mSceneManager->getRootSceneNode()->createChildSceneNode();

	// Scale the ball and set the position of the ball
	ballNode->scale(.5, .5, .5);
	ballNode->setPosition(Ogre::Vector3(0, 175, 0));
	
	// Attach the ball to the node and give it a material
	ballNode->attachObject(ballEntity);
	// ballNode->showBoundingBox(true);
}

void Application::buildOgreHead() {
	Ogre::Entity* ballEntity = mSceneManager->createEntity("ogrehead.mesh");
	ballNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	ballNode->attachObject(ballEntity);
}

void Application::setupCamera() {
	// Create Camera
	mCamera = mSceneManager->createCamera("Main Camera");

	// Add viewport
	Viewport * vp = mRenderWindow->addViewport(mCamera);
	mCamera->setAutoAspectRatio(true);

	// Set Position
	mCamera->setPosition(Ogre::Vector3(0, (length / 2), 0));
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
}

void Application::setupLight() {
	//mSceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Light* light = mSceneManager->createLight("MainLight");
	light->setType(Ogre::Light::LT_POINT);
	light->setDiffuseColour(.6,.6,.6);
	light->setPosition(20, 80, 50);
}

void Application::checkForCollisions() {
	static int dx = (rand() % 5) + 1;
	static int dy = (rand() % 5) + 1;
	static int dz = (rand() % 5) + 1;

	static Ogre::Vector3 move = Ogre::Vector3(dx, dy, dz);

	AxisAlignedBox ballBox = ballEntity->getWorldBoundingBox();
	
	if (ballBox.intersects(eastWallEntity->getWorldBoundingBox())
			|| ballBox.intersects(westWallEntity->getWorldBoundingBox()))
		dx = -(dx);

	
	if (ballBox.intersects(southWallEntity->getWorldBoundingBox())
			|| ballBox.intersects(northWallEntity->getWorldBoundingBox()))
		dz = -(dz);

	if (ballBox.intersects(ceilingEntity->getWorldBoundingBox())
			|| ballBox.intersects(groundEntity->getWorldBoundingBox()))
		dy = -(dy);

	move = Ogre::Vector3(dx, dy, dz);
	mCamera->lookAt(ballNode->getPosition());
	ballNode->translate(move);

}

bool Application::frameRenderingQueued(const FrameEvent &evt)
{
	static Ogre::Real timer = 0.0;

	if (mRenderWindow->isClosed())
	{
		return false;
	}

	if (!mRunning)
	{
		return false;
	}

	timer -= evt.timeSinceLastFrame;

	if (timer < 0.0) {
		checkForCollisions();
		timer = 1.0 / ballSpeed;
	}
	
	return true;
}
