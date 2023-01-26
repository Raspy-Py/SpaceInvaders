#include "WidgetManager.h"

#include <exception>

bool WidgetManager::CheckIf(std::string widgetName, Widget::Command::Type commandType)
{
    auto pWidget = m_widgetMap.find(widgetName);

    if (pWidget != m_widgetMap.end())
    {
        auto optionalCommand = pWidget->second->GetCommand();
        if (optionalCommand.has_value())
        {
            return commandType == optionalCommand.value().GetType();
        }
        else
        {
            return false;
        }
    }
    else
    {
        std::string msg = "ERROR: trying to read command from widget [" + widgetName + "], which does not exist.";
        throw std::exception(msg.c_str());
    }
}

std::string WidgetManager::ReadInputFrom(std::string widgetName)
{
    auto pWidget = m_widgetMap.find(widgetName);

    if (pWidget != m_widgetMap.end())
    {
        auto optionalCommand = pWidget->second->GetCommand();
        if (optionalCommand.has_value())
        {
            return optionalCommand.value().GetArgs();
        }
        else
        {
            return "";
        }
    }
    else
    {
        std::string msg = "ERROR: trying to read input from widget [" + widgetName + "], which does not exist.";
        throw std::exception(msg.c_str());
    }
}

void WidgetManager::Draw(sf::RenderWindow& renderTarget)
{
    for (auto* pWidget : m_widgetArray)
        pWidget->Draw(renderTarget);
}

void WidgetManager::Update(float dt)
{
    for (auto* pWidget : m_widgetArray)
        pWidget->Update(dt);
}
