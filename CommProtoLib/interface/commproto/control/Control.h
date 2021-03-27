#ifndef CONTROL_H
#define CONTROL_H

#include <string>
#include <commproto/common/Common.h>
#include <memory>

namespace commproto
{
    namespace control{
        class Control{
        public:
        virtual ~Control() = default;
        virtual std::string getName() const = 0;
        virtual std::string getUX() const = 0;
		};
        
        using ControlHandle = std::shared_ptr<Control>;
    }
}


#endif // CONTROL_H