#pragma once

#include "Commands.h"

namespace CommandStream
{
	void Add(Command* command);
	void Execute(size_t count);
}