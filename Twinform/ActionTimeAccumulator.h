#pragma once

#include "TwinformTypes.h"

class ActionTimeAccumulator
{
public:
	ActionTimeAccumulator();
	ActionTimeAccumulator(REAL limit);

	void Add(REAL delta);
	void Subtract(REAL delta);
	void SetLimit(REAL limit);

	void Reset();
	bool IsReady();

	REAL GetAccumulation();

private:
	REAL mAccumulation;
	REAL mLimit;
};