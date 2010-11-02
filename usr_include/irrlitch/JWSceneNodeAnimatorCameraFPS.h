/*
 * JWSceneNodeAnimatorCameraFPS.h
 *
 *  Created on: Nov 2, 2010
 *      Author: Petko
 */

#ifndef JWSCENENODEANIMATORCAMERAFPS_H_
#define JWSCENENODEANIMATORCAMERAFPS_H_

#include "ISceneNodeAnimatorCameraFPS.h"
#include "ICameraSceneNode.h"

namespace irr
{
namespace scene
{

class JWSceneNodeAnimatorCameraFPS: public ISceneNodeAnimatorCameraFPS
{
private:
	ISceneNodeAnimatorCameraFPS* m_Wrapped;
public:
	JWSceneNodeAnimatorCameraFPS(ISceneNodeAnimatorCameraFPS* wrapped);
	virtual ~JWSceneNodeAnimatorCameraFPS();

	/**
	 * Takes a ISceneNodeAnimatorCameraFPS* from the camera animators (first one in the list)
	 * and wraps it with an instance of this class.
	 * Removes all camera animators and adds the new animator
	 */
	static JWSceneNodeAnimatorCameraFPS* injectOnFPSCamera(
			ICameraSceneNode* fpsCamera);

	//! Animates the scene node, currently only works on cameras
	virtual void animateNode(ISceneNode* node, u32 timeMs);

	//! Event receiver
	virtual bool OnEvent(const SEvent& event);

	//! Returns the speed of movement in units per second
	virtual f32 getMoveSpeed() const;

	//! Sets the speed of movement in units per second
	virtual void setMoveSpeed(f32 moveSpeed);

	//! Returns the rotation speed
	virtual f32 getRotateSpeed() const;

	//! Set the rotation speed
	virtual void setRotateSpeed(f32 rotateSpeed);

	//! Sets the keyboard mapping for this animator
	//! \param keymap: an array of keyboard mappings, see SKeyMap
	//! \param count: the size of the keyboard map array
	virtual void setKeyMap(SKeyMap *map, u32 count);

	//! Sets whether vertical movement should be allowed.
	virtual void setVerticalMovement(bool allow);

	//! Creates a clone of this animator.
	/** Please note that you will have to drop
	 (IReferenceCounted::drop()) the returned pointer after calling
	 this. */
	virtual ISceneNodeAnimator* createClone(ISceneNode* node,
			ISceneManager* newManager = 0);

	virtual void setInvertMouse(bool invert);

	virtual bool hasFinished(void);

	virtual ESCENE_NODE_ANIMATOR_TYPE getType() const;

	virtual bool isEventReceiverEnabled() const
	{
		return true;
	}
};
}
}
#endif /* JWSCENENODEANIMATORCAMERAFPS_H_ */
