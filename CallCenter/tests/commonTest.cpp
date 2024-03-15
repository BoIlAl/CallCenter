#include <gtest/gtest.h>

#include "common.h"

TEST(commonTest, isUIntReturnTrueOnUInt) {
    std::string normal = "1000";
    std::string zero = "0";
    std::string allNums = "1234567890";
    std::string big = "111111111111111111111111111111111111111111111111111111111111111111111111111";

    EXPECT_TRUE(isUInt(normal));
    EXPECT_TRUE(isUInt(zero));
    EXPECT_TRUE(isUInt(allNums));
    EXPECT_TRUE(isUInt(big));
}

TEST(commonTest, isUIntReturnFalseOnNotUInt) {
    std::string negative = "-1";
    std::string spaces = " 1 2";
    std::string symbols = "\n&%qrd\0";

    EXPECT_FALSE(isUInt(negative));
    EXPECT_FALSE(isUInt(spaces));
    EXPECT_FALSE(isUInt(symbols));
}

TEST(commonTest, tryParseBoolReturnTrueAndFillCorrectlyOnTrue) {
    std::string sTrue = "true";

    bool f = false;
    
    EXPECT_TRUE(tryParseBool(sTrue, f));
    EXPECT_TRUE(f);
}

TEST(commonTest, tryParseBoolReturnTrueAndFillCorrectlyOnFalse) {
    std::string sFalse = "false";

    bool t = true;
    
    EXPECT_TRUE(tryParseBool(sFalse, t));
    EXPECT_FALSE(t);
}

TEST(commonTest, tryParseBoolReturnFalseAndNotChangeOnCase) {
    std::string incTrue = "tRue";

    bool t = true;
    bool f = false;
    
    EXPECT_FALSE(tryParseBool(incTrue, t));
    EXPECT_TRUE(t);
    EXPECT_FALSE(tryParseBool(incTrue, f));
    EXPECT_FALSE(f);
}

TEST(commonTest, tryParseBoolReturnFalseOnCase) {
    std::string incFalse = "False";

    bool t;
    
    EXPECT_FALSE(tryParseBool(incFalse, t));
}

TEST(commonTest, tryParseBoolReturnFalseAndNotChangeOnSymbols) {
    std::string symbols = "\n\0&jhj";

    bool t = true;
    bool f = false;

    EXPECT_FALSE(tryParseBool(symbols, t));
    EXPECT_TRUE(t);
    EXPECT_FALSE(tryParseBool(symbols, f));
    EXPECT_FALSE(f);
}

TEST(commonTest, tryParseBoolReturnFalseAndNotChangeOnSpaces) {
    std::string spaces = "  true";

    bool t = true;
    bool f = false;
    
    EXPECT_FALSE(tryParseBool(spaces, t));
    EXPECT_TRUE(t);
    EXPECT_FALSE(tryParseBool(spaces, f));
    EXPECT_FALSE(f);
}

TEST(commonTest, tryParseUInt16ReturnTrueAndFillCorrectlyOn1000) {
    std::string s1000 = "1000";

    uint16_t i;
    
    EXPECT_TRUE(tryParseUInt(s1000, i));
    EXPECT_EQ(i, 1000);
}

TEST(commonTest, tryParseUInt16ReturnTrueAndFillCorrectlyOn0) {
    std::string s0 = "0";

    uint16_t i;
    
    EXPECT_TRUE(tryParseUInt(s0, i));
    EXPECT_EQ(i, 0);
}

TEST(commonTest, tryParseUInt16ReturnTrueAndFillCorrectlyOnMax) {
    std::string s65535 = "65535";

    uint16_t i;
    
    EXPECT_TRUE(tryParseUInt(s65535, i));
    EXPECT_EQ(i, 65535);
}

TEST(commonTest, tryParseUInt32ReturnTrueAndFillCorrectlyOn1000) {
    std::string s1000 = "1000";

    uint32_t i;
    
    EXPECT_TRUE(tryParseUInt(s1000, i));
    EXPECT_EQ(i, 1000);
}

TEST(commonTest, tryParseUInt32ReturnTrueAndFillCorrectlyOn0) {
    std::string s0 = "0";

    uint32_t i;
    
    EXPECT_TRUE(tryParseUInt(s0, i));
    EXPECT_EQ(i, 0);
}

TEST(commonTest, tryParseUInt32ReturnTrueAndFillCorrectlyOnMax) {
    std::string s4294967295 = "4294967295";

    uint32_t i;
    
    EXPECT_TRUE(tryParseUInt(s4294967295, i));
    EXPECT_EQ(i, 4294967295);
}

