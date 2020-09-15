#pragma once

// For use by Engine applications
#include "Engine/Application.h"
#include "Engine/Log.h"

#include "Engine/Layer.h"
#include "Engine/ImGui/ImGuiLayer.h"

#include "Engine/Input.h"
#include "Engine/MouseButtonCodes.h"
#include "Engine/KeyCodes.h"

// ---Rendering--------------------------
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/ECS/Components/Camera.h"
// --------------------------------------

// ---Components-------------------------
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/Mesh.h"
#include "Engine/ECS/Components/Material.h"
// --------------------------------------

// ---Entry Point------------------------
#include "Engine/EntryPoint.h"
// --------------------------------------