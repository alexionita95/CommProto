#ifndef BUTTON_IMPL_H
#define BUTTON_IMPL_H
#include "ControlBase.h"
#include <functional>

namespace commproto
{
	namespace control {

		class UxManager
		{
		public:
			UxManager(const std::string & name_, const std::string & connectionName_)
				: name{name_}
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
		using UxGeneratorHandle =  std::shared_ptr<UxGenerator>;

		class UxButton : public ControlBase {
		public:
			UxButton(const std::string& name_, const UxGeneratorHandle& generator_)
				: ControlBase{ name_ }
				, generator{ generator_ }
			{
			}

			std::string getUX() const override;
		private:
			UxGeneratorHandle generator;
		};

		class UxGenerator
		{
		public:
			UxGenerator(UxManager& manager_) 
			: manager{manager_}
			{
				
			}
			template <typename ControlType> 
			std::string generate(const ControlType& control) const;
		protected:
			UxManager& manager;

		};

		template <>
		inline std::string UxGenerator::generate(const UxButton& control) const
		{
			return manager.getConnectionName() + manager.getName() + control.getName();
		}


		using ButtonAction = std::function<void()>;

		class EndpointButton : public ControlBase {
		public:
			EndpointButton(const std::string & name, const ButtonAction & action_)
				: ControlBase(name)
				, action{ action_ }
			{

			}

			void press() const
			{
				action();
			}

			std::string getUX() const override
			{
				return "";
			}

		private:
			ButtonAction action;
		};

	}
}


#endif // BUTTON_IMPL_H