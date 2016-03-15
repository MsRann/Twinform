#include "CommandStream.h"

#include <mutex>
#include <queue>

namespace
{
	std::queue<Command*> mCommandQueue;
	std::mutex mMutex;
}

void CommandStream::Add(Command* command)
{
	std::lock_guard<std::mutex> guard(mMutex);
	mCommandQueue.push(command);
}

void CommandStream::Execute(size_t count)
{
	std::lock_guard<std::mutex> guard(mMutex);
	for (size_t i = 0; i < count; ++i)
	{
		if (mCommandQueue.size() == 0)
			break;
		Command* com = mCommandQueue.front();
		com->Execute();
		mCommandQueue.pop();
		delete com;
	}
}
