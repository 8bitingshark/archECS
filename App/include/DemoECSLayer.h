#pragma once

#include "core/Layer.h"
#include "IRenderer.h"
#include "IMesh.h"
#include "ecs/Coordinator.h"
#include "ecs/systems/DemoSystems.h"

namespace app {

	class DemoECSLayer : public core::Layer {

	public:

		DemoECSLayer(gfx::IRenderer* renderer);
		~DemoECSLayer() override = default;

		void OnAttach() override;
		void OnUpdate(float deltaTime) override;
		void OnRender() override;

	private:

		gfx::IRenderer* m_Renderer;              // non-owning
		std::unique_ptr<gfx::IMesh> m_Cube;

		// ECS core
		ecs::Coordinator m_Coordinator{};
		std::shared_ptr<ecs::MovementSystem> m_MoveSystem{};
		std::shared_ptr<ecs::RenderSystem>   m_RenderSystem{};

		// Camera
		DirectX::XMMATRIX m_View{};
		DirectX::XMMATRIX m_Projection{};

		// Shader
		std::shared_ptr<gfx::IShaderClass> m_Shader{};
	};

}