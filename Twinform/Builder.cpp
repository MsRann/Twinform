#ifdef BUILDING

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Builder.h"
#include "Renderer.h"
#include "TwinMath.h"
#include "Simulator.h"
#include "Creator.h"
#include "ActionTimeAccumulator.h"

namespace
{
	sf::RectangleShape mCurrentDrawable;
	Camera* mCamera = nullptr;
	bool mCharactersSpawned = false;
	ActionTimeAccumulator mDeleteAccumulator(5.0f);

	void ProcessMouseInputs();
	void ProcessKeyboardInputs();

	void ProcessMouseInputs()
	{
		if (!mCamera->GetWindow().hasFocus())
			return;

		sf::Vector2i mousePosition = sf::Mouse::getPosition(mCamera->GetWindow());
		mousePosition.x += (int)(mCamera->GetView().getCenter().x 
			- ((REAL)mCamera->GetWindowWidth() / 2.0f));
		mousePosition.y += (int)(mCamera->GetView().getCenter().y 
			- ((REAL)mCamera->GetWindowHeight() / 2.0f));
		twinmath::SnapToGrid(mousePosition, GRID_WIDTH_HALF, GRID_HEIGHT_HALF);
		mCurrentDrawable.setPosition((REAL)mousePosition.x, (REAL)mousePosition.y);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			HashedCellStorage& staticStorage = Simulator::GetStaticStorage();
			if (!staticStorage.IsPointColliding(mousePosition))
				Creator::MakeStaticGeometry(mousePosition, 
					sf::Vector2f(static_cast<REAL>(GRID_WIDTH_HALF), 
					static_cast<REAL>(GRID_HEIGHT_HALF))
				);
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && 
				mDeleteAccumulator.IsReady())
		{
			uint32_t id = Simulator::GetId(mousePosition);
			Creator::Delete(id);
			mDeleteAccumulator.Reset();
		}
	}

	void ProcessKeyboardInputs()
	{
		sf::Vector2f offset;

		// Process screen offset based on WASD
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			offset += sf::Vector2f(0.05f, 0.0f);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			offset += sf::Vector2f(-0.05f, 0.0f);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			offset += sf::Vector2f(0.0f, -0.05f);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			offset += sf::Vector2f(0.0f, 0.05f);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde) && !mCharactersSpawned)
		{
			Creator::MakeControllableCharacter(sf::Vector2f(0, 300), sf::Vector2f(25.0f, 25.0f), CONTROLS_WASD, sf::Vector2f(0.0f, 10.0f));
			//Creator::MakeControllableCharacter(sf::Vector2f(0, 400), sf::Vector2f(25.0f, 25.0f), CONTROLS_ARROWS, sf::Vector2f(0.0, -10.0f));
			mCharactersSpawned = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde) && mCharactersSpawned)
		{
			std::vector<ControllableCharacter*> characters;
			if (Creator::GetControllableCharacters(characters))
			{
				for (unsigned int i = 0; i < characters.size(); ++i)
				{
					Particle& fCharacterParticle = characters[i]->GetParticle();
					fCharacterParticle.ZeroMovement();
					// God, this is fucking dumb // I wish I had written why this is dumb when I wrote that comment
					fCharacterParticle.SetPosition(sf::Vector2f(0, 320.0f + 60.0f * (float)i));
				}
			}
		}

		mCamera->Translate(offset);
	}
}

void Builder::Initialize(Camera& twinformWindow)
{
	mCamera = &twinformWindow;
	mCurrentDrawable.setPosition(0.0f, 0.0f);
	mCurrentDrawable.setSize(sf::Vector2f(GRID_WIDTH / 2, GRID_HEIGHT / 2));
	mCurrentDrawable.setFillColor(sf::Color(0u, 0u, 0u, 0u));
	mCurrentDrawable.setOutlineThickness(2.0f);
	mCurrentDrawable.setOutlineColor(sf::Color(0u, 0u, 255u, 255u));
	Renderer::Add(0, mCurrentDrawable);
}

void Builder::Update(REAL delta)
{
	mDeleteAccumulator.Add(delta);
	ProcessMouseInputs();
	ProcessKeyboardInputs();
}

#endif