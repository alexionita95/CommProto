#include <commproto/messages/SenderMaping.h>

namespace  commproto {


	SenderMapping* SenderMapping::instance = nullptr;

	SenderMapping::SenderMapping()
		: id{ 0 }
		, name{ "" }
	{

	}

	uint32_t SenderMapping::getId()
	{
		return getInstance()->id;
	}

	void SenderMapping::InitializeId(const uint32_t id_)
	{
		if (getInstance()->id != 0)
		{
			return;
		}
		getInstance()->id = id_;
	}

	std::string SenderMapping::getName()
	{
		return getInstance()->name;
	}

	void SenderMapping::InitializeName(const std::string& name_)
	{
		if (getInstance()->name.compare("") != 0)
		{
			return;
		}
		getInstance()->name = name_;
	}

	SenderMapping* SenderMapping::getInstance()
	{
		if (!instance)
		{
			instance = new SenderMapping();
		}
		return instance;
	}

}