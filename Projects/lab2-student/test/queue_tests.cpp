// tests.cpp
#include "../src/BoundedBuffer.h"
#include "../src/ProducerConsumer.h"
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <string>

//Test checking if bounded buffer is empty
TEST(PCTest, Test1) {
    BoundedBuffer *BB = new BoundedBuffer(5);
    EXPECT_TRUE(BB->isEmpty());

    delete BB;
}

//Test checking append() and remove() from buffer
TEST(PCTest, Test2){
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


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
