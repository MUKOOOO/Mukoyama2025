#pragma once
#include"gameObject.h"
#include<list>
#include<vector>
#include<typeinfo>

enum struct SCENE_STATE
{
	TITLE,
	STAGE
};

class Scene
{
protected:
	//GameObject* m_GameObject[4]{};
	std::list<GameObject*> m_GameObject[3];
	SCENE_STATE m_sceneState = SCENE_STATE::TITLE;
	int a;
public:
	virtual void Init()
	{
		
	}

	virtual void Uninit()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Uninit();
				delete gameObject;
			}

			m_GameObject[i].clear();//リスト構造の削除
		}
	}

	virtual void Update()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Update();
			}

			m_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });
		}	
	}

	virtual void Draw()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Draw();
			}
		}
	}

	template <typename T>
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		m_GameObject[Layer].push_back(gameObject);
		gameObject->Init();

		return gameObject;
	}

	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))
				{
					return (T*)object;
				}
			}
		}
		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetGameObjects()
	{ 
		std::vector<T*> objects;

		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}

	// シーンを跨いだスコアの保存
	void SetA(int s) { a = s; }
	int GetA(void) { return a; }

	SCENE_STATE GetSceneName() { return m_sceneState; }
};