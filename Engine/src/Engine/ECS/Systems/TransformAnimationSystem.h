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

		enum class PlayMode
		{
			Once = 0,
			Loop
		};

		Animation(PlayMode play_mode, const std::vector<Segment>& segments);

		bool IsDone(float time) const;
		Transform GetOffset(float time) const;
		float GetDuration() const { return m_Duration; }
		PlayMode GetPlayMode() const { return m_PlayMode; }

	private: 
		std::vector<Segment> m_Segments;
		float m_Duration;
		PlayMode m_PlayMode;
	};
}

namespace Engine::Components
{
	struct Animator
	{
		UUID CurrentAnimationUUID;
		Transform Offset = Transform();
		float AnimationTime = 0.0f;
	};
}

namespace Engine::Systems
{
	void TransformAnimationSystem(float deltaTime, Entity entity, Components::Animator& animator);
}