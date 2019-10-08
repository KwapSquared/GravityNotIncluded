#include "PCH.h"
#include "Other/SM/StateLibrary.h"
#include "..\Header\GravityNotIncluded.h"

StateLibrary* StaticStateLibrary = new StateLibrary();

const short MEMBERLISTSIZE = 20;

const short NumPlanet = 5;
const short NumEntity	= MEMBERLISTSIZE * NumPlanet;
const short NumDisease	= 5;
const short NumCure		= 5;
const short NumCAI		= NumCure;
const short NumDAI		= NumDisease;
const short NumEAI		= NumEntity;
const short NumPAI		= NumPlanet;
const short NumPhysics	= NumPlanet;
const short NumSound	= NumPlanet;
const short NumGraphics = NumPlanet;



GravityNotIncluded::GravityNotIncluded()
{
	TempPool = new ObjectPool(NumDisease, NumCure, NumEntity, NumPlanet, NumEAI, NumCAI, NumDAI, NumPAI, NumGraphics, NumPhysics, NumSound);
	StaticStateLibrary->init();
	PlanetList = new Planet*[NumPlanet]();
	ConstructWorld();
}

void GravityNotIncluded::GameLoop()
{

	std::ostringstream SS;
	std::string temp;
	Timer tempTimer;
	for (int x = 0; x < 201; x++)
	{

		ProcessAI();
		if (x % 50 == 0)
		{
			temp = "Resources Turn(" + std::to_string(x) + "): " + PlanetList[0]->getID() + " Food: " + std::to_string(PlanetList[0]->getResources().Food) + ", Water: " +
				std::to_string(PlanetList[0]->getResources().Water) + ", Tech: " + std::to_string(PlanetList[0]->getResources().Tech) + ", Meds: " +
				std::to_string(PlanetList[0]->getResources().Medicine) + ", Wood " + std::to_string(PlanetList[0]->getResources().Wood);
			message(_TestGame, temp, __FILE__, __LINE__);

			temp = "Resources Turn(" + std::to_string(x) + "): " + PlanetList[1]->getID() + " Food: " + std::to_string(PlanetList[1]->getResources().Food) + ", Water: " +
				std::to_string(PlanetList[1]->getResources().Water) + ", Tech: " + std::to_string(PlanetList[1]->getResources().Tech) + ", Meds: " +
				std::to_string(PlanetList[1]->getResources().Medicine) + ", Wood " + std::to_string(PlanetList[1]->getResources().Wood);
			message(_TestGame, temp, __FILE__, __LINE__);
		}
	}
}


void GravityNotIncluded::Renderer() {}
void GravityNotIncluded::ProcessAI() 
{
	for (int x = 0; x < NumPlanet; x++)
	{
		if (PlanetList[x] == nullptr)
		{
			continue;
		}

		CreateNewGO(PlanetList[x]);
		if (PlanetList[x]->getDisease() != nullptr)
		{
			PlanetList[x]->getDisease()->getAI()->updateAI();
		}

		if (PlanetList[x]->getCure() != nullptr)
		{
			PlanetList[x]->getCure()->getAI()->updateAI();
		}

		for (int y = 0; y < MEMBERLISTSIZE; y++)
		{
			if (PlanetList[x]->getMember(y) == nullptr)
			{
				continue;
			}

			PlanetList[x]->getMember(y)->getAI()->updateAI();
			if (PlanetList[x]->getMember(y)->peekState().getID().compare(StateIDList[DEAD]) == 0)
			{
				message(_TestGame, " Entity Died :(", __FILE__, __LINE__);
				DeleteGO(PlanetList[x]->getMember(y));
				PlanetList[x]->getMember(y)->~Entity();
				PlanetList[x]->removeMember(y);
			}
		}
		PlanetList[x]->getAI()->updateAI();
	}
}

void GravityNotIncluded::ProcessGraphics() {}
void GravityNotIncluded::ProcessSound() {}
void GravityNotIncluded::ProcessPhysics() {}

void GravityNotIncluded::DeleteGO(GameObject *t_GO) 
{
	TempPool->onDestroy(*t_GO);
}
void GravityNotIncluded::ConstructWorld() 
{
	PlanetList[0] = TempPool->newPlanet();
	PlanetList[0]->setID("EARTH");

	PlanetList[1] = TempPool->newPlanet();
	PlanetList[1]->setID("MARS");
}


void GravityNotIncluded::CreateNewGO(Planet *t_Planet)
{
	std::string t_message = t_Planet->getAI()->updateMessage;
	if (t_message.compare("") == 0)
	{
		return;
	}

	if (t_message.compare("CREATE NEW CURE") == 0)
	{
		message(_TestGame, "Create New Cure. On Planet: " + t_Planet->getID(), __FILE__, __LINE__);
		t_Planet->setCure(TempPool->newCure(5));
	}
	else if (t_message.compare("CREATE NEW DISEASE") == 0)
	{
		message(_TestGame, "Create New Disease. On Planet: " + t_Planet->getID(), __FILE__, __LINE__);
		t_Planet->setDisease(TempPool->newDisease(5));
	}
	else if (t_message.compare("CREATE NEW ENTITY") == 0)
	{
		if (t_Planet->CheckAvalibility() == TRUE)
		{
			message(_TestGame, "Create New Entity. On Planet: " + t_Planet->getID() , __FILE__, __LINE__);
			Entity *temp = TempPool->newEntity();
			t_Planet->addMember(temp);
		}
	}
	else if (t_message.compare("DELETE DISEASE") == 0)
	{
		message(_TestGame, "Deleted Disease And Cure. On Planet: " + t_Planet->getID(), __FILE__, __LINE__);
		DeleteGO(t_Planet->getDisease());
		t_Planet->getDisease()->~Disease();
		DeleteGO(t_Planet->getCure());
		t_Planet->getCure()->~Cure();
		t_Planet->setCure(nullptr);
		t_Planet->setDisease(nullptr);
		
	}
	t_Planet->getAI()->updateMessage = "";
}


GravityNotIncluded::~GravityNotIncluded()
{
	for (int x = 0; x < NumPlanet; x++)
	{
		if (PlanetList[x] == nullptr)
		{
			continue;
		}
		PlanetList[x]->~Planet();
	}
	delete[] PlanetList;
	delete TempPool;
}
