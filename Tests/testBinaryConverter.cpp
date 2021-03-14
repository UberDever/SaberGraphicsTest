#include "pch.h"
#include "../SaberGraphicsTest/Assignment/BinaryConverter.hpp"

TEST(BinaryConverter, CheckZero) {
	auto b = BinaryForm(0);
	EXPECT_EQ(atoi(b.toString()), 0);
}

TEST(BinaryConverter, CheckNegative) {
	auto b = BinaryForm(-2);
	EXPECT_TRUE(b.isNegative());
}

TEST(BinaryConverter, CheckLength) {
	auto b0 = BinaryForm(0);
	EXPECT_EQ(b0.getLength(), 1);
	auto b1 = BinaryForm(128);
	EXPECT_EQ(b1.getLength(), 8);
	auto b2 = BinaryForm(-127);
	EXPECT_EQ(b2.getLength(), 7);
	auto b3 = BinaryForm(13);
	EXPECT_EQ(b3.getLength(), 4);
}

TEST(BinaryConverter, CompareStrings)
{
	auto b0 = BinaryForm(0);
	EXPECT_STREQ(b0.toString(), "0");
	auto b1 = BinaryForm(0xE);
	EXPECT_STREQ(b1.toString(), "1110");
	auto b2 = BinaryForm(0xDEAD);
	EXPECT_STREQ(b2.toString(), "1101111010101101");
	auto b3 = BinaryForm(0xBEEF);
	EXPECT_STREQ(b3.toString(), "1011111011101111");
	auto b4 = BinaryForm(-0xD15EA5E);
	EXPECT_STREQ(b4.toString(), "1101000101011110101001011110");
	EXPECT_TRUE(b4.isNegative());

}