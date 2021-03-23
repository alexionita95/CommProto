#ifndef DELGATOR_PROVIDER_H
#define DELGATOR_PROVIDER_H


namespace commproto
{
	namespace endpoint
	{
		class DelegatorProvider
		{
		public:
			virtual parser::ParserDelegatorHandle provide(const std::string & name) = 0;
			virtual ~DelegatorProvider() = default;
		};

		using DelegatorProviderHandle = std::shared_ptr<DelegatorProvider>;
	}
}

#endif //DELGATOR_PROVIDER_H