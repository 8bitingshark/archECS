#include "DemoECSLayer.h"
#include "TestMeshCube.h"
#include "DX11ShaderClass.h"
#include <random>
#include <filesystem>

app::DemoECSLayer::DemoECSLayer(gfx::IRenderer* renderer)
	: Layer("DemoECSLayer"), m_Renderer(renderer)
{
}

void app::DemoECSLayer::OnAttach()
{
	//m_Cube = std::make_unique<gfx::TestMeshCube>();
	//m_Cube->Create(m_Renderer);
	m_Coordinator.Init();
	m_Coordinator.RegisterComponent<ecs::Transform>();
	m_Coordinator.RegisterComponent<ecs::Velocity>();
	m_Coordinator.RegisterComponent<ecs::Color>();
	m_Coordinator.RegisterComponent<ecs::MeshComponent>();

	// Movement system
	m_MoveSystem = m_Coordinator.RegisterSystem<ecs::MovementSystem>();
	{
		std::bitset<ecs::MAX_COMPONENTS> sig;
		sig.set(m_Coordinator.GetComponentType<ecs::Transform>());
		sig.set(m_Coordinator.GetComponentType<ecs::Velocity>());
		m_Coordinator.SetSystemSignature<ecs::MovementSystem>(sig);
	}

	// Render system
	m_RenderSystem = m_Coordinator.RegisterSystem<ecs::RenderSystem>();
	{
		std::bitset<ecs::MAX_COMPONENTS> sig; sig;
		sig.set(m_Coordinator.GetComponentType<ecs::Transform>());
		sig.set(m_Coordinator.GetComponentType<ecs::Color>());
		sig.set(m_Coordinator.GetComponentType<ecs::MeshComponent>());
		m_Coordinator.SetSystemSignature<ecs::RenderSystem>(sig);
	}

	// --- Mesh e shader ---
	auto mesh = std::make_shared<gfx::TestMeshCube>();
	mesh->Create(m_Renderer);

	m_Shader = m_Renderer->CreateShader(
		L"C:/Develop/archECS/RenderCore/assets/shaders/simple_lit_vs.hlsl",
		L"C:/Develop/archECS/RenderCore/assets/shaders/simple_lit_ps.hlsl");

	// --- Camera ---
	DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 3.0f, -6.0f, 1.0f);
	DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_View = DirectX::XMMatrixLookAtLH(eye, at, up);
	m_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 16.0f / 9.0f, 0.1f, 100.0f);

	m_RenderSystem->Init(m_Renderer, m_Shader.get(), m_View, m_Projection);

	// --- Spawn entities ---
	std::mt19937 rng(1337);
	std::uniform_real_distribution<float> distPos(-10.0f, 40.0f);
	std::uniform_real_distribution<float> distCol(0.2f, 1.0f);
	std::uniform_real_distribution<float> distVel(0.2f, 1.0f);


	const int numCubes = 2000;
	for (int i = 0; i < numCubes; ++i)
	{
		auto e = m_Coordinator.CreateEntity();

		DirectX::XMFLOAT3 pos{ distPos(rng), distPos(rng) * 0.3f, distPos(rng) };
		DirectX::XMFLOAT3 rot{ 0.0f, distPos(rng) * 0.5f, 0.0f };
		DirectX::XMFLOAT3 scl{ 0.5f, 0.5f, 0.5f };

		m_Coordinator.AddComponent(e, ecs::Transform{ pos, rot, scl });
		m_Coordinator.AddComponent(e, ecs::Velocity{ distVel(rng), distPos(rng) });
		m_Coordinator.AddComponent(e, ecs::Color{
			{ distCol(rng), distCol(rng) * 0.8f, distCol(rng), 1.0f }
			});
		m_Coordinator.AddComponent(e, ecs::MeshComponent{ mesh });
	}
}

void app::DemoECSLayer::OnUpdate(float deltaTime)
{
	//m_Cube->LoadBuffersOnGPU();
	m_MoveSystem->Update(m_Coordinator, deltaTime);
}

void app::DemoECSLayer::OnRender()
{
	// render all
	// m_Renderer->BeginFrame(0.5, 0.5, 0.5, 1);
	// m_Renderer->PresentFrame();
	m_RenderSystem->Update(m_Coordinator);
}
