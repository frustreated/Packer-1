#pragma once

#include "Image.h"
#include "../Util/List.h"
#include "../Util/String.h"
#include "../Util/SharedPtr.h"
#include "../Util/DataSource.h"

class FormatBase
{
public:
	FormatBase() {}
	virtual ~FormatBase() {}

	virtual bool load(SharedPtr<DataSource> source, bool fromMemory) = 0;
	virtual void setFileName(const String &fileName) = 0;
	virtual void setFilePath(const String &filePath) = 0;
	virtual const String &getFileName() const = 0;
	virtual const String &getFilePath() const = 0;
	virtual Image toImage() = 0;
	virtual const List<Import> &getImports() = 0;
	virtual const List<ExportFunction> &getExports() = 0;
	virtual const ImageInfo &getInfo() const = 0;
	virtual const List<uint64_t> &getRelocations() = 0;
	virtual const List<Section> &getSections() const = 0;

	virtual void setSections(const List<Section> &sections) = 0;
	virtual void setRelocations(const List<uint64_t> &relocations) = 0;
	virtual void setImageInfo(const ImageInfo &info) = 0;

	virtual void save(SharedPtr<DataSource> target) = 0;
	virtual size_t estimateSize() const = 0;

	virtual bool isSystemLibrary(const String &filename) = 0;

	static SharedPtr<FormatBase> loadImport(const String &filename, const String &hint = String(""), int architecture = 0);
};