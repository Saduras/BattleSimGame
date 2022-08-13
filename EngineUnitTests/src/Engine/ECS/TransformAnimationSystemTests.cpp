#include "pch.h"
#include "CppUnitTest.h"
#include "AssertUtils.h"

#include "Engine/ECS/Systems/TransformAnimationSystem.h"
#include "Engine/Assets/AssetRegistry.h"
#include "Engine/ECS/Entity.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Engine;

namespace TransformAnimationSystemTests
{
	TEST_CLASS(TransformAnimationSystemTests)
	{
	public:
		TEST_METHOD(StartEndTest)
		{
			std::vector<Animation::Segment> segments = {
					{
						1.0f, // duration
						{ { 1.0f, 2.0f, 3.0f}, { 0.0f, 0.0f, -10.0f }, { 1.0f, 1.0f, 1.0f } }, // start transform
						{ { 0.0f, 0.0f, 0.0f}, { 15.0f, 13.0f, 0.0f }, { 1.3f, 0.2f, 1.0f } }, // end transform
					},
			};
			AssetRegistry::Add("animation", new Animation(segments));
			Engine::Entity entity;
			Engine::Components::Animator animator{ "animation", Transform(), 0.0f };

			// move to start of animation
			Engine::Systems::TransformAnimationSystem(0.0f, entity, animator);

			Assert::AreEqual(segments[0].StartTransform, animator.Offset, L"Start doesn't match!");

			// move to end of animation
			Engine::Systems::TransformAnimationSystem(1.0f, entity, animator);

			Assert::AreEqual(segments[0].EndTransform, animator.Offset, L"End doesn't match!");

			AssetRegistry::Delete("animation");
		}

		TEST_METHOD(WrapTest)
		{
			std::vector<Animation::Segment> segments = {
					{
						0.7f, // duration
						{ { 1.0f, 2.0f, 3.0f}, { 0.0f, 0.0f, -10.0f }, { 1.0f, 1.0f, 1.0f } }, // start transform
						{ { 0.0f, 0.0f, 0.0f}, { 15.0f, 13.0f, 0.0f }, { 1.3f, 0.2f, 1.0f } }, // end transform
					},
			};
			AssetRegistry::Add("animation", new Animation(segments));
			Engine::Entity entity;
			Engine::Components::Animator animator{ "animation", Transform(), 0.0f };

			// wrap once
			Engine::Systems::TransformAnimationSystem(1.0f, entity, animator);

			Assert::AreEqual(0.3f, animator.AnimationTime, 0.01f);

			// wrap multiple times
			Engine::Systems::TransformAnimationSystem(7.0f, entity, animator);

			Assert::AreEqual(0.3f, animator.AnimationTime, 0.01f);

			AssetRegistry::Delete("animation");
		}
	};
}
