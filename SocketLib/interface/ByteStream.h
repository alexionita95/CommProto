#pragma once
#include <vector>
#include <string>

namespace commproto
{
	namespace socketlib
	{
		class ByteStream
		{
		public:
			ByteStream() : position(0)
			{

			}

			ByteStream(const std::vector<uint8_t> & stream_) : position(0) , stream(stream_)
			{

			}

			template <typename T>
			bool read(T& value);
			template <typename T>
			void write(const T& value);


			bool read(void* buf, const uint32_t size);
			bool write(const void* buf, const uint32_t size);

	


			const std::vector<uint8_t> & getStream() const
			{
				return stream;
			}

		private:
			std::vector<uint8_t> stream;
			uint32_t position;
		};

		template <typename T>
		void ByteStream::write(const T& value)
		{
			const uint8_t* valuePtr = reinterpret_cast<const uint8_t*>(&value);
			uint32_t valueSize = sizeof(value);
			position += valueSize;
			stream.insert(stream.end(), valuePtr, valuePtr + valueSize);
		}

		template <typename  T>
		bool ByteStream::read(T& value)
		{
			uint32_t valueSize = sizeof(value);
			if (position + valueSize > stream.size())
			{
				return false;
			}
			memcpy(&value, reinterpret_cast<T*>(stream.data() + position), valueSize);
			position += valueSize;
			return true;
		}

		inline bool ByteStream::write(const void* buf, const uint32_t size)
		{
			if (!size || !buf)
			{
				return false;
			}

			const uint8_t* bufPtr = reinterpret_cast<const uint8_t*>(buf);
			stream.insert(stream.end(), bufPtr, bufPtr + size);
			return true;
		}

		template <>
		inline void ByteStream::write(const std::string & value){
			write<uint32_t>(value.size());
			write(value.c_str(), value.size());
			write<uint8_t>(0);
		}

		inline bool ByteStream::read(void* buf, const uint32_t size)
		{
			if (!size || !buf || position + size > stream.size())
			{
				return false;
			}

			memcpy(buf, stream.data() + position, size);
			return true;
		}

		template <>
		inline bool ByteStream::read(std::string & value) {
			uint32_t size = 0;
			bool res = read(size);
			if(!res)
			{
				return false;
			}

			//won't work if the system doesn't store strings continuously in memory
			value.resize(size);
			value.reserve(size);
			return read(&value[0], size);
		}

	}
}
