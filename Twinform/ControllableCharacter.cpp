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
	static PropertyReader mCharacterSettings("Settings/character_settings.txt");

	static bool mSettingsLoaded = false;
	static REAL mRightForceX;
	static REAL mRightForceY;

	static REAL mLeftForceX;
	static REAL mLeftForceY;

	static REAL mJumpForceX;
	static REAL mJumpForceY;

	static REAL mDownForceX;
	static REAL mDownForceY;

	static REAL mRadius;
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
	mDrawable.setRadius(mRadius);
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

	if (mParticleSpawnAccumulator.IsReady())
	{
		sf::Vector2f dir = mParticle.GetVelocity();
		if (twinmath::LengthSquared(dir) > FLOAT_SMALL * FLOAT_SMALL)
			twinmath::Normalize(dir);
		sf::Vector2f middle = mParticle.GetPosition() + sf::Vector2f(mRadius, mRadius);
		ParticleSystem::Create(middle);
		mParticleSpawnAccumulator.Reset();
	}
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

	mParticle.AddForce(sf::Vector2f(mDownForceX, mDownForceY));
	mDownActionAccumulator.Reset();
}

void ControllableCharacter::ExecuteActionUp()
{
	if (!mUpActionAccumulator.IsReady())
		return;

	float jumpForce = mJumpForceY;
	if (mGravity.y < 0.0f)
		jumpForce *= -1;

	mParticle.AddForce(sf::Vector2f(mJumpForceX, jumpForce));
	mUpActionAccumulator.Reset();
}

void ControllableCharacter::ExecuteActionRight()
{
	if (!mRightActionAccumulator.IsReady())
		return;

	mParticle.AddVelocity(mRightForceX, mRightForceY);
	mRightActionAccumulator.Reset();
}

void ControllableCharacter::ExecuteActionLeft()
{
	if (!mLeftActionAccumulator.IsReady())
		return;

	mParticle.AddVelocity(mLeftForceX, mLeftForceY);
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
	if (mSettingsLoaded)
		return;

	mCharacterSettings.ReadFloat("RightForceX", mRightForceX);
	mCharacterSettings.ReadFloat("RightForceY", mRightForceY);
	mCharacterSettings.ReadFloat("LeftForceX", mLeftForceX);
	mCharacterSettings.ReadFloat("LeftForceY", mLeftForceY);
	mCharacterSettings.ReadFloat("JumpForceX", mJumpForceX);
	mCharacterSettings.ReadFloat("JumpForceY", mJumpForceY);
	mCharacterSettings.ReadFloat("DownForceX", mDownForceX);
	mCharacterSettings.ReadFloat("DownForceY", mDownForceY);
	mCharacterSettings.ReadFloat("Radius", mRadius);
	mSettingsLoaded = true;
}