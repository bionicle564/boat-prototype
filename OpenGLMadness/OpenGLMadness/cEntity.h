#pragma once
#include <string>

#include <vector>

class cComponent;

class cEntity
{
public:
	cEntity();
	virtual ~cEntity();

	template<class T> T* AddComponent();

	//if the component was made elsewhere, this takes ownership
	template<class T> void AddComponent(T* component);

	template<class T> T* GetComponent();
	template<class T> bool HasComponent();

	void RemoveComponent(cComponent* component);

	unsigned int id;
	std::string name;
	std::vector<cComponent*> components;

};

template<class T>
inline T* cEntity::AddComponent()
{
	T* newComponent = new T();
	if (dynamic_cast<cComponent*>(newComponent) == 0)
	{
		return 0;
	}

	this->components.push_back(newComponent);
	return newComponent;
	
}

template<class T>
inline void cEntity::AddComponent(T* component)
{
	this->components.push_back(component);
}

template<class T>
inline T* cEntity::GetComponent()
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		if (dynamic_cast<T*>(components[i]) != 0)
		{
			return dynamic_cast<T*>(components[i]);
		}
	}

	return 0;
}

template<class T>
inline bool cEntity::HasComponent()
{
	return false;
}
