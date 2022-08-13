#pragma once

#include "Engine/Math.h"
#include "Engine/Assets/Asset.h"


namespace Engine
{
	class Entity;

	class Animation : public Asset
	{
	public:
		struct Segment
		{
			float Duration;
			Transform StartTransform;
			Transform EndTransform;
		};

		Animation(const std::vector<Segment>& segments);

		Transform GetOffset(float time) const;
		float GetDuration() const { return m_Duration; }

	private: 
		std::vector<Segment> m_Segments;
		float m_Duration;
	};
}

namespace Engine::Components
{
	struct Animator
	{
		std::string CurrentAnimationID;
		Transform Offset = Transform();
		float AnimationTime = 0.0f;
	};
}

namespace Engine::Systems
{
	void TransformAnimationSystem(float deltaTime, Entity entity, Components::Animator& animator);
}