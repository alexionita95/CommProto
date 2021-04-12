#ifndef UX_CONTROL_PARSER_DELEGATOR_UTILS_H
#define UX_CONTROL_PARSER_DELEGATOR_UTILS_H

#include <commproto/parser/ParserDelegator.h>
#include <commproto/control/ux/UIController.h>

namespace commproto
{
    namespace control
    {
		namespace ux
		{
			void addParsers(const parser::ParserDelegatorHandle & delegator, const UIControllerHandle & controller);
		}
    }
}

#endif //UX_CONTROL_PARSER_DELEGATOR_UTILS_H