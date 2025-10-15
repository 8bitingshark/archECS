/// --------------------------------------------------------------
/// Layer
/// --------------------------------------------------------------
/// A layer is a logical container that represents an independent 
/// part of the engine's behavior or display.
/// 
/// For example you can have different layers with
/// different responsabilities, like Game Layer (with ECS, 
/// rendering), UI Layer, Editor Layer ...
/// 
/// Each Layer can receive Events from the Engine, 
/// it can be updated and rendered. 

#pragma once

#include "core/events/Event.h"
#include <string>

namespace core {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer") : m_debugName(name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnEvent(Event& event) {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}

		const std::string& GetName() const { return m_debugName; }

	protected:
		std::string m_debugName{};
	};
}