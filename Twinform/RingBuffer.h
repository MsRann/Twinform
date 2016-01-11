#pragma once

#include <vector>

template <class T>
class RingBuffer
{
public:
	RingBuffer();
	RingBuffer(unsigned int size);

	void Add(T item);
	bool Get(unsigned int rewind, T& item) const;

	const unsigned int Size() const;

private:
	std::vector<T> mBuffer;
	// Internet indicates this will be optimized like a bitset (the internet is always right too)
	std::vector<bool> mUsedCache;
	unsigned int mIndxPtr;
	unsigned int mBufferFill;
	unsigned int mBufferSize;
};

template <class T>
RingBuffer<T>::RingBuffer()
{
	mBuffer.resize(10);

	mUsedCache.resize(10);
	for (unsigned int i = 0; i < 10; ++i)
		mUsedCache[i] = false;

	mIndxPtr = 0;
	mBufferFill = 0;
	mBufferSize = 10;
}

template <class T>
RingBuffer<T>::RingBuffer(unsigned int size)
{
	mBuffer.resize(size);

	mUsedCache.resize(size);
	for (unsigned int i = 0; i < size; ++i)
		mUsedCache[i] = false;

	mIndxPtr = 0;
	mBufferFill = 0;
	mBufferSize = size;
}

template <class T>
void RingBuffer<T>::Add(T item)
{
	mBuffer[mIndxPtr] = item;
	mUsedCache[mIndxPtr++] = true;

	// Wrap around
	if (mIndxPtr == mBufferSize)
		mIndxPtr = 0;

	if (mBufferFill < mBufferSize)
		++mBufferFill;
}

template <class T>
bool RingBuffer<T>::Get(unsigned int rewind, T& item) const
{
	// This is a weird case. 
	// If the current slot has an item in it (it would be the oldest)
	// and rewind is 0 just return it. If it doesn't one hasn't been inserted yet.
	if (rewind == 0 && mUsedCache[mIndxPtr])
	{
		if (mUsedCache[mIndxPtr]) 
		{
			item = mBuffer[mIndxPtr];
			return true;
		}

		return false;
	}

	// TODO:
	// BUG HERE WHEN INDEX ENDS UP NEGATIVE
	// TO REPRODUCE: Have characters spawn very close to platform

	int rewindIndx = mIndxPtr - rewind - 1;

	// If it's negative get the value from the top of the buffer minus the rewind
	if (rewindIndx < 0)
		rewindIndx = mBufferSize + rewindIndx;

	if (!mUsedCache[rewindIndx])
		return false;

	item = mBuffer[rewindIndx];
	return true;
}

template <class T>
const unsigned int RingBuffer<T>::Size() const
{
	return mBufferFill;
}