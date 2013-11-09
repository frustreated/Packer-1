#include "../../Packer/PEFormat.h"
#include "../../Packer/Win32Runtime.h"
#include "../../Packer/DataSource.h"
#include "../../Packer/Win32Loader.h"
#include "../Win32Stub.h"

int Entry()
{
	Win32NativeHelper::get()->init();
	PEFormat format;
	SharedPtr<MemoryDataSource> selfImageSource = MakeShared<MemoryDataSource>(reinterpret_cast<uint8_t *>(WIN32_STUB_BASE_ADDRESS), 0);
	format.load(selfImageSource, true);

	Image mainImage;
	List<Image> importImages;
	for(auto &i : format.getSections())
	{
		if(i.name == WIN32_STUB_MAIN_SECTION_NAME)
			mainImage = Image::unserialize(i.data, nullptr);
		else if(i.name == WIN32_STUB_IMP_SECTION_NAME)
		{
			size_t off = 0;
			size_t size = 0;
			while(off < i.data->size())
			{
				importImages.push_back(Image::unserialize(i.data->getView(off, 0), &size));
				off += size;
			}
		}
	}

	Win32NativeHelper::get()->unmapViewOfSection(reinterpret_cast<void *>(WIN32_STUB_BASE_ADDRESS));

	Win32Loader loader(mainImage, std::move(importImages));
	loader.execute();
	
	return 0;
}