// tests.cpp
#include "../src/BoundedBuffer.h"
#include "../src/ProducerConsumer.h"
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <string>

//Test checking if bounded buffer is empty
TEST(PCTest, TestEmpty) {
	BoundedBuffer *BB = new BoundedBuffer(5);
	EXPECT_TRUE(BB->isEmpty());

	delete BB;
}

//Test checking append() and remove() from buffer
TEST(PCTest, TestAddRemoveSimple){
	BoundedBuffer *BB = new BoundedBuffer(5);
	BB->append(0);
	ASSERT_EQ(0,BB->remove());

	delete BB;
}

TEST(VeryNotPCTest, TestFullBufferRemove_prefilled){
	BoundedBuffer *BB = new BoundedBuffer(5);
	
	for (int i = 0; i < 5; i ++){
		BB->append(i);
	}

	for (int i = 0; i < 5; i ++){
		ASSERT_EQ(i, BB->remove());
	}

	delete BB;
}

TEST(VeryNotPC, TestFullRemove2x){
	BoundedBuffer *BB = new BoundedBuffer(5);
	
	BB->append(0);
	BB->append(1);
	BB->append(2);
	BB->append(3);
	BB->append(4);

	for (int i = 0; i < 5; i ++){
		ASSERT_EQ(i, BB->remove());
	}

	BB->append(5);
	BB->append(6);
	BB->append(7);
	BB->append(8);
	BB->append(9);


	for (int i = 5; i < 10; i ++){
		ASSERT_EQ(i, BB->remove());
	}
	
	delete BB;
}

TEST(VeryNotPC, TestFullRemoveCircular1x){
	BoundedBuffer *BB = new BoundedBuffer(5);
	
	for (int j = 0; j < 1; j++ ) {

		for (int i = 0; i < 3; i ++){
			BB->append(i);
		}

		for (int i = 0; i < 1; i ++){
			ASSERT_EQ(i, BB->remove());
		}

		
		BB->append(3);
		BB->append(4);
		BB->append(5);

		for (int i = 1; i < 6; i ++){
			ASSERT_EQ(i, BB->remove());
		}
		
	}
	delete BB;
}

TEST(VeryNotPC, TestFullRemoveCircular1xRemove2ThenAppend){
	BoundedBuffer *BB = new BoundedBuffer(5);
	
	for (int j = 0; j < 1; j++ ) {

		for (int i = 0; i < 3; i ++){
			BB->append(i);
		}

		for (int i = 0; i < 2; i ++){
			ASSERT_EQ(i, BB->remove());
		}

		
		BB->append(3);
		BB->append(4);
		BB->append(5);
		BB->append(6);

		for (int i = 2; i < 7; i ++){
			ASSERT_EQ(i, BB->remove());
		}
		
	}
	delete BB;
}

TEST(VeryNotPC, TestFullRemoveCircular1xV3Remove3ThenAppend){
	BoundedBuffer *BB = new BoundedBuffer(5);
	
	for (int j = 0; j < 1; j++ ) {

		for (int i = 0; i < 3; i ++){
			BB->append(i);
		}

		for (int i = 0; i < 3; i ++){
			ASSERT_EQ(i, BB->remove());
		}

		
		BB->append(4);
		BB->append(5);
		BB->append(6);
		BB->append(7);
		BB->append(8);

		for (int i = 4; i < 9; i ++){
			ASSERT_EQ(i, BB->remove());
		}
		
	}
	delete BB;
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
