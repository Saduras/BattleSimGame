#pragma once

#include <Engine/Math.h>

namespace Engine::Components
{
	class Camera
	{
	public:
		virtual ~Camera() {};

		inline Mat4 GetProjectionMatrix() { return m_Proj; }
		inline Mat4 GetProjectionMatrixInverse() { return m_ProjInverse; }

	// Static helpers
		static Vec4 ScreenToView(Vec2 screenPoint, float screenWidth, float screenHeight);
		static Vec4 ViewToWorld(Camera camera, Vec4 viewPoint);
		static Vec4 ScreenToWorld(Camera camera, Vec2 screenPoint, float screenWidth, float screenHeight);

	protected:
		Mat4 m_Proj;
		Mat4 m_ProjInverse;
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float zNear = -5.0f, float zFar = 5.0f);
	private:
		float m_Left;
		float m_Right;
		float m_Bottom;
		float m_Top;
		float m_ZNear;
		float m_ZFar;
	};
}