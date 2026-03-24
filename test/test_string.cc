#include "cpputils/string.h"

#include <gtest/gtest.h>

TEST(ByteArr2HexStr, EmptyArray) {
    std::vector<uint8_t> empty;
    EXPECT_TRUE(Cpputils::ByteArr2HexStr(empty).empty());
}

TEST(ByteArr2HexStr, BasicConversion) {
    std::vector<uint8_t> array {1, 2, 3, 4, 0xa, 0xb};
    auto str = Cpputils::ByteArr2HexStr(array);

    EXPECT_EQ(str.length(), array.size() * 2);
    EXPECT_EQ("010203040a0b", str);
}

TEST(ByteArr2HexStr, SingleByte) {
    EXPECT_EQ("00", Cpputils::ByteArr2HexStr({0x00}));
    EXPECT_EQ("ff", Cpputils::ByteArr2HexStr({0xFF}));
    EXPECT_EQ("7f", Cpputils::ByteArr2HexStr({0x7F}));
    EXPECT_EQ("80", Cpputils::ByteArr2HexStr({0x80}));
}

TEST(ByteArr2HexStr, AllZeros) {
    std::vector<uint8_t> zeros(4, 0x00);
    EXPECT_EQ("00000000", Cpputils::ByteArr2HexStr(zeros));
}

TEST(ByteArr2HexStr, AllOnes) {
    std::vector<uint8_t> ones(4, 0xFF);
    EXPECT_EQ("ffffffff", Cpputils::ByteArr2HexStr(ones));
}

TEST(ByteArr2HexStr, OutputIsLowercase) {
    std::vector<uint8_t> arr {0xAB, 0xCD, 0xEF};
    auto str = Cpputils::ByteArr2HexStr(arr);
    EXPECT_EQ("abcdef", str);
    for (char c : str) {
        EXPECT_FALSE(std::isupper(c)) << "Expected lowercase but found: " << c;
    }
}

TEST(ByteArr2HexStr, OutputLengthIsAlwaysDouble) {
    for (size_t len = 1; len <= 32; ++len) {
        std::vector<uint8_t> arr(len, 0x42);
        EXPECT_EQ(len * 2, Cpputils::ByteArr2HexStr(arr).size());
    }
}

TEST(ByteArr2HexStr, NibbleBoundaryValues) {
    std::vector<uint8_t> arr {0x0F, 0xF0, 0x10, 0x01};
    EXPECT_EQ("0ff01001", Cpputils::ByteArr2HexStr(arr));
}
