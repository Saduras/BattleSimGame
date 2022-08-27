#include "epch.h"
#include "TransformAnimationSystem.h"

#include "Engine/ECS/Entity.h"
#include "Engine/Assets/AssetRegistry.h"

namespace Engine
{
	Animation::Animation(PlayMode play_mode, const std::vector<Segment>& segments)
		: m_PlayMode(play_mode), m_Segments(segments)
	{
		m_Duration = 0.0f;
		for (const Segment& segment : m_Segments)
			m_Duration += segment.Duration;
	}

	bool Animation::IsDone(float time) const
	{
		switch (m_PlayMode)
		{
		case PlayMode::Once:
			return time >= GetDuration();
		case PlayMode::Loop:
			return false;
		default:
			ENG_CORE_ERROR("Unsupported play mode {}!", m_PlayMode);
			return true;
		}
	}

	Transform Animation::GetOffset(float time) const
	{
		ENG_CORE_ASSERT_FMT(time >= 0.0f && time <= m_Duration, "Invalid time {} must be in [0, {}]", time, m_Duration);

		for (const Segment& segment : m_Segments)
		{
			if (time <= segment.Duration)
			{
				float t = time / segment.Duration;
				return {
					Lerp(segment.StartTransform.Position, segment.EndTransform.Position, t),
					Lerp(segment.StartTransform.Rotation, segment.EndTransform.Rotation, t),
					Lerp(segment.StartTransform.Scale, segment.EndTransform.Scale, t),
				};
			}

			time -= segment.Duration;
		}

		return Transform();
	}
}

namespace Engine::Systems
{
	void TransformAnimationSystem(float deltaTime, Entity entity, Components::Animator& animator)
	{
		if (!animator.CurrentAnimationUUID.IsValid())
			return;

		Animation& animation = AssetRegistry::Get<Animation>(animator.CurrentAnimationUUID);

		if (animation.IsDone(animator.AnimationTime))
			return;

		// Advance animation time
		animator.AnimationTime += deltaTime;
		if (animator.AnimationTime > animation.GetDuration())
		{
			switch (animation.GetPlayMode())
			{
			case Animation::PlayMode::Once:
				animator.AnimationTime = animation.GetDuration();
				break;
			case Animation::PlayMode::Loop:
				animator.AnimationTime = Mod(animator.AnimationTime, animation.GetDuration());
				break;
			default:
				break;
			}
		}

		// Update offset
		animator.Offset = animation.GetOffset(animator.AnimationTime);
	}
}
