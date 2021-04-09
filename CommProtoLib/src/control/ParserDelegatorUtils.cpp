#include <commproto/control/ParserDelegatorUtils.h>
#include <commproto/parser/ParserDelegatorUtils.h>
#include "ButtonImpl.h"
#include "ToggleImpl.h"
#include "LabelImpl.h"
#include "NotificationImpl.h"

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
				parser::addParserHandlerPair<ux::NotificationResponseParser, ux::NotificationResponseMessage>(delegator, std::make_shared<ux::NotificationResponseHandler>(controller));
			}
		}
		namespace ux
		{
			void addParsers(const parser::ParserDelegatorHandle& delegator, const UIControllerHandle& controller)
			{
				parser::addParserHandlerPair<endpoint::ButtonParser, endpoint::ButtonMessage>(delegator, std::make_shared<endpoint::ButtonHandler>(controller));
				parser::addParserHandlerPair<endpoint::ToggleParser, endpoint::ToggleMessage>(delegator, std::make_shared<endpoint::ToggleHandler>(controller));
				parser::addParserHandlerPair<endpoint::LabelParser, endpoint::LabelMessage>(delegator, std::make_shared<endpoint::LabelHandler>(controller));
				parser::addParserHandlerPair<endpoint::LabelUpdateParser, endpoint::LabelUpdateMessage>(delegator, std::make_shared<endpoint::LabelUpdateHandler>(controller));
				parser::addParserHandlerPair<endpoint::NotificationParser, endpoint::NotificationMessage>(delegator, std::make_shared<endpoint::NotificationHandler>(controller));
				parser::addParserHandlerPair<endpoint::DisplayNotificationParser, endpoint::DisplayNotificationMessage>(delegator, std::make_shared<endpoint::DisplayNotificationHandler>(controller));
			}
		}
	}
}
