#ifndef BUTTON_IMPL_H
#define BUTTON_IMPL_H
#include <functional>
#include <commproto/control/Button.h>

namespace commproto
{
	namespace control {

		class UxManager
		{
		public:
			UxManager(const std::string & name_, const std::string & connectionName_)
				: name{ name_ }
				, connectionName{ connectionName_ }
			{

			}
			std::string getName() const
			{
				return name;
			}
			std::string getConnectionName() const
			{
				return connectionName;
			}
		protected:
			std::string name;
			std::string connectionName;
		};

		class UxGenerator;
		using UxGeneratorHandle = std::shared_ptr<UxGenerator>;

		namespace ux {
			class ButtonImpl : public Button {
			public:
				ButtonImpl(const std::string& name_, const UxGeneratorHandle& generator_)
					: Button{ name }
					, generator{ generator_ }
				{
				}

				void press() override
				{
				}

				std::string getUx() const override;
			private:
				UxGeneratorHandle generator;
			};
		}

		class UxGenerator
		{
		public:
			UxGenerator(UxManager& manager_)
				: manager{ manager_ }
			{

			}
			template <typename ControlType>
			std::string generate(const ControlType& control) const;
		protected:
			UxManager& manager;

		};

		template <>
		inline std::string UxGenerator::generate(const ux::ButtonImpl& control) const
		{
			return manager.getConnectionName() + manager.getName() + control.getName();
		}

		namespace endpoint {
			class ButtonImpl : public Button {
			public:
				ButtonImpl(const std::string & name, const ButtonAction & action_)
					: Button{ name }
					, action{ action_ }
				{
				}

				void press() override
				{
					action();
				}

				Message serialize() const override
				{
					Message msg;
					return msg;
				}
			private:
				ButtonAction action;
			};
		}

	}
}


#endif // BUTTON_IMPL_H