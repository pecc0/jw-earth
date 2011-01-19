/* AUTORIGHTS
Copyright (C) 2010,2011 Petko Petkov (petkodp@gmail.com
      
This file is part of JWorld.

JWorld is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/
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
#include "SKeyMap.h"
#include <map>

namespace irr
{
namespace scene
{

enum
{
	ANIMATION_MOVE_EVENT = 300
};

class JWSceneNodeAnimatorCameraFPS: public ISceneNodeAnimatorCameraFPS
{
private:
	ISceneNodeAnimatorCameraFPS* m_Wrapped;
	IEventReceiver* m_AnimationEventsReceiver;
	typedef std::map<EKEY_CODE, s32> TKeyMap;
	TKeyMap KeyMap;
	bool CursorKeys[EKA_COUNT];
	s32 m_LastAnimationTime;
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

	IEventReceiver *getAnimationEventsReceiver() const;

	void setAnimationEventsReceiver(IEventReceiver *m_animationEventsReceiver);

};
}
}
#endif /* JWSCENENODEANIMATORCAMERAFPS_H_ */
