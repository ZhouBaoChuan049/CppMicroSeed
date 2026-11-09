#include <gtest/gtest.h>
#include <iostream>
#include <unordered_map>

class GoogleTestUnorderedMap : public testing::Test
{
protected:
    std::unordered_map<int, int> mp;

    // 整个测试套件开始前：只执行一次
    static void SetUpTestSuite()
    {
        std::cout << "\n===== 测试套件开始 =====\n";
    }

    // 整个测试套件结束后：只执行一次
    static void TearDownTestSuite()
    {
        std::cout << "===== 测试套件结束 =====\n";
    }

    // 每个测试用例开始前执行
    void SetUp() override
    {
        mp.clear();
        mp.insert({1, 1});
        mp.insert({2, 2});
        mp.insert({3, 3});

        std::cout << "\n===== 单个测试开始 =====\n";
        PrintMap();
    }

    // 每个测试用例结束后执行
    void TearDown() override
    {
        std::cout << "===== 单个测试结束 =====\n";
        PrintMap();
    }

    void PrintMap()
    {
        for (std::unordered_map<int, int>::const_iterator it = mp.begin();
             it != mp.end(); ++it)
        {
            std::cout << it->first << " -> "
                      << it->second << '\n';
        }
    }
};

TEST_F(GoogleTestUnorderedMap, InsertTest)
{
    mp.insert({4, 4});
    EXPECT_EQ(mp.size(), 4U);
}

TEST_F(GoogleTestUnorderedMap, EraseTest)
{
    mp.erase(1);
    EXPECT_EQ(mp.count(1), 0U);
}

TEST_F(GoogleTestUnorderedMap, ModifyTest)
{
    mp[2] = 99;
    EXPECT_EQ(mp[2], 99);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}