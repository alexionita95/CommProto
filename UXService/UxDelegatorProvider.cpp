#include "UxDelegatorProvider.h"
#include <commproto/parser/ParserDelegatorUtils.h>
#include <commproto/messages/TypeMapper.h>
#include <commproto/control/ux/ParserDelegatorUtils.h>
#include <commproto/logger/Logging.h>

commproto::parser::ParserDelegatorHandle buildSelfDelegator()
{
	std::shared_ptr<commproto::parser::ParserDelegator> delegator = std::make_shared<commproto::parser::ParserDelegator>();
	buildBase(delegator);
	return delegator;
}

UXServiceProvider::UXServiceProvider(const commproto::messages::TypeMapperHandle& mapper_, const commproto::sockets::SocketHandle& socket_, const commproto::control::ux::UxControllersHandle& controllers)
	: mapper{ mapper_ }
	, socket{ socket_ }
	, controllers{ controllers }
{
}

commproto::parser::ParserDelegatorHandle UXServiceProvider::provide(const std::string& name, const uint32_t id)
{
	commproto::parser::ParserDelegatorHandle delegator = buildSelfDelegator();
	auto controller = std::make_shared<commproto::control::ux::UIFactory>("UI", name, mapper, socket, id)->build();
	commproto::control::ux::addParsers(delegator, controller);
	controllers->addController(name, controller);

	LOG_INFO("Added controller for connection \"%s\" - %d", name.c_str(), id);

	return delegator;
}
