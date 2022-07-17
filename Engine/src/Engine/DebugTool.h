#pragma once

#include "Engine/Math.h"

namespace Engine
{
	class Mesh;
	class Shader;
}

namespace Engine::Debug
{
	/// <summary>
	/// Specify which shader to use for debug drawing.
	/// </summary>
	/// <param name="path"></param>
	void SetShader(const std::string& path);

	/// <summary>
	/// Draws a line.
	/// </summary>
	/// <param name="start">Start point in pixel space</param>
	/// <param name="end">End point in pixel space</param>
	/// <param name="color">Color of the line</param>
	/// <param name="width">Width in pixel</param>
	void DrawLine(Vec2 start, Vec2 end, Vec3 color, float width);

	/// <summary>
	/// Renders all debug drawings for this frame and clears them afterwards.
	/// </summary>
	void Render();
}