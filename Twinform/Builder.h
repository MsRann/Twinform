#ifdef BUILDING
#pragma once

#include "TwinformTypes.h"
#include "Camera.h"

namespace Builder
{
	void Initialize(Camera& twinformWindow);
	// Might not need delta
	void Update(REAL delta);

	void SetCharacterSpawned(bool value);
}

#endif