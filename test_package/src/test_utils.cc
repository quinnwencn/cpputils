#include "cpputils/string.h"

#include <gtest/gtest.h>

TEST(UtilsTest, EmptyArr2StrTest) {
    std::vector<uint8_t> empty;
    EXPECT_TRUE(Cpputils::ByteArr2HexStr(empty).empty());
}

TEST(UtilsTest, Arr2StrTest) {
    std::vector<uint8_t> array {1, 2, 3, 4, 0xa, 0xb};
    auto str = Cpputils::ByteArr2HexStr(array);
    
    EXPECT_EQ(str.length(), array.size() * 2);
    EXPECT_EQ("010203040a0b", str);
}
