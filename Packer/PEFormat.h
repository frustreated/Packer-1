#pragma once

#include <array>
#include <memory>

#include "FormatBase.h"
#include "File.h"
#include "PEHeader.h"
#include "List.h"

class PEFormat : public FormatBase
{
private:
	std::array<IMAGE_DATA_DIRECTORY, 16> dataDirectories_;
	std::shared_ptr<File> file_;
	List<Section> sections_;
	List<Import> imports_;
	List<uint64_t> relocations_;
	List<ExtendedData> extendedData_;
	ImageInfo info_;
	void processDataDirectory();
	void processRelocation(IMAGE_BASE_RELOCATION *info);
	void processImport(IMAGE_IMPORT_DESCRIPTOR *descriptor);
	uint8_t *getDataPointerOfRVA(uint64_t rva);
public:
	PEFormat(std::shared_ptr<File> file);
	~PEFormat();

	virtual std::string getFilename();
	virtual std::shared_ptr<FormatBase> loadImport(const std::string &filename);
	virtual Image serialize();
	virtual List<Import> getImports();

	virtual bool isSystemLibrary(const std::string &filename);
};
