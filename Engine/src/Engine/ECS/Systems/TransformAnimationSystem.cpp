#include "epch.h"
#include "TransformAnimationSystem.h"

#include "Engine/ECS/Entity.h"
#include "Engine/Assets/AssetRegistry.h"

namespace Engine
{
	Animation::Animation(const std::vector<Segment>& segments)
		: m_Segments(segments)
	{
		m_Duration = 0.0f;
		for (const Segment& segment : m_Segments)
			m_Duration += segment.Duration;
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
		Animation& animation = AssetRegistry::Get<Animation>(animator.CurrentAnimationID);

		// Advance animation time
		animator.AnimationTime += deltaTime;
		if (animator.AnimationTime > animation.GetDuration())
			animator.AnimationTime = Mod(animator.AnimationTime, animation.GetDuration());

		// Update offset
		animator.Offset = animation.GetOffset(animator.AnimationTime);
	}
}
