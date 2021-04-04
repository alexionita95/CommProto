#include <commproto/control/ParserDelegatorUtils.h>
#include <commproto/parser/ParserDelegatorUtils.h>
#include "ButtonImpl.h"
#include "ToggleImpl.h"

namespace commproto
{
	namespace control
	{
		namespace endpoint
		{

			void addParsers(const parser::ParserDelegatorHandle & delegator, const UIControllerHandle & controller)
			{
				parser::addParserHandlerPair<ux::PressButtonParser, ux::PressButtonMessage>(delegator, std::make_shared<ux::PressButtonHandler>(controller));
				parser::addParserHandlerPair<ux::ToggleParser, ux::ToggleMessage>(delegator, std::make_shared<ux::ToggleHandler>(controller));
			}
		}
		namespace ux
		{
			void addParsers(const parser::ParserDelegatorHandle& delegator, const UIControllerHandle& controller)
			{
				parser::addParserHandlerPair<endpoint::ButtonParser, endpoint::ButtonMessage>(delegator, std::make_shared<endpoint::ButtonHandler>(controller));
				parser::addParserHandlerPair<endpoint::ToggleParser, endpoint::ToggleMessage>(delegator, std::make_shared<endpoint::ToggleHandler>(controller));
			}
		}
	}
}
