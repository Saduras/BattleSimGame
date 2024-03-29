#pragma once

// For use by Engine applications
#include "Engine/Application.h"
#include "Engine/Log.h"

#include "Engine/Layer.h"
#include "Engine/ImGui/ImGuiLayer.h"

#include "Engine/Input.h"
#include "Engine/MouseButtonCodes.h"
#include "Engine/KeyCodes.h"

#include "Engine/UUID.h"
#include "Engine/Assets/AssetRegistry.h"

#include "Engine/Math.h"
#include "Engine/Grid.h"
#include "Engine/String.h"
#include "Engine/Time.h"
#include "Engine/Random.h"
#include "Engine/DebugTool.h"

#include "Engine/WavefunctionCollapse/WavefunctionCollapse.h"

// ---Rendering--------------------------
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Sprite.h"
#include "Engine/Renderer/TextureAtlas.h"
#include "Engine/Renderer/Material.h"
// --------------------------------------

#include "Engine/Scene.h"

// ---ECS-Base--------------------------
#include "Engine/ECS/Entity.h"
// -------------------------------------

// ---Components-------------------------
#include "Engine/ECS/Components/Camera.h"
#include "Engine/ECS/Components/Renderable.h"
// --------------------------------------

// ---Systems----------------------------
#include "Engine/ECS/Systems/Render3DSystem.h"
#include "Engine/ECS/Systems/Render2DSystem.h"
#include "Engine/ECS/Systems/TransformAnimationSystem.h"
// --------------------------------------


// ---Entry Point------------------------
//#include "Engine/EntryPoint.h"
// --------------------------------------