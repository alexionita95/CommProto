#ifndef COMMPROTO_TYPE_NAME_H
#define COMMPROTO_TYPE_NAME_H


#define DEFINE_DATA_TYPE(type)     \
    template <>                    \
    class MessageName<type>           \
    {                              \
    public:                        \
        static const char* name()  \
        {                          \
            return #type;          \
        }                          \
    };

namespace commproto 
{
    namespace messages{
        template <typename T>
        class MessageName{
        public:
            static const char* name(){
                return "";
            }
		};
    }
}

#endif // COMMPROTO_TYPE_NAME_H