
#pragma once

#include <vector>
#include <memory>
#include <type_traits>
#include <utility>
#include "Layer.h"

namespace core {

	class LayerStack {

	public:

		LayerStack() = default;
		~LayerStack();
		
		template<typename TLayer, typename... Args>
			requires(std::is_base_of_v<Layer, TLayer>)
		void PushLayer(Args&&... args) 
		{
			auto layer = std::make_unique<TLayer>(std::forward<Args>(args)...);
			layer->OnAttach();
			m_Layers.emplace_back(std::move(layer));
		}

		void PopLayer();

		/** Iterators */
		auto begin() { return m_Layers.begin(); }
		auto end() { return m_Layers.end(); }
		auto rbegin() { return m_Layers.rbegin(); }
		auto rend() { return m_Layers.rend(); }

		/** Const Iterators */
		auto begin() const { return m_Layers.begin(); }
		auto end() const { return m_Layers.end(); }
		auto rbegin() const { return m_Layers.rbegin(); }
		auto rend() const { return m_Layers.rend(); }

		/** Direct Access */
		std::vector<std::unique_ptr<Layer>>& GetLayers() { return m_Layers; }

	private:
		std::vector<std::unique_ptr<Layer>> m_Layers;
	};
}