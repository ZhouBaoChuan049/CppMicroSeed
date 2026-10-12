
#include <gtest/gtest.h>
#include <iostream>
#include <unordered_map>

std::unordered_map<int, int> mp;

class GoogleTestEnvironment : public testing::Environment
{
public:
    void SetUp() override{
        mp.insert({1, 1});
        mp.insert({2, 2});
        mp.insert({3, 3});
        std::cout << "全局环境初始化完成" << std::endl;
    }
    void TearDown() override{
        std::cout << "全局环境清理" << std::endl;
    }
};

TEST(GlobalMapTest, EraseTest){
    mp.erase(1);
    std::cout << "EraseTest: " << mp.size() << std::endl;
}

TEST(GlobalMapTest, InsertTest){
    mp.insert({4, 4});
    std::cout << "InsertTest: " << mp.size() << std::endl;
}

TEST(GlobalMapTest, CheckTest){
    std::cout << "CheckTest: " << mp.size() << std::endl;
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment(
        new GoogleTestEnvironment
    );
    return RUN_ALL_TESTS();
}
