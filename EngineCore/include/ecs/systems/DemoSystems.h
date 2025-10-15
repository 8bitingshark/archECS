#pragma once
#include "ecs/System.h"
#include "ecs/Coordinator.h"
#include "ecs/Components/DemoComponents.h"
#include <DirectXMath.h>
#include "IRenderer.h"
#include "IShaderClass.h"

namespace ecs {

	class MovementSystem : public System {
	public:
		void Update(Coordinator& coordinator, float dt) {
			
			for (auto entity : m_Entities) {
				auto& transform = coordinator.GetComponent<Transform>(entity);
				auto& velocity = coordinator.GetComponent<Velocity>(entity);

				transform.rotation.y += velocity.angularSpeed * dt;

				// Circolar position
				transform.position.x = cosf(transform.rotation.y) * velocity.radius;
				transform.position.z = sinf(transform.rotation.y) * velocity.radius;
			}
		}
	};

	class RenderSystem : public System {
	public:

		void Init(gfx::IRenderer* renderer,
			gfx::IShaderClass* shader,
			const DirectX::XMMATRIX& view,
			const DirectX::XMMATRIX& projection)
		{
			m_Renderer = renderer;
			m_Shader = shader;
			m_View = view;
			m_Projection = projection;
		}

		void Update(Coordinator& coordinator)
		{
			m_Renderer->BeginFrame(0.1f, 0.1f, 0.15f, 1.0f);

			for (auto entity : m_Entities) {
				auto& transform = coordinator.GetComponent<Transform>(entity);
				auto& color = coordinator.GetComponent<Color>(entity);
				auto& meshRef = coordinator.GetComponent<MeshComponent>(entity);

				DirectX::XMMATRIX world =
					DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z) *
					DirectX::XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z) *
					DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);

				DirectX::XMFLOAT4X4 world4x4, view4x4, proj4x4;
				XMStoreFloat4x4(&world4x4, world);
				XMStoreFloat4x4(&view4x4, m_View);
				XMStoreFloat4x4(&proj4x4, m_Projection);

				m_Shader->SetMatrices(reinterpret_cast<float*>(&world4x4),
					reinterpret_cast<float*>(&view4x4),
					reinterpret_cast<float*>(&proj4x4));

				m_Shader->SetColor(reinterpret_cast<const float*>(&color.value));
				m_Shader->Bind();

				meshRef.mesh->LoadBuffersOnGPU();
				m_Renderer->Draw(meshRef.mesh->GetIndexCount());
			}

			m_Renderer->PresentFrame();
		}

	private:

		gfx::IRenderer* m_Renderer{};
		gfx::IShaderClass* m_Shader{};
		DirectX::XMMATRIX m_View;
		DirectX::XMMATRIX m_Projection;
	};

}