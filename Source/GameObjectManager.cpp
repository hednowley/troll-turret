#include "stdafx.h"
#include "GameObjectManager.h"
#include "Game.h"

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
	std::for_each(_gameObjects.begin(),_gameObjects.end(),GameObjectDeallocator());
}

void GameObjectManager::Add(std::string name, VisibleGameObject* gameObject)
{
	_gameObjects.insert(std::pair<std::string,VisibleGameObject*>(name,gameObject));
}

void GameObjectManager::Remove(std::string name)
{
	std::map<std::string, VisibleGameObject*>::iterator results = _gameObjects.find(name);
	if(results != _gameObjects.end() )
	{
		delete results->second;
		_gameObjects.erase(results);
	}
}

VisibleGameObject* GameObjectManager::Get(std::string name) const
{
	std::map<std::string, VisibleGameObject*>::const_iterator results = _gameObjects.find(name);
	if(results == _gameObjects.end() )
		return NULL;
	return results->second;
	
}

int GameObjectManager::GetObjectCount() const
{
	return _gameObjects.size();
}


void GameObjectManager::DrawAll(sf::RenderWindow& renderWindow)
{

	std::map<std::string,VisibleGameObject*>::const_iterator itr = _gameObjects.begin();
	while(itr != _gameObjects.end())
	{
		itr->second->Draw(renderWindow);
		itr++;
	}
}

void GameObjectManager::UpdateAll()
{
	std::map<std::string,VisibleGameObject*>::const_iterator itr = _gameObjects.begin();

	
	float timeDelta = clock.restart().asSeconds();

	while(itr != _gameObjects.end())
	{
		itr->second->Update(timeDelta);
		itr++;
	}
	RemoveOutOfBounds();
}

// Remove objects out of screen bounds
void  GameObjectManager::RemoveOutOfBounds()
{
	std::map<std::string, VisibleGameObject*>::const_iterator itr = _gameObjects.begin();
	while (itr != _gameObjects.end())
	{
		sf::Rect<float> myRect = itr->second->GetBoundingRect();
		if (!myRect.intersects(sf::Rect<float>(0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT)))
		{
			std::map<std::string, VisibleGameObject*>::const_iterator toErase = itr;
			itr++;
			_gameObjects.erase(toErase);
		}
		else
			itr++;
	}
}

// Returns object map filtered to only trolls
std::map<std::string, VisibleGameObject*> GameObjectManager::ReturnTrolls()
{
	std::map<std::string, VisibleGameObject*> myMap;

	std::map<std::string, VisibleGameObject*>::const_iterator itr = _gameObjects.begin();
	while (itr != _gameObjects.end())
	{
		Troll* test = dynamic_cast<Troll*>(itr->second);
		if (test != NULL)
		{
			myMap.insert(std::pair<std::string, VisibleGameObject*>(itr->first, itr->second));
		}
		itr++;
	}

	return myMap;
}