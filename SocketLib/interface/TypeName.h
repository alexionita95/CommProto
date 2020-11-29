#ifndef COMMPROTO_TYPE_NAME_H
#define COMMPROTO_TYPE_NAME_H


#define DEFINE_DATA_TYPE(type)     \
    template <>                    \
    class TypeName<type>           \
    {                              \
    public:                        \
        static const char* name()  \
        {                          \
            return #type;          \
        }                          \
    };

namespace commproto 
{
    namespace data{
        template <typename T>
        class TypeName{
        public:
            static const char* name(){
                return "";
            }
		};
    }
}

#endif // COMMPROTO_TYPE_NAME_H