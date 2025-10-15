#include "core/InputSystem.h"

/// ----------------------------------------------------------------
/// InputSystem::OnKeyDown
/// ----------------------------------------------------------------

void core::InputSystem::OnKeyDown(int key)
{
	m_Keys[key] = true;
}

/// ----------------------------------------------------------------
/// InputSystem::OnKeyUp
/// ----------------------------------------------------------------

void core::InputSystem::OnKeyUp(int key)
{
	m_Keys[key] = false;
}

/// ----------------------------------------------------------------
/// InputSystem::IsKeyDown
/// ----------------------------------------------------------------

bool core::InputSystem::IsKeyDown(int key) const
{
	return m_Keys[key];
}

/// ----------------------------------------------------------------
/// InputSystem::IsKeyUp
/// ----------------------------------------------------------------

bool core::InputSystem::IsKeyUp(int key) const
{
	return !m_Keys[key];
}

/// ----------------------------------------------------------------
/// InputSystem::OnMouseButtonDown
/// ----------------------------------------------------------------

void core::InputSystem::OnMouseButtonDown(int button)
{
	m_MouseButtons[button] = true;
}

/// ----------------------------------------------------------------
/// InputSystem::OnMouseButtonUp
/// ----------------------------------------------------------------

void core::InputSystem::OnMouseButtonUp(int button)
{
	m_MouseButtons[button] = false;
}

/// ----------------------------------------------------------------
/// InputSystem::IsMouseButtonDown
/// ----------------------------------------------------------------

bool core::InputSystem::IsMouseButtonDown(int button) const
{
	return m_MouseButtons[button];
}

/// ----------------------------------------------------------------
/// InputSystem::OnMouseMoved
/// ----------------------------------------------------------------

void core::InputSystem::OnMouseMoved(float x, float y)
{
	m_MousePos = { x, y };
}

/// ----------------------------------------------------------------
/// InputSystem::GetMousePosition()
/// ----------------------------------------------------------------

std::pair<float, float> core::InputSystem::GetMousePosition() const
{
	return m_MousePos;
}