#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include "Vector.h"
#include "Image.h"
#include "List.h"

template<class ContainerType>
Vector<typename ContainerType::value_type> containerToDataStorage(const ContainerType &src)
{
	Vector<typename ContainerType::value_type> result;
	size_t cnt;
	result.resize(src.size());
	cnt = 0;
	for(auto &i : src)
		result.get()[cnt ++] = i;
	return result;
}

template<class ContainerType>
Vector<typename ContainerType::value_type> containerToDataStorage(ContainerType &&src)
{
	Vector<typename ContainerType::value_type> result;
	size_t cnt;
	result.resize(src.size());
	cnt = 0;
	for(auto &i : src)
		result.get()[cnt ++] = std::move(i);
	return result;
}

inline Vector<char> containerToDataStorage(std::string &&src)
{
	Vector<char> result;
	size_t cnt;
	result.resize(src.size() + 1);
	cnt = 0;
	for(auto &i : src)
		result.get()[cnt ++] = i;
	result.get()[cnt ++] = '\0';
	return result;
}

inline Vector<char> containerToDataStorage(const std::string &src)
{
	Vector<char> result;
	size_t cnt;
	result.resize(src.size() + 1);
	cnt = 0;
	for(auto &i : src)
		result.get()[cnt ++] = i;
	result.get()[cnt ++] = '\0';
	return result;
}

class FormatBase
{
public:
	FormatBase() {}
	virtual ~FormatBase() {}

	virtual Image serialize() = 0;
	virtual std::string getFilename() = 0;
	virtual std::shared_ptr<FormatBase> loadImport(const std::string &filename) = 0;
	virtual List<Import> getImports() = 0;

	virtual bool isSystemLibrary(const std::string &filename) = 0;
};