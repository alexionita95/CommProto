#ifndef CONTROL_H
#define CONTROL_H

#include <string>
#include <commproto/common/Common.h>
#include <memory>

namespace commproto
{
	namespace control {

		class BaseControl
		{
		public:
			BaseControl(const std::string& name_) : name{ name_ }
			{
			}

			std::string getName() const
			{
				return name;
			}
		protected:
			const std::string name;
		};


		namespace endpoint {
			class Control : public BaseControl {
			public:
				Control(const std::string & name) : BaseControl(name){}
				virtual ~Control() = default;
				virtual Message serialize() const = 0;
			};
			using ControlHandle = std::shared_ptr<Control>;
		}

		namespace ux
		{
			class Control : public BaseControl {
			public:
				Control(const std::string & name) : BaseControl(name) {}
				virtual ~Control() = default;
				virtual std::string getUx() = 0;
			};
			using ControlHandle = std::shared_ptr<Control>;
		}


	}
}


#endif // CONTROL_H