TEST(commonTest, tryParseUInt64ReturnTrueAndFillCorrectlyOn1000) {
    std::string s1000 = "1000";

    uint64_t i;
    
    EXPECT_TRUE(tryParseUInt(s1000, i));
    EXPECT_EQ(i, 1000);
}

TEST(commonTest, tryParseUInt64ReturnTrueAndFillCorrectlyOn0) {
    std::string s0 = "0";

    uint64_t i;

    EXPECT_TRUE(tryParseUInt(s0, i));
    EXPECT_EQ(i, 0);
}

TEST(commonTest, tryParseUInt64ReturnTrueAndFillCorrectlyOnMax) {
    std::string s18446744073709551615 = "18446744073709551615";

    uint64_t i;
    
    EXPECT_TRUE(tryParseUInt(s18446744073709551615, i));
    EXPECT_EQ(i, 18446744073709551615ull);
}

TEST(commonTest, tryParseUInt16ReturnFalseAndNotChangeOnNegative) {
    std::string negative = "-1";

    uint16_t i = 2;
    
    EXPECT_FALSE(tryParseUInt(negative, i));
    EXPECT_EQ(i, 2);
}

TEST(commonTest, tryParseUInt16ReturnFalseAndNotChangeOnSpaces) {
    std::string spaces = " 1";

    uint16_t i = 2;
    
    EXPECT_FALSE(tryParseUInt(spaces, i));
    EXPECT_EQ(i, 2);
}

TEST(commonTest, tryParseUInt16ReturnFalseAndNotChangeOnSymbols) {
    std::string symbols = "\n\0123sf";

    uint16_t i = 2;
    
    EXPECT_FALSE(tryParseUInt(symbols, i));
    EXPECT_EQ(i, 2);
}

TEST(commonTest, tryParseUInt16ReturnFalseAndNotChangeOnOverflow) {
    std::string overflow = "65536";

    uint16_t i = 2;
    
    EXPECT_FALSE(tryParseUInt(overflow, i));
    EXPECT_EQ(i, 2);
}

TEST(commonTest, tryParseUInt32ReturnFalseAndNotChangeOnNegative) {
    std::string negative = "-1";

    uint32_t i = 2;
    
    EXPECT_FALSE(tryParseUInt(negative, i));
    EXPECT_EQ(i, 2);
}

TEST(commonTest, tryParseUInt32ReturnFalseAndNotChangeOnSpaces) {
    std::string spaces = " 1";

    uint32_t i = 2;
    
    EXPECT_FALSE(tryParseUInt(spaces, i));
    EXPECT_EQ(i, 2);
}

TEST(commonTest, tryParseUInt32ReturnFalseAndNotChangeOnSymbols) {
    std::string symbols = "\n\0123sf";

    uint32_t i = 2;
    
    EXPECT_FALSE(tryParseUInt(symbols, i));
    EXPECT_EQ(i, 2);
}

TEST(commonTest, tryParseUInt32ReturnFalseAndNotChangeOnOverflow) {
    std::string overflow = "4294967296";

    uint32_t i = 2;
    
    EXPECT_FALSE(tryParseUInt(overflow, i));
    EXPECT_EQ(i, 2);
}

TEST(commonTest, tryParseUInt64ReturnFalseAndNotChangeOnNegative) {
    std::string negative = "-1";

    uint64_t i = 2;
    
    EXPECT_FALSE(tryParseUInt(negative, i));
    EXPECT_EQ(i, 2);
}

TEST(commonTest, tryParseUInt64ReturnFalseAndNotChangeOnSpaces) {
    std::string spaces = " 1";

    uint64_t i = 2;
    
    EXPECT_FALSE(tryParseUInt(spaces, i));
    EXPECT_EQ(i, 2);
}

TEST(commonTest, tryParseUInt64ReturnFalseAndNotChangeOnSymbols) {
    std::string symbols = "\n\0123sf";

    uint64_t i = 2;
    
    EXPECT_FALSE(tryParseUInt(symbols, i));
    EXPECT_EQ(i, 2);
}

TEST(commonTest, tryParseUInt64ReturnFalseAndNotChangeOnOverflow) {
    std::string overflow = "18446744073709551616";

    uint64_t i = 2;
    
    EXPECT_FALSE(tryParseUInt(overflow, i));
    EXPECT_EQ(i, 2);
}
