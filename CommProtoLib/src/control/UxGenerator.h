#ifndef UX_GENERATOR_H
#define UX_GENERATOR_H

#include <commproto/control/UIController.h>
#include "ButtonImpl.h"
#include <sstream>
#include "commproto/logger/Logging.h"

namespace commproto
{
	namespace control
	{
		namespace ux
		{
			class Generator
			{
			public:
				Generator(UIController& manager_)
					: manager{ manager_ }
				{

				}
				template <typename ControlType>
				std::string generate(const ControlType& control) const;

				void send(Message msg) const;
			protected:
				UIController& manager;

			};

			inline void Generator::send(Message msg) const
			{
				manager.send(msg);
			}

			template <typename ControlType>
			std::string Generator::generate(const ControlType& control) const
			{
				return "";
			}

			template <>
			inline std::string Generator::generate(const ButtonImpl& control) const
			{
				std::stringstream sstream;
				sstream << "<button onclick = \"post('" << manager.getConnectionName() << "','" << control.getName() << "')\"> Test </button>";
				return sstream.str();
			}

			using GeneratorHandle = std::shared_ptr<Generator>;
		}
	}
}

#endif //UX_GENERATOR_H