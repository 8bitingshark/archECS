///
/// Input System class
/// 

#pragma once

#include <array>
#include <utility>

namespace core {

	class InputSystem {

	public:

		static InputSystem& GetInstance()
		{
			static InputSystem instance{};
			return instance;
		}

		void OnKeyDown(int key);
		void OnKeyUp(int key);

		bool IsKeyDown(int key) const;
		bool IsKeyUp(int key) const;

		void OnMouseButtonDown(int button);
		void OnMouseButtonUp(int button);

		bool IsMouseButtonDown(int button) const;

		void OnMouseMoved(float x, float y);
		std::pair<float, float> GetMousePosition() const;

	private:

		InputSystem() {
			m_Keys.fill(false);
			m_MouseButtons.fill(false);
		}

		std::array<bool, 256> m_Keys;
		std::array<bool, 8> m_MouseButtons;
		std::pair<float, float> m_MousePos{ 0.0f, 0.0f };
	};
}