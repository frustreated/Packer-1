#pragma once

#include <cstdint>
#include "SharedPtr.h"

class DataView;
class DataSource
{
public:
	virtual SharedPtr<DataView> getView(uint64_t offset, size_t size) = 0;
	virtual uint8_t *map(uint64_t offset) = 0;
	virtual void unmap() = 0;
};

class DataView
{
private:
	SharedPtr<DataSource> source_;
	uint64_t offset_;
	uint8_t *baseAddress_;
	size_t size_;
public:
	DataView(SharedPtr<DataSource> source, uint64_t offset, size_t size) : source_(source), offset_(offset), size_(size), baseAddress_(source_->map(offset_)) {}
	virtual ~DataView() 
	{
		if(baseAddress_)
			source_->unmap();
	}

	void unmap()
	{
		if(baseAddress_)
			source_->unmap();
		baseAddress_ = 0;
	}

	uint8_t *get() const
	{
		return baseAddress_;
	}

	size_t size() const
	{
		return size_;
	}

	SharedPtr<DataView> getView(uint64_t offset, size_t size) const
	{
		return MakeShared<DataView>(source_, offset_ + offset, size);
	}
};

class MemoryDataSource : public DataSource, public EnableSharedFromThis<MemoryDataSource>
{
private:
	uint8_t *memory_;
	size_t size_;
public:
	MemoryDataSource(uint8_t *memory, size_t size = 0) : memory_(memory), size_(size) {}
	virtual ~MemoryDataSource() {}

	virtual SharedPtr<DataView> getView(uint64_t offset, size_t size = 0)
	{
		if(size == 0)
			size = size_;
		return MakeShared<DataView>(sharedFromThis(), offset, size);
	}

	virtual uint8_t *map(uint64_t offset)
	{
		return memory_ + offset;
	}

	virtual void unmap()
	{

	}
};