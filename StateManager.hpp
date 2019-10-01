#pragma once
#include "BaseState.hpp"
#include "SharedContext.hpp"
#include <unordered_map>
#include <functional>

using Types = std::vector<StateType>;
using States = std::vector<std::pair<StateType, BaseState*>>;
using StateFactory = std::unordered_map<StateType, std::function<BaseState*(void)>>;

class StateManager {
public:
	StateManager(SharedContext* shared);
	~StateManager();

	void Update(const sf::Time& time);
	void Draw();

	void ProcessRequests();

	void SwitchTo(const StateType& type);
	void Remove(const StateType& type);

	SharedContext* GetContext();
	bool HasState(const StateType& type);

private:

	void CreateState(const StateType& type);
	void RemoveState(const StateType& type);

	template <class T>
	void RegisterState(const StateType& type) {
		stateFactory[type] = [this]() -> BaseState* {
			return new T(this);
		};
	}

	SharedContext* shared;
	StateFactory stateFactory;
	States states;
	Types toRemove;
};

