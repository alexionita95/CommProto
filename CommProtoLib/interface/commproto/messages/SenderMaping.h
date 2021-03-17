
#ifndef  COMMPROTO_SENDER_MAPPING_H
#define  COMMPROTO_SENDER_MAPPING_H

#include <cstdint>
#include <string>

namespace commproto
{
	class SenderMapping
	{
	public:
		static uint32_t getId();
		static std::string getName();

		/**
		* \brief Initializes the id of the current sender. Only has effect once. This should be set once a connection with the dispatch server has been established.
		* \param id
		*/
		static void InitializeId(const uint32_t id);
		/**
		 * \brief Initializes the name of the current sender. Only has effect once. Ideally, this method should be called in your main() function.
		 * \param name
		 */
		static void InitializeName(const std::string & name);
		static SenderMapping * getInstance();
	private:
		SenderMapping();
		uint32_t id;
		std::string name;
		static SenderMapping * instance;
	};
}

#endif //COMMPROTO_SENDER_MAPPING_H