/*
 * JWSceneNodeAnimatorCameraFPS.cpp
 *
 *  Created on: Nov 2, 2010
 *      Author: Petko
 */

#include "irrlitch/JWSceneNodeAnimatorCameraFPS.h"
#include "string.h"

namespace irr
{
namespace scene
{

JWSceneNodeAnimatorCameraFPS::JWSceneNodeAnimatorCameraFPS(
		ISceneNodeAnimatorCameraFPS* wrapped) :
	m_Wrapped(wrapped), m_animationEventsReceiver(0)
{
	memset(CursorKeys, 0, EKA_COUNT);
	m_Wrapped->grab();
}

JWSceneNodeAnimatorCameraFPS::~JWSceneNodeAnimatorCameraFPS()
{
	m_Wrapped->drop();
}

f32 JWSceneNodeAnimatorCameraFPS::getRotateSpeed() const
{
	return m_Wrapped->getRotateSpeed();
}

void JWSceneNodeAnimatorCameraFPS::setKeyMap(SKeyMap *map, u32 count)
{
	m_Wrapped->setKeyMap(map, count);

	for (u32 i = 0; i < count; ++i)
	{
		KeyMap[map[i].KeyCode] = map[i].Action;
	}
}

void JWSceneNodeAnimatorCameraFPS::setRotateSpeed(f32 rotateSpeed)
{
	m_Wrapped->setRotateSpeed(rotateSpeed);
}

void JWSceneNodeAnimatorCameraFPS::setMoveSpeed(f32 moveSpeed)
{
	m_Wrapped->setMoveSpeed(moveSpeed);
}

f32 JWSceneNodeAnimatorCameraFPS::getMoveSpeed() const
{
	return m_Wrapped->getMoveSpeed();
}

bool JWSceneNodeAnimatorCameraFPS::OnEvent(const SEvent & event)
{
	switch (event.EventType)
	{
	case EET_KEY_INPUT_EVENT:
	{
		TKeyMap::iterator it = KeyMap.find(event.KeyInput.Key);

		if (KeyMap.end() != it)
		{
			CursorKeys[it->second] = event.KeyInput.PressedDown;
		}
		break;
	}
	default:
		break;
	}
	return m_Wrapped->OnEvent(event);
}

void JWSceneNodeAnimatorCameraFPS::animateNode(ISceneNode *node, u32 timeMs)
{
	core::vector3df startPos = node->getPosition();

	m_Wrapped->animateNode(node, timeMs);
	if (CursorKeys[EKA_JUMP_UP])
	{
		core::vector3df pos = node->getPosition();
		pos.Y += getMoveSpeed();
		node->setPosition(pos);
	}
	if (CursorKeys[EKA_CROUCH])
	{
		core::vector3df pos = node->getPosition();
		pos.Y -= getMoveSpeed();
		node->setPosition(pos);
	}
	if (m_animationEventsReceiver && startPos != node->getPosition())
	{
		SEvent event;
		event.EventType = EET_USER_EVENT;
		event.UserEvent.UserData1 = ANIMATION_MOVE_EVENT;
		event.UserEvent.UserData2 = (irr::s32) node;
		//event.even
		m_animationEventsReceiver->OnEvent(event);
	}
}

void JWSceneNodeAnimatorCameraFPS::setVerticalMovement(bool allow)
{
}

ISceneNodeAnimator *JWSceneNodeAnimatorCameraFPS::createClone(ISceneNode *node,
		ISceneManager *newManager)
{

	return new JWSceneNodeAnimatorCameraFPS(
			(ISceneNodeAnimatorCameraFPS*) ((m_Wrapped->createClone(node,
					newManager))));
}

void JWSceneNodeAnimatorCameraFPS::setInvertMouse(bool invert)
{
	m_Wrapped->setInvertMouse(invert);
}

JWSceneNodeAnimatorCameraFPS *JWSceneNodeAnimatorCameraFPS::injectOnFPSCamera(
		ICameraSceneNode *fpsCamera)
{
	JWSceneNodeAnimatorCameraFPS
			*result =
					new JWSceneNodeAnimatorCameraFPS(
							(ISceneNodeAnimatorCameraFPS*) (((((*(fpsCamera->getAnimators().begin())))))));
	fpsCamera->removeAnimators();
	fpsCamera->addAnimator(result);
	result->drop();
	return result;
}

bool JWSceneNodeAnimatorCameraFPS::hasFinished(void)
{
	return m_Wrapped->hasFinished();
}

ESCENE_NODE_ANIMATOR_TYPE JWSceneNodeAnimatorCameraFPS::getType() const
{
	return m_Wrapped->getType();
}

IEventReceiver *JWSceneNodeAnimatorCameraFPS::getAnimationEventsReceiver() const
{
	return m_animationEventsReceiver;
}

void JWSceneNodeAnimatorCameraFPS::setAnimationEventsReceiver(
		IEventReceiver *m_animationEventsReceiver)
{
	this->m_animationEventsReceiver = m_animationEventsReceiver;
}

}
}
