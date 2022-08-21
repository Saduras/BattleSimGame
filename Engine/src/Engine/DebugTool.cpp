#include "epch.h"
#include "DebugTool.h"

#include "Engine/Assets/AssetRegistry.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine::Debug
{
	static UUID s_DebugShaderUUID;

	static MeshData s_MeshData = {
		{},
		{ 
			{ Engine::ShaderDataType::Float2, "a_Position"}, 
			{ Engine::ShaderDataType::Float3, "a_Color"},
		},
		{}
	};

	static void Reset()
	{
		s_MeshData.Verticies.clear();
		s_MeshData.Indicies.clear();
	}

	void SetShader(const std::string& path)
	{
		s_DebugShaderUUID = AssetRegistry::Add(new Shader(path));
	}

	void DrawLine(Vec2 start, Vec2 end, Vec3 color, float width)
	{
		auto& verts = s_MeshData.Verticies;
		auto& indicies = s_MeshData.Indicies;

		unsigned int index_offset = (unsigned int)verts.size() / 5;

		Vec2 direction = Normalize(end - start);
		Vec2 side = Vec2(-direction.y, direction.x);

		verts.insert(verts.end(), {
			start.x + side.x * width,	start.y + side.y * width,		color.r, color.g, color.b,
			end.x + side.x * width,		end.y + side.y * width,			color.r, color.g, color.b,
			end.x - side.x * width,		end.y - side.y * width,			color.r, color.g, color.b,
			start.x - side.x * width,	start.y - side.y * width,		color.r, color.g, color.b,
		});

		// 1st triangle
		indicies.insert(indicies.end(), { 
			index_offset + 0, index_offset + 1, index_offset + 2,	// 1st triangle
			index_offset + 2, index_offset + 3, index_offset + 0	// 2nd triangle
		});
	}

	void DrawRect(Vec2 bottomLeft, Vec2 topRight, Vec3 color, float lineWidth)
	{
		Vec2 topLeft = Vec2(bottomLeft.x, topRight.y);
		Vec2 bottomRight = Vec2(topRight.x, bottomLeft.y);

		DrawLine(topLeft, topRight, color, lineWidth);
		DrawLine(topRight, bottomRight, color, lineWidth);
		DrawLine(bottomRight, bottomLeft, color, lineWidth);
		DrawLine(bottomLeft, topLeft, color, lineWidth);
	}

	void Render()
	{
		if (!s_DebugShaderUUID.IsValid())
			return;

		Shader& shader = AssetRegistry::Get<Shader>(s_DebugShaderUUID);
		Mesh mesh(s_MeshData);

		Renderer::SetShader(&shader);
		shader.ApplyProperties();
		Renderer::Submit(mesh.GetVertexArray());

		Reset();
	}
}