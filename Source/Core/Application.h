#pragma once

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreWindowEventUtilities.h>
#include <OgreRenderTargetListener.h>
#include <OgrePlane.h>
#include <OgreMeshManager.h>
#include <iostream>
#include <cstdlib>
#include <ctime>


class Application : public Ogre::FrameListener, public Ogre::WindowEventListener, public Ogre::RenderTargetListener
{
public:
	Application();
	virtual ~Application();

	virtual void init();

	void buildRoom();
	void buildBall();
	void buildOgreHead();
	void setupCamera();
	void setupLight();
	void checkForCollisions();

	Ogre::Root * mRoot;
	Ogre::RenderWindow * mRenderWindow;
	Ogre::SceneManager * mSceneManager;
	Ogre::Camera * mCamera;
	Ogre::SceneNode* ballNode;

	Ogre::Entity* ballEntity;
	Ogre::Entity* groundEntity;
	Ogre::Entity* southWallEntity;
	Ogre::Entity* northWallEntity;
	Ogre::Entity* eastWallEntity;
	Ogre::Entity* westWallEntity;
	Ogre::Entity* ceilingEntity;

	const Ogre::Real framePerSecond = 300;
	bool mRunning = true;

	// Ogre::FrameListener
	//virtual bool frameEnded(const Ogre::FrameEvent &evt) override;
	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt) override;
	//virtual bool frameStarted(const Ogre::FrameEvent &evt) override;

	// Ogre::WindowEventListener
	//virtual void windowClosed(Ogre::RenderWindow *rw) override;
	//virtual bool windowClosing(Ogre::RenderWindow *rw) override;
	//virtual void windowFocusChange(Ogre::RenderWindow *rw) override;
	//virtual void windowMoved(Ogre::RenderWindow *rw) override;
	//virtual void windowResized(Ogre::RenderWindow *rw) override;
};
