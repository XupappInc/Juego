#include "GameManager.h"

#include "EntityComponent/Entity.h"
#include "EntityComponent/Transform.h"

#include <EntityComponent/EntityManager.h>
#include <LuaEngine/Behaviour.h>
#include <PhysicsEngine/RigidBody.h>
#include <RenderEngine/MeshRenderer.h>
#include <RenderEngine/ParticleSystem.h>
#include <SceneEngine/SceneManager.h>
#include <UIEngine/Image.h>
#include <fstream>
#include <iostream>
#include <lua.hpp>
#include <LuaBridge.h>  //MANTENER EN ULTIMA POSICION

CrazyU::GameManager::GameManager(bool inGameScene)
    : paradaActual_(nullptr), paradaActualTr_(nullptr),
      paradas_(std::vector<Separity::Entity*>()), score_(0),
      paradasInitialized_(false), timeBetweenStops_(30000), currTime_(0),
      player_(nullptr), playerTr_(nullptr), indexParada_(-1),
      particleSys_(nullptr), particleSysTr_(nullptr), finalPoints_(0),
      sumScore_(20), isPlaying_(inGameScene) {

	if(isPlaying_) 
	{
		//los componentes se crean aqui para que se inicialicen con el ciclo del motor
		arrow_ = Separity::EntityManager::getInstance()->addEntity(
			Separity::_grp_GENERAL);
		arrowTr_ = arrow_->getComponent<Separity::Transform>();
		
		auto meshRenderer = arrow_->addComponent<Separity::MeshRenderer>();
		meshRenderer->setMesh("flecha.mesh");

		auto particleSysEnt = Separity::EntityManager::getInstance()->addEntity(
			Separity::_grp_GENERAL);
		particleSysTr_ = particleSysEnt->getComponent<Separity::Transform>();
		particleSys_ = particleSysEnt->addComponent<Separity::ParticleSystem>();
		particleSys_->setParticleSystem("ParticulasParadas",
										"particles/ExplosionB");
		particleSys_->setEmitting(true);
	}
}

CrazyU::GameManager::~GameManager() {}

void CrazyU::GameManager::start() {

	if(!isPlaying_)
		return;

	addParadas("SM_Prop_BusStop_01");
	nextParada();

	auto players =
	    Separity::EntityManager::getInstance()->getEntitiesByTag("Player");

	if(players.size() < 1) {
		std::cout << "[CRAZY U] GameManager: No hay player en la escena\n";
		return;
	}

	player_ = players[0];
	playerTr_ = player_->getComponent<Separity::Transform>();
}

void CrazyU::GameManager::update(const uint32_t& deltaTime) {
	
	maxTime_ = currTime_ + maxTimeAssigned; 
	if(!isPlaying_) 
	{
		return;
	}
	
	currTime_ += deltaTime;
	if(timeLeft() <= 0) {
		// isPlaying_ = false;
		Separity::SceneManager* sm = Separity::SceneManager::getInstance();
		writeFinalScore(scoreFileName_);
		sm->changeScene("finalScene.lua");
	}

	if(player_ != nullptr)
		arrowTr_->setPosition(playerTr_->getPosition() +
		                      Spyutils::Vector3(0, 5, 0));

	if(paradaActualTr_ != nullptr)
		arrowTr_->lookAt(paradaActualTr_->getPosition());

	else
		std::cout << "[CRAZY U] GameManager: Parada actual no asignada\n";
}

void CrazyU::GameManager::addScore(int score) {
	score_ += score;
	
	if(timeBetweenStops_ + (score_ * 1000) > maxTime_) {
		timeBetweenStops_ = maxTime_;
	}
	timeBetweenStops_ += score * 1000;
	finalPoints_ += sumScore_;
}

int CrazyU::GameManager::getScore() { return score_; }

void CrazyU::GameManager::addParadas(std::string tagParada) {
	if(!paradasInitialized_) {
		paradas_ =
		    Separity::EntityManager::getInstance()->getEntitiesByTag(tagParada);

		for(auto it = paradas_.begin(); it != paradas_.end();) {
			if((*it)->getComponent<Separity::Behaviour>() == nullptr) {
				it = paradas_.erase(it);
			} else
				it++;
		}

		paradasInitialized_ = true;
	}
}

void CrazyU::GameManager::nextParada() {
	if(paradas_.size() <= 0) {
		std::cout << "[CRAZY U] GameManager: No hay paradas en la escena\n";
		return;
	}

	if(indexParada_ < paradas_.size() - 1)
		indexParada_++;
	else
		indexParada_ = 0;

	paradaActual_ = paradas_[indexParada_];
	paradaActualTr_ = paradaActual_->getComponent<Separity::Transform>();

	repositionParticleSys();

	// indica al script de Lua de la siguiente parada que esta activo
	auto behaviour = paradaActual_->getComponent<Separity::Behaviour>();
	auto scriptLua = behaviour->getBehaviourLua();

	auto setActiveLua = (*scriptLua)["setActive"];

	if(setActiveLua.isFunction()) {
		setActiveLua();
	}
}

void CrazyU::GameManager::repositionParticleSys() {
	if(particleSys_ == nullptr) {
		std::cout << "[CRAZY U] GameManager: No existe Particle System\n";
		return;
	}

	particleSysTr_->setPosition(paradaActualTr_->getPosition());
}

float CrazyU::GameManager::timeLeft() {
	float timeLeft = timeBetweenStops_ - currTime_;
	float timeSecs = timeLeft / 1000;
	return timeSecs;
}

int CrazyU::GameManager::getPercentageofTime() {
	auto time = timeLeft();
	int percentage = (time / (maxTime_ / 1000)) * 100;
	return percentage;
}

int CrazyU::GameManager::getBusNum() {
	int buses = 0;
	int i = 1;
	while(i < maxBusScore_ + 1) {
		if(finalPoints_ < sumScore_ * i) {
			buses = i - 1;
			break;
		} else {
			++i;
		}
	}
	return buses;
}

int CrazyU::GameManager::readFinalScore(const std::string& fileName) {
	std::ifstream fich;
	fich.open(fileName, std::ofstream::in);
	if(!fich.is_open()) {
		std::cout << "[CRAZY U] GameManager : Error al abrir el archivo de puntuacion\n";
		return -1;
	}
	int num = 0;
	fich >> num;
	fich.close();
	return num;
}

void CrazyU::GameManager::writeFinalScore(const std::string& fileName) {
	std::ofstream fich;
	fich.open(fileName, std::ofstream::out);
	if(!fich.is_open()) {
		std::cout << "[CRAZY U] GameManager Clar: Error al abrir el archivo de puntuacion\n";
		return;
	}
	fich << getBusNum();
	fich.close();
}

void CrazyU::GameManager::drawBuses() {
	int num = readFinalScore(scoreFileName_);
	int offsetX = 20;
	int iniX = 10;
	int iniY = 70;
	int iniW = 210;
	int iniH = 162;

	for(int i = 0; i < num; i++) {
		auto busEnt = Separity::EntityManager::getInstance()->addEntity();
		busEnt->addComponent<Separity::Image>("Bus" + i, iniX + (offsetX * i),
		                                      iniY, iniW, iniH, "busScore", 3);
	}
}
