#ifndef CONTROL_COLLECTION_H
#define CONTROL_COLLECTION_H
#include <commproto/control/Control.h>

namespace commproto
{
    namespace control
    {
        namespace endpoint
        {
            class ControlCollection
			{
			public:
				virtual ~ControlCollection() = default;
				virtual void addControl(const ControlHandle & control) = 0;
				virtual uint32_t reserveId() = 0;
				virtual ControlHandle getControl(const uint32_t id) = 0;
				virtual void clear() = 0;
			};
        }
        namespace ux
        {
            class ControlCollection
            {
                public:
                virtual ~ControlCollection() = default;
				virtual void addControl(const ControlHandle & control) = 0;
				virtual bool hasUpdate() = 0;
				virtual ControlHandle getControl(const uint32_t id) = 0;
				virtual void clear() = 0;
				virtual void notifyUpdate() = 0;
			};
        }
    }
}

#endif //CONTROL_COLLECTION_H