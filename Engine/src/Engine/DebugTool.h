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
	/// Draws a rectange.
	/// </summary>
	/// <param name="bottomLeft">Bottom left corner in pixel space</param>
	/// <param name="topRight">Top right corner in pixel space</param>
	/// <param name="color">Color of the lines</param>
	/// <param name="lineWidth">Width of the lines</param>
	void DrawRect(Vec2 bottomLeft, Vec2 topRight, Vec3 color, float lineWidth);

	/// <summary>
	/// Draws a cross.
	/// </summary>
	/// <param name="position">Center position of cross</param>
	/// <param name="size">Length of lines</param>
	/// <param name="color">Color of the lines</param>
	/// <param name="lineWidth">Width of the lines</param>
	void DrawCross(Vec2 position, float size, Vec3 color, float lineWidth);

	/// <summary>
	/// Renders all debug drawings for this frame and clears them afterwards.
	/// </summary>
	void Render();
}