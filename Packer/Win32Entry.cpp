#include "PackerMain.h"
#include "../Win32/Win32Runtime.h"
#include "../Util/Util.h"

void WindowsEntry()
{
	List<String> arguments = Win32NativeHelper::get()->getArgumentList();

	PackerMain(Option(arguments)).process();
}