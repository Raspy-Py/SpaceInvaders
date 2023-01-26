#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <optional>

class Widget
{
public:
	class Command
	{
	public:
		enum class Type
		{
			None = 0,
			Pressed,
			Hovered,
			Count
		};
		Command(Type type, std::string args)
			:
			m_type(type), m_args(args)
		{}
		Command()
			:
			m_type(Type::None), m_args("")
		{}
		Command(const std::string& args)
			:
			m_type(Type::None), m_args(args)
		{}

		Type GetType()
		{
			return m_type;
		}
		void SetType(Type type)
		{
			m_type = type;
		}
		const std::string& GetArgs()
		{
			return m_args;
		}
		bool friend operator==(const Command& left, const Command& right)
		{
			return left.m_type == right.m_type;
		}
	private:
		Type m_type;
		std::string m_args;
	};
public:
	Widget() {};
	
	virtual void Draw(sf::RenderWindow& renderTarget) = 0;
	virtual void Update(float dt) = 0;

	std::optional<Command> GetCommand()
	{
		return m_command;
	}
protected:
	Command m_command;
};
