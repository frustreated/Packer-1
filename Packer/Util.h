#pragma once

#include <cstdint>

template<typename IteratorType, typename Comparator>
typename IteratorType binarySearch(IteratorType begin, IteratorType end, Comparator comparator)
{
	int s = 0;
	int e = end - begin;

	while(s <= e)
	{
		int m = (s + e) / 2;
		int cmp = comparator(begin + m);
		if(cmp > 0)
			e = m - 1;
		else if(cmp < 0)
			s = m + 1;
		else
			return begin + m;
	}
	return end;
}

template<typename DestinationType, typename SourceType>
inline void copyMemory(DestinationType *dest_, const SourceType *src_, size_t size)
{
	uint8_t *dest = reinterpret_cast<uint8_t *>(dest_);
	const uint8_t *src = reinterpret_cast<const uint8_t *>(src_);
	if(!size)
		return;
	size_t i;
	for(i = 0; i < reinterpret_cast<size_t>(src) % sizeof(size_t); i ++)
		*(dest + i) = *(src + i); //align to boundary
	if(size > sizeof(size_t))
		for(; i < size - sizeof(size_t); i += sizeof(size_t))
			*reinterpret_cast<size_t *>(dest + i) = *reinterpret_cast<const size_t *>(src + i);
	for(; i < size; i ++)
		*(dest + i) = *(src + i); //remaining
}

template<typename DestinationType>
inline void zeroMemory(DestinationType *dest_, size_t size)
{
	uint8_t *dest = reinterpret_cast<uint8_t *>(dest_);
	if(!size)
		return;
	size_t i;
	for(i = 0; i < reinterpret_cast<size_t>(dest) % sizeof(size_t); i ++)
		*(dest + i) = 0; //align to boundary
	if(i > sizeof(size_t))
		for(; i < size - sizeof(size_t); i += sizeof(size_t))
			*reinterpret_cast<size_t *>(dest + i) = 0;
	for(; i < size; i ++)
		*(dest + i) = 0; //remaining
}

inline size_t multipleOf(size_t value, size_t n)
{
	return ((value + n - 1) / n) * n;
}

static const uint8_t *decodeSize(const uint8_t *ptr, uint8_t *flag, uint32_t *size)
{
	//f1xxxxxx
	//f01xxxxx xxxxxxxx
	//f001xxxx xxxxxxxx xxxxxxxx
	//f0001xxx xxxxxxxx xxxxxxxx xxxxxxxx
	//f0000100 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
	*size = *ptr ++;
	*flag = (*size & 0x80) >> 7;
	if(*size & 0x40)
		*size &= 0x3f;
	else if(*size & 0x20)
	{
		*size &= 0x1f;
		*size <<= 8;
		*size |= *(ptr ++);
	}
	else if(*size & 0x10)
	{
		*size &= 0x0f;
		*size <<= 8;
		*size |= *(ptr ++);
		*size <<= 8;
		*size |= *(ptr ++);
	}
	else if(*size & 0x08)
	{
		*size &= 0x07;
		*size <<= 8;
		*size |= *(ptr ++);
		*size <<= 8;
		*size |= *(ptr ++);
		*size <<= 8;
		*size |= *(ptr ++);
	}
	else if(*size & 0x04)
	{
		*size = *(ptr ++);
		*size <<= 8;
		*size |= *(ptr ++);
		*size <<= 8;
		*size |= *(ptr ++);
		*size <<= 8;
		*size |= *(ptr ++);
	}

	return ptr;
}

static uint32_t decompress(const uint8_t *compressedData, uint8_t *decompressedData)
{
	uint32_t controlSize = *reinterpret_cast<const uint32_t *>(compressedData);
	const uint8_t *data = compressedData + controlSize + 4;
	const uint8_t *control = compressedData + 4;
	const uint8_t *controlPtr = control;
	uint32_t resultSize = 0;

	uint8_t flag;
	size_t size;
	while(controlPtr < control + controlSize)
	{
		controlPtr = decodeSize(controlPtr, &flag, &size);
		resultSize += size;
		if(flag)
		{
			//succession
			for(; size > 0; size --)
				*decompressedData ++ = *data;
			data ++;
		}
		else
		{
			//nonsuccession
			for(; size > 0; size --)
				*decompressedData ++ = *data ++;
		}
	}

	return resultSize;
}
