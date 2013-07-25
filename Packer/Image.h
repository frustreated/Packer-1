#pragma once

#include <cstdint>
#include <utility>
#include "Vector.h"
#include "String.h"

enum ArchitectureType
{
	ArchitectureWin32,
	ArchitectureWin32AMD64,
};
struct ImageInfo
{
	ArchitectureType architecture;
	uint64_t baseAddress;
	uint64_t entryPoint;
	uint64_t size;
};

enum SectionFlag
{
	SectionFlagCode = 1,
	SectionFlagData = 2,
	SectionFlagRead = 4,
	SectionFlagWrite = 8,
	SectionFlagExecute = 16,
};

struct ExtendedData //resource, header, ...
{
	ExtendedData() {}
	ExtendedData(ExtendedData &&operand) : baseAddress(operand.baseAddress), data(std::move(operand.data)) {}
	const ExtendedData &operator =(ExtendedData &&operand)
	{
		baseAddress = operand.baseAddress;
		data = std::move(operand.data);

		return *this;
	}
	uint64_t baseAddress;
	Vector<uint8_t> data;
};

struct Section
{
	Section() {}
	Section(Section &&operand) : name(std::move(operand.name)), baseAddress(operand.baseAddress), size(operand.size), data(std::move(operand.data)), flag(operand.flag) {}
	const Section &operator =(Section &&operand)
	{
		name = std::move(operand.name);
		baseAddress = operand.baseAddress;
		size = operand.size;
		data = std::move(operand.data);
		flag = operand.flag;

		return *this;
	}
	Vector<char> name;
	uint64_t baseAddress;
	uint64_t size;
	Vector<uint8_t> data;
	uint32_t flag;
};

struct ImportFunction
{
	ImportFunction() {}
	ImportFunction(ImportFunction &&operand) : ordinal(operand.ordinal), name(std::move(operand.name)), iat(operand.iat) {}
	const ImportFunction &operator =(ImportFunction &&operand)
	{
		ordinal = operand.ordinal;
		name = std::move(operand.name);
		iat = operand.iat;

		return *this;
	}
	uint16_t ordinal;
	Vector<char> name;
	uint64_t iat;
};

struct Import
{
	Import() {}
	Import(Import &&operand) : libraryName(std::move(operand.libraryName)), functions(std::move(operand.functions)) {}
	const Import &operator =(Import &&operand)
	{
		libraryName = std::move(operand.libraryName);
		functions = std::move(operand.functions);

		return *this;
	}
	Vector<char> libraryName;
	Vector<ImportFunction> functions;
};

struct Image
{
	Image() {}
	Image(Image &&operand) : info(operand.info), sections(std::move(operand.sections)), imports(std::move(operand.imports)), relocations(std::move(operand.relocations)), extendedData(std::move(operand.extendedData)), fileName(std::move(operand.fileName)) {}
	const Image &operator =(Image &&operand)
	{
		info = std::move(operand.info);
		sections = std::move(operand.sections);
		imports = std::move(operand.imports);
		relocations = std::move(operand.relocations);
		extendedData = std::move(operand.extendedData);
		fileName = std::move(operand.fileName);

		return *this;
	}
	ImageInfo info;
	String fileName;
	Vector<Section> sections;
	Vector<Import> imports;
	Vector<uint64_t> relocations;
	Vector<ExtendedData> extendedData;
};
