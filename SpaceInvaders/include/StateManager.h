#pragma once
#include "State.h"
#include "StateCommon.h"
#include "Utils.h"

#include <exception>
#include <string>
#include <map>

class StateManager
{
	using StateMap = std::map<std::string, std::unique_ptr<State>>;
public:
	StateManager() = default;
	void UpdateCurrent(float dt) noexcept(NDEBUG);
	void DrawCurrent(sf::RenderWindow& renderTarget) noexcept(NDEBUG);
	template <class StateType>
	void AddState() noexcept(NDEBUG);

private:
	void SwitchToState(SwitchStateParams switchParams) noexcept(NDEBUG);

private:
	StateMap m_states;
	State* mp_activeState = nullptr;
};

template<class StateType>
void StateManager::AddState() noexcept(NDEBUG)
{
	auto state = std::make_unique<StateType>();
	std::string stateName = RETRIEVE_NAME(StateType);
	m_states.emplace(stateName, std::move(state));

	// Assigning proper (or any if proper is not available) 
	// value to mp_activeState as we need it to keep app running
	if (typeid(StateType) == typeid(MainMenuState) || m_states.size() == 1)
	{
		auto pState = m_states.find(stateName);
		if (pState != m_states.end())
		{
			mp_activeState = pState->second.get();
		}
	}
}
