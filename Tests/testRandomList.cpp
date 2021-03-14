#include "pch.h"
#include "../SaberGraphicsTest/Assignment/RandomList.hpp"

class RandomListTest : public ::testing::Test
{
protected:
	// Separate variables were chosen for index clarification
	RandomList list;
	ListNode* _1;
	ListNode* _2;
	ListNode* _3;
	ListNode* _4;
	ListNode* _5;
	ListNode* _6;
	ListNode* _7;
	ListNode* _8;
	virtual void SetUp()
	{
		_1 = new ListNode("one");
		_2 = new ListNode("two");
		_3 = new ListNode("three");
		_4 = new ListNode("four");
		_5 = new ListNode("five");
		_6 = new ListNode("six");
		_7 = new ListNode("seven");
		_8 = new ListNode("eightAndAlsoVeryLongStringToStore,soSorryForThis,IMustHaveBeenInsaneThenWroteThis");

		list.addNode(_1);
		list.addNode(_2);
		list.addNode(_3);
		list.addNode(_4);
		list.addNode(_5);
		list.addNode(_6);
		list.addNode(_7);
		list.addNode(_8);

		// Link randomly
		_1->rand = _4;
		_2->rand = _3;
		_3->rand = _8;
		_4->rand = NULL;
		_5->rand = _3;
		_6->rand = _7;
		_7->rand = _7;
		_8->rand = _5;
	}

	virtual void TearDown()
	{
	}
};
TEST_F(RandomListTest, Print) {
	testing::internal::CaptureStdout();
	list.printNodes();
	std::string output = testing::internal::GetCapturedStdout();

	EXPECT_STREQ(output.c_str(),
		"one\n"
		"two\n"
		"three\n"
		"four\n"
		"five\n"
		"six\n"
		"seven\n"
		"eightAndAlsoVeryLongStringToStore,soSorryForThis,IMustHaveBeenInsaneThenWroteThis\n");
}

TEST_F(RandomListTest, PrintWithRandom)
{
	testing::internal::CaptureStdout();
	list.printNodesWithRand();
	std::string output = testing::internal::GetCapturedStdout();

	EXPECT_STREQ(output.c_str(),
		"one.r = four\n"
		"two.r = three\n"
		"three.r = eightAndAlsoVeryLongStringToStore,soSorryForThis,IMustHaveBeenInsaneThenWroteThis\n"
		"four.r = NULL\n"
		"five.r = three\n"
		"six.r = seven\n"
		"seven.r = seven\n"
		"eightAndAlsoVeryLongStringToStore,soSorryForThis,IMustHaveBeenInsaneThenWroteThis.r = five\n");
}

TEST_F(RandomListTest, GetCount)
{
	EXPECT_EQ(list.getCount(), 8);
}

TEST_F(RandomListTest, SerializeAndDeserialize)
{
	FILE* binFile = fopen("random_list.bin", "wb");
	ASSERT_FALSE(binFile == NULL);
	list.Serialize(binFile);
	fclose(binFile);

	binFile = fopen("random_list.bin", "rb");
	auto newList = RandomList();
	newList.Deserialize(binFile);
	fclose(binFile);

	EXPECT_EQ(newList.getCount(), 8);

	testing::internal::CaptureStdout();
	newList.printNodesWithRand();
	std::string output = testing::internal::GetCapturedStdout();

	EXPECT_STREQ(output.c_str(),
		"one.r = four\n"
		"two.r = three\n"
		"three.r = eightAndAlsoVeryLongStringToStore,soSorryForThis,IMustHaveBeenInsaneThenWroteThis\n"
		"four.r = NULL\n"
		"five.r = three\n"
		"six.r = seven\n"
		"seven.r = seven\n"
		"eightAndAlsoVeryLongStringToStore,soSorryForThis,IMustHaveBeenInsaneThenWroteThis.r = five\n");
}
