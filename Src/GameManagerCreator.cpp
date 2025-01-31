#include "GameManagerCreator.h"

#include "GameManager.h"
#include "EntityComponent\Entity.h"
#include "LuaEngine\LuaManager.h"

#include <lua.hpp>
#include <LuaBridge.h>

CrazyU::GameManagerCreator::GameManagerCreator() {}

void CrazyU::GameManagerCreator::registerInLua() {
	
	auto L = Separity::LuaManager::getInstance()->getLuaState();
	luabridge::getGlobalNamespace(L)
	    .beginClass<GameManager>("GameManager")
	    .addFunction("addScore", &GameManager::addScore)
	    .addFunction("getScore", &GameManager::getScore)
	    .addFunction("getPercentage", &GameManager::getPercentageofTime)
	    .addFunction("nextParada", &GameManager::nextParada)
	    .addFunction("drawBuses", &GameManager::drawBuses)
		.endClass();
}

bool CrazyU::GameManagerCreator::createComponent(lua_State* L,
                                                 Separity::Entity* ent) {
	bool inGameScene = true;

	readParam("inGameScene", L, inGameScene);

	auto gm = ent->addComponent<GameManager>(inGameScene);

	auto L_Scripting = Separity::LuaManager::getInstance()->getLuaState();
	luabridge::setGlobal(L_Scripting, gm, "GameManager");

	return true;
}
