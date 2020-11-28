#include <gtest/gtest.h>

#include <ByteStream.h>



namespace commproto
{
	namespace socketlib
	{
		namespace test
		{
			template <typename T>
			const T & testValue()
			{
				return 42;
			}
			template <>
			const std::string & testValue()
			{
				return "test";
			}

			template <typename T>
			class ByteStreamTest : public testing::Test
			{
			};


			TYPED_TEST_SUITE_P(ByteStreamTest);

			TYPED_TEST_P(ByteStreamTest,CanReadAndWrite)
			{
				const TypeParam input = testValue<TypeParam>();
				TypeParam output;

				ByteStream writer;
				writer.write(input);
				ByteStream reader(writer.getStream());

				bool readResult = reader.read(output);
				ASSERT_TRUE(readResult);
				ASSERT_EQ(input, output);

			}

			REGISTER_TYPED_TEST_SUITE_P(ByteStreamTest,CanReadAndWrite);

			using WriteableTypes = ::testing::Types<uint8_t, int8_t,uint16_t,int16_t, uint32_t, uint64_t,int64_t,float_t,double_t>;
			INSTANTIATE_TYPED_TEST_SUITE_P(ByteStreamTestSuite, ByteStreamTest, WriteableTypes);


			TEST(ByteStream,CanReadAndWriteString)
			{
				ByteStream writer;
				std::string input = "test";
				std::string output;


				writer.write(input);

				ByteStream reader(writer.getStream());

				bool res = reader.read(output);

				ASSERT_TRUE(res);
				ASSERT_EQ(input, output);
			}
		}
	}
}