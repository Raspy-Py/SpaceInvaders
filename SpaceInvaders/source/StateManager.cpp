#include "StateManager.h"
#include "StateCommon.h"

void StateManager::UpdateCurrent(float dt) noexcept(NDEBUG)
{
	if (mp_activeState)
	{
		try {
			if (auto changeStateParams = mp_activeState->Update(dt))
			{
				SwitchToState(std::move(changeStateParams.value()));
			}
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}
	else
	{
		throw std::exception("ERROR: trying to update state, while active state is not defined.");
	}
}

void StateManager::DrawCurrent(sf::RenderWindow& renderTarget) noexcept(NDEBUG)
{
	if (mp_activeState)
	{
		mp_activeState->Draw(renderTarget);
	}
	else
	{
		throw std::exception("ERROR: trying to draw state, while active state is not defined.");
	}
}

void StateManager::SwitchToState(SwitchStateParams switchParams) noexcept(NDEBUG)
{
	auto pStateToSwitch = m_states.find(switchParams.targetStateName);

	if (pStateToSwitch != m_states.end())
	{
		mp_activeState->Leave();
		mp_activeState = pStateToSwitch->second.get();
		mp_activeState->Init(switchParams.args);
	}
	else
	{
		std::string msg = "ERROR: no state [" + switchParams.targetStateName + "] found.";
		throw std::exception(msg.c_str());
	}
}
