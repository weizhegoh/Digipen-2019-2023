#pragma once
#include <map>


	struct IComponent
	{


	};
namespace sComponent
{
	struct SeriliaztionFactory
	{
		std::map<int, std::pair<bool, IComponent*>> _objectComponent;
		std::string _name;
		ObjectType _obj;
		void addComponent(int index, bool _bool, IComponent* _component);
	};

	void SeriliaztionFactory::addComponent(int index, bool _bool, IComponent* _component)
	{
		_objectComponent.insert({ index, {_bool,_component} });
	}
}