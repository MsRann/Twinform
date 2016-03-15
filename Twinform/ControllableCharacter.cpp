#include "ControllableCharacter.h"
#include "Renderer.h"
#include "Simulator.h"
#include "PropertyReader.h"
#include "ParticleSystem.h"
#include "TwinMath.h"

#include <SFML\Window\Keyboard.hpp>

#include <iostream> // TODO: Remove me

namespace
{
	static PropertyReader sCharacterSettings("Settings/character_settings.txt");

	static bool sSettingsLoaded = false;
	static REAL sRightForceX;
	static REAL sRightForceY;

	static REAL sLeftForceX;
	static REAL sLeftForceY;

	static REAL sJumpForceX;
	static REAL sJumpForceY;

	static REAL sDownForceX;
	static REAL sDownForceY;

	static REAL sRadius;
}

ControllableCharacter::ControllableCharacter() :
	Simulatable()
{
	LoadSettings();
}

ControllableCharacter::ControllableCharacter(sf::Vector2f start, sf::Vector2f size, ControllableControls controls, uint32_t id) :
	Simulatable(start, size)
	, mUpActionAccumulator(0.50f)
	, mRightActionAccumulator(0.05f)
	, mLeftActionAccumulator(0.05f)
	, mDownActionAccumulator(0.50f)
	, mParticleSpawnAccumulator(0.10f)
	, mSpaceActionAccumulator(MAX_DT)
	, mControls(controls)
	, mRewind(0)
{
	LoadSettings();
	mID = id;
	mDrawable.setPosition(start);
	mDrawable.setRadius(sRadius);
}

ControllableCharacter::~ControllableCharacter()
{
}

void ControllableCharacter::PrepareUpdate(REAL delta)
{
	UpdateAccumulators(delta);
	ApplyInputs();
}

void ControllableCharacter::PreUpdate(REAL delta)
{
	sf::Vector2f pos = mParticle.GetPosition();
	mDrawable.setPosition(pos);
	UpdateCollisionBounds();
	//ExecuteActionRight();

	//std::cout << mCollisionBounds.top << std::endl;

	/*if (mParticleSpawnAccumulator.IsReady())
	{
		sf::Vector2f dir = mParticle.GetVelocity();
		if (twinmath::LengthSquared(dir) > FLOAT_SMALL * FLOAT_SMALL)
			twinmath::Normalize(dir);
		sf::Vector2f middle = mParticle.GetPosition() + sf::Vector2f(sRadius, sRadius);
		ParticleSystem::Create(middle);
		mParticleSpawnAccumulator.Reset();
	}*/
}

void ControllableCharacter::UpdateAccumulators(REAL delta)
{
	mUpActionAccumulator.Add(delta);
	mDownActionAccumulator.Add(delta);
	mRightActionAccumulator.Add(delta);
	mLeftActionAccumulator.Add(delta);
	mSpaceActionAccumulator.Add(MAX_DT);
	mParticleSpawnAccumulator.Add(delta);
}

sf::CircleShape& ControllableCharacter::GetDrawable()
{
	return mDrawable;
}

void ControllableCharacter::ExecuteActionDown()
{
	if (!mDownActionAccumulator.IsReady())
		return;

	mParticle.AddForce(sf::Vector2f(sDownForceX, sDownForceY));
	mDownActionAccumulator.Reset();
}

void ControllableCharacter::ExecuteActionUp()
{
	if (!mUpActionAccumulator.IsReady())
		return;

	float jumpForce = sJumpForceY;
	if (mGravity.y < 0.0f)
		jumpForce *= -1;

	mParticle.AddForce(sf::Vector2f(sJumpForceX, jumpForce));
	mUpActionAccumulator.Reset();
}

void ControllableCharacter::ExecuteActionRight()
{
	if (!mRightActionAccumulator.IsReady())
		return;

	mParticle.AddVelocity(sRightForceX, sRightForceY);
	mRightActionAccumulator.Reset();
}

void ControllableCharacter::ExecuteActionLeft()
{
	if (!mLeftActionAccumulator.IsReady())
		return;

	mParticle.AddVelocity(sLeftForceX, sLeftForceY);
	mLeftActionAccumulator.Reset();
}

void ControllableCharacter::ApplyInputs()
{
	if (mControls == CONTROLS_ARROWS)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			ExecuteActionRight();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			ExecuteActionLeft();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && abs(mParticle.GetAcceleration().y) < abs(mGravity.y) + FLOAT_SMALL)
			ExecuteActionUp();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			ExecuteActionDown();
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			ExecuteActionRight();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			ExecuteActionLeft();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && abs(mParticle.GetAcceleration().y) < abs(mGravity.y) + FLOAT_SMALL)
			ExecuteActionUp();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			ExecuteActionDown();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mSpaceActionAccumulator.IsReady())
	{
		SetFlag(DONT_INTEGRATE);
		mRewind++;
		Particle oldParticle;
		if (mPast.Get(mRewind, oldParticle))
		{
			SetParticle(oldParticle);
			PreUpdate(MAX_DT);
		}
	}
	
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		mRewind = 0;
		UnsetFlag(DONT_INTEGRATE);
	}
}

void ControllableCharacter::LoadSettings()
{
	if (sSettingsLoaded)
		return;

	sCharacterSettings.ReadFloat("RightForceX", sRightForceX);
	sCharacterSettings.ReadFloat("RightForceY", sRightForceY);
	sCharacterSettings.ReadFloat("LeftForceX", sLeftForceX);
	sCharacterSettings.ReadFloat("LeftForceY", sLeftForceY);
	sCharacterSettings.ReadFloat("JumpForceX", sJumpForceX);
	sCharacterSettings.ReadFloat("JumpForceY", sJumpForceY);
	sCharacterSettings.ReadFloat("DownForceX", sDownForceX);
	sCharacterSettings.ReadFloat("DownForceY", sDownForceY);
	sCharacterSettings.ReadFloat("Radius", sRadius);
	sSettingsLoaded = true;
}