#pragma once
#include "Widget.h"
#include "Utils.h"

#include <map>
#include <memory>


class WidgetManager
{
	using WidgetMap = std::map<std::string, std::unique_ptr<Widget>>;
public:
	WidgetManager() = default;

	template <class WidgetType>
	void AddWidget(const std::string& name);

	template <class WidgetType>
	void AddWidget(const std::string& name, std::unique_ptr<WidgetType> pWidget);

	template <class WidgetType, class ...Args>
	void AddWidget(const std::string& name, Args... args);

	template <class WidgetType>
	WidgetType& GetWidget(const std::string& name);

	bool CheckIf(std::string widgetName, Widget::Command::Type commandType);
	std::string ReadInputFrom(std::string widgetName);
	void Draw(sf::RenderWindow& renderTarget);
	void Update(float dt);
private:
	WidgetMap m_widgetMap;
	// Array of regular pointers for fast iteration, when drawing widgets
	std::vector<Widget*> m_widgetArray;
};

template<class WidgetType>
inline void WidgetManager::AddWidget(const std::string& name)
{
	auto pWidget = std::make_unique<WidgetType>();
	m_widgetArray.push_back(pWidget.get());
	m_widgetMap.emplace(name, std::move(pWidget));
}

template<class WidgetType>
inline void WidgetManager::AddWidget(const std::string& name, std::unique_ptr<WidgetType> pWidget)
{
	m_widgetArray.push_back(pWidget.get());
	m_widgetMap.emplace(name, std::move(pWidget));
}

template<class WidgetType, class ...Args>
inline void WidgetManager::AddWidget(const std::string& name, Args ...args)
{
	auto pWidget = std::make_unique<WidgetType>(args...);
	m_widgetArray.push_back(pWidget.get());
	m_widgetMap.emplace(name, std::move(pWidget));
}

template<class WidgetType>
inline WidgetType& WidgetManager::GetWidget(const std::string& name)
{
	auto pWidgetBase = m_widgetMap.find(name);

	if (pWidgetBase != m_widgetMap.end())
	{
		if (WidgetType* pWidget = dynamic_cast<WidgetType*>(pWidgetBase->second.get()))
		{
			return *pWidget;
		}
		else
		{
			std::string msg = "ERROR: failing to cast Widget to " + RETRIEVE_NAME(WidgetType);
			throw std::exception(msg.c_str());
		}
	}
	else
	{
		std::string msg = "ERROR: to get widget [" + name + "], which does not exist.";
		throw std::exception(msg.c_str());
	}
}
