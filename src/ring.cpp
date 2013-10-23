#include "ring.hpp"

#ifndef NDEBUG
#include <cassert>
#endif

Ring::Ring(const char* key, unsigned int length, const char* salt, unsigned int saltLength, unsigned int mutationInterval)
{
#ifndef NDEBUG
	assert(length > 0);
	assert(length <= MAPSIZE);
	assert(saltLength > 0);
	assert(saltLength <= IVSIZE);
#endif
	this->saltLength = saltLength;
	for (unsigned int i=0; i<saltLength; i++)
	{
		this->initSalt[i] = (unsigned char)salt[i];
	}
	this->mutationInterval = mutationInterval;
	for (unsigned int i=0; i<length; i++)
	{
		this->pw[i] = (unsigned char)key[i];
	}
	this->pwLength = length;
	reinit();
}

Ring::~Ring()
{
}

void Ring::reinit()
{
	this->last = 0;
	this->actualSalt = 0;
	for (unsigned int i=0; i<MAPSIZE; i++)
	{
		this->map[i] = i;
	}
	this->operationsSinceMutation = 0;
	for (unsigned int i=0; i<saltLength; i++)
	{
		this->salt[i] = this->initSalt[i];
	}
	mutate();
}

void Ring::shuffle()
{
	for (unsigned int i=0; i<MAPSIZE; i++)
	{
		unsigned char index = (this->map[i] ^ this->salt[i%this->saltLength])%MAPSIZE;
		unsigned char tmp = this->map[index];
		this->map[index] = this->map[i];
		this->map[i] = tmp;
	}
	for (int i=0; i<MAPSIZE; i++)
	{
		decodeMap[this->map[i]] = i;
	}
}

void Ring::mutate()
{
	unsigned int count = 0;
	int prev[MAPSIZE];
	int next[MAPSIZE];
	unsigned char mapCopy[MAPSIZE];
	for (int i=0; i<MAPSIZE; i++)
	{
		mapCopy[i] = this->map[i];
		next[i] = (i+1)%MAPSIZE;
		int index = i-1;
		if (index < 0)
		{
			index += MAPSIZE;
		}
		prev[i] = index;
	}
	int actual = 0;
	int unchanged = 0;
	unsigned int actualPWEntry = 0;
	while (count < MAPSIZE)
	{
		int actualPWValue = ((unsigned int)(pw[actualPWEntry])+1)%(MAPSIZE-count);
		while (unchanged < actualPWValue)
		{
			actual = next[actual];
#ifndef NDEBUG
			assert(actual != -1);
#endif
			unchanged++;
		}
		unchanged = 0;
		actualPWEntry = (actualPWEntry + 1)%pwLength;
		this->map[actual] = mapCopy[count];
		count++;
		int nextMapEntry = next[actual];
		int prevMapEntry = prev[actual];
#ifndef NDEBUG
		assert(nextMapEntry != -1);
		assert(prevMapEntry != -1);
#endif
		next[prevMapEntry] = nextMapEntry;
		prev[nextMapEntry] = prevMapEntry;
#ifndef NDEBUG
		next[actual] = -1;
		prev[actual] = -1;
#endif
		actual = nextMapEntry;
	}
	for (int i=0; i<MAPSIZE; i++)
	{
		decodeMap[this->map[i]] = i;
	}
}

unsigned char Ring::encode(unsigned char c)
{
#ifndef NDEBUG
	assert(c < MAPSIZE);
#endif
	unsigned int saltIndex = this->actualSalt;
	this->actualSalt = (saltIndex + 1)%this->saltLength;
	unsigned int index = (c ^ this->salt[saltIndex])%MAPSIZE;
	unsigned char ret = (this->map[index] ^ this->last)%MAPSIZE;
	this->salt[saltIndex] = ret;
	if (this->mutationInterval != 0)
	{
		this->operationsSinceMutation++;
		if (this->operationsSinceMutation == this->mutationInterval)
		{
			shuffle();
			this->operationsSinceMutation = 0;
		}
	}
	this->last = c;
	return ret;
}

unsigned char Ring::decode(unsigned char c)
{
#ifndef NDEBUG
	assert(c < MAPSIZE);
#endif
	unsigned int saltIndex = this->actualSalt;
	this->actualSalt = (saltIndex + 1)%this->saltLength;
	unsigned char ret = (decodeMap[(c ^ this->last)%MAPSIZE] ^ this->salt[saltIndex])%MAPSIZE;
	this->salt[saltIndex] = c;
	if (this->mutationInterval != 0)
	{
		this->operationsSinceMutation++;
		if (this->operationsSinceMutation == this->mutationInterval)
		{
			shuffle();
			this->operationsSinceMutation = 0;
		}
	}
	this->last = ret;
	return ret;
}

void Ring::encode(char* c, unsigned int length)
{
#ifndef NDEBUG
	assert(length > 0);
#endif
	for (unsigned int i=0; i<length; i++)
	{
		c[i] = (char)encode((unsigned char)c[i]);
	}
}

void Ring::decode(char* c, unsigned int length)
{
#ifndef NDEBUG
	assert(length > 0);
#endif
	for (unsigned int i=0; i<length; i++)
	{
		c[i] = (char)decode((unsigned char)c[i]);
	}
}
