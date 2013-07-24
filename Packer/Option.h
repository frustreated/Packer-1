#pragma once

#include <memory>

#include "List.h"
#include "Map.h"
#include "String.h"

class File;
class Option
{
private:
	List<std::shared_ptr<File>> inputFiles_;
	Map<String, bool> booleanOptions_;
	Map<String, String> stringOptions_;

	void parseOptions(int argc, List<String> rawOptions);
	void handleStringOption(const String &name, const String &value);
	bool isBooleanOption(const String &optionName);
public:
	Option(int argc, char **argv);
	Option(int argc, wchar_t **argv);

	List<std::shared_ptr<File>> getInputFiles() const;
};
