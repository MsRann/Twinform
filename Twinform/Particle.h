#pragma once

#include <SFML/System/Vector2.hpp>

#include "TwinformTypes.h"

class Particle
{
public:
  Particle();
  Particle(const sf::Vector2f position);
  ~Particle();

  void Integrate(REAL duration);

  void Translate(const sf::Vector2f& offset);

  void SetMass(const REAL mass);
  REAL GetMass() const;

  void SetInverseMass(const REAL inverseMass);
  REAL GetInverseMass() const;

  bool HasFiniteMass() const;

  void SetDamping(const REAL damping);
  REAL GetDamping() const;

  void SetPosition(const sf::Vector2f& position);
  void SetPosition(const REAL x, const REAL y);
  sf::Vector2f GetPosition() const;

  void SetVelocity(const sf::Vector2f& velocity);
  void SetVelocity(const REAL x, const REAL y);
  sf::Vector2f GetVelocity() const;

  void AddVelocity(const sf::Vector2f& velocity);
  void AddVelocity(const REAL x, const REAL y);

  void DampenVelocity(const REAL scalar);

  void SetAcceleration(const sf::Vector2f& acceleration);
  void SetAcceleration(const REAL x, const REAL y);
  void AddAcceleration(const sf::Vector2f& acceleration);
  void AddAcceleration(const REAL x, const REAL y);
  sf::Vector2f GetAcceleration() const;

  void DampenAcceleration(const REAL scalar);

  void ClearAccumulator();

  void AddForce(const sf::Vector2f& force);
  void ZeroMovement();

  sf::Vector2f mPosition;
  sf::Vector2f mVelocity;
  sf::Vector2f mAcceleration;
private:
  sf::Vector2f mForceAccumulator;
  
  REAL mInverseMass;
  REAL mDamping;
};

