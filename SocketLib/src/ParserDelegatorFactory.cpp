#include <ParserDelegatorFactory.h>
#include "MappingType.h"


namespace commproto
{
	namespace data
	{

		class MappingTypeParser : public Parser
		{
		public:
			MappingTypeParser(const HandlerHandle & handler) : Parser{ handler } {}
			void parse(socketlib::ByteStream&& reader) override
			{
				std::string name;
				uint32_t typeId;
				reader.read(name);
				reader.read(typeId);
				handler->handle(std::move(MappingType(name, typeId)));
			}
		};


		class MappingTypeHandler : public Handler
		{
		public:
			MappingTypeHandler(const ParserDelegatorHandle & delegator_)
				: delegator{ delegator_ }
			{

			}

			void handle(TypeBase && data) override
			{
				MappingType & mapping = static_cast<MappingType&>(data);
				delegator->registerMapping(mapping.name, mapping.typeId);
			}

		private:
			ParserDelegatorHandle delegator;
		};


		ParserDelegatorHandle ParserDelegatorFactory::build()
		{

			ParserDelegatorHandle delegator = std::make_shared<ParserDelegator>();

			ParserHandle mappingParser = std::make_shared<MappingTypeParser>(std::make_shared<MappingTypeHandler>(delegator));

			delegator->registerParser<MappingType>(mappingParser);
			delegator->registerMapping(TypeName<MappingType>::name(), 0);


			return delegator;

		}
	}
}
