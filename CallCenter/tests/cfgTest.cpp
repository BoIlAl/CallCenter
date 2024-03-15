#include <gtest/gtest.h>
#include <sstream>
#include "CDRHandlerCfg.h"
#include "operatorManagerCfg.h"

json handlerCfgJsonFromValues(std::string rMin, std::string rMax, std::string isUpdate, std::string queueLength) {
    std::stringstream ss;

    ss << "{\"RQueueMin\": " << rMin << ", \"RQueueMax\": " << rMax << ", \"IsUpdateDublicates\":"
       << isUpdate << ", \"QueueLength\":  " << queueLength << "}";

    return json::parse(ss);
}

json omCfgJsonFromValues(std::string rMin, std::string rMax) {
    std::stringstream ss;

    ss << "{\"ROperatorMin\": " << rMin << ", \"ROperatorMax\": " << rMax << "}";

    return json::parse(ss);
}

TEST(cfgTests, getNamesIsValid) {
    CDRHandlerCfg handlerCfg;
    OperatorManagerCfg omCfg;

    EXPECT_FALSE(handlerCfg.getNames().empty());
    EXPECT_FALSE(omCfg.getNames().empty());
}

TEST(cfgTests, handlerCFGSetReturnTrueIfValuesCorrect) {
    CDRHandlerCfg handlerCfg;

    std::string rMax = "3600000";
    std::string rMin = "1000";
    std::string rNormal = "30000";
    std::string sTrue = "true";
    std::string sFalse = "false";
    std::string nMax = "1000";
    std::string nNormal = "100";
    std::string nMin = "1";

    EXPECT_TRUE(handlerCfg.set(handlerCfgJsonFromValues(rMin, rNormal, sTrue, nNormal)));
    EXPECT_TRUE(handlerCfg.set(handlerCfgJsonFromValues(rNormal, rMax, sTrue, nNormal)));
    EXPECT_TRUE(handlerCfg.set(handlerCfgJsonFromValues(rMin, rMin, sTrue, nNormal)));
    EXPECT_TRUE(handlerCfg.set(handlerCfgJsonFromValues(rMax, rMax, sTrue, nNormal)));

    EXPECT_TRUE(handlerCfg.set(handlerCfgJsonFromValues(rNormal, rMax, sTrue, nNormal)));
    EXPECT_TRUE(handlerCfg.set(handlerCfgJsonFromValues(rNormal, rMax, sFalse, nNormal)));
    EXPECT_TRUE(handlerCfg.set(handlerCfgJsonFromValues(rNormal, rMax, sFalse, nMin)));
    EXPECT_TRUE(handlerCfg.set(handlerCfgJsonFromValues(rNormal, rMax, sFalse, nMax)));
}

TEST(cfgTests, omCFGsetReturnTrueIfValuesCorrect) {
    OperatorManagerCfg omCfg;

    std::string rMax = "3600000";
    std::string rMin = "1000";
    std::string rNormal = "30000";

    EXPECT_TRUE(omCfg.set(omCfgJsonFromValues(rMin, rNormal)));
    EXPECT_TRUE(omCfg.set(omCfgJsonFromValues(rNormal, rMax)));
    EXPECT_TRUE(omCfg.set(omCfgJsonFromValues(rMin, rMin)));
    EXPECT_TRUE(omCfg.set(omCfgJsonFromValues(rMax, rMax)));
}

TEST(cfgTests, omCFGsetReturnFalseIfRMinIncorrect) {
    OperatorManagerCfg omCfg;

    std::string rMore = "3600001";
    std::string rLess = "999";
    std::string rMax = "3600000";
    std::string rNegative = "-1";
    std::string rAnotherType = "true";

    EXPECT_FALSE(omCfg.set(omCfgJsonFromValues(rMore, rMax)));
    EXPECT_FALSE(omCfg.set(omCfgJsonFromValues(rLess, rMax)));
    EXPECT_FALSE(omCfg.set(omCfgJsonFromValues(rNegative, rMax)));
    EXPECT_FALSE(omCfg.set(omCfgJsonFromValues(rAnotherType, rMax)));
}

TEST(cfgTests, omCFGsetReturnFalseIfRMaxIncorrect) {
    OperatorManagerCfg omCfg;

    std::string rMore = "3600001";
    std::string rLess = "999";
    std::string rMin = "1000";
    std::string rNegative = "-1";
    std::string rAnotherType = "true";

    EXPECT_FALSE(omCfg.set(omCfgJsonFromValues(rMin, rMore)));
    EXPECT_FALSE(omCfg.set(omCfgJsonFromValues(rMin, rLess)));
    EXPECT_FALSE(omCfg.set(omCfgJsonFromValues(rMin, rNegative)));
    EXPECT_FALSE(omCfg.set(omCfgJsonFromValues(rMin, rAnotherType)));
}

TEST(cfgTests, omCFGsetReturnFalseIfRminMoreThenRMaxIncorrect) {
    OperatorManagerCfg omCfg;

    std::string rMax = "3600000";
    std::string rMin = "1000";

    EXPECT_FALSE(omCfg.set(omCfgJsonFromValues(rMax, rMin)));
}

TEST(cfgTests, handlerCFGsetReturnFalseIfRMinIncorrect) {
    CDRHandlerCfg handlerCfg;

    std::string rMore = "3600001";
    std::string rLess = "999";
    std::string rMax = "3600000";
    std::string rNegative = "-1";
    std::string sTrue = "true";
    std::string nMax = "1000";
    std::string rAnotherType = "true";

    EXPECT_FALSE(handlerCfg.set(handlerCfgJsonFromValues(rMore, rMax, sTrue, nMax)));
    EXPECT_FALSE(handlerCfg.set(handlerCfgJsonFromValues(rLess, rMax, sTrue, nMax)));
    EXPECT_FALSE(handlerCfg.set(handlerCfgJsonFromValues(rNegative, rMax, sTrue, nMax)));
    EXPECT_FALSE(handlerCfg.set(handlerCfgJsonFromValues(rAnotherType, rMax, sTrue, nMax)));

}

TEST(cfgTests, handlerCFGsetReturnFalseIfRMaxIncorrect) {
    CDRHandlerCfg handlerCfg;

    std::string rMore = "3600001";
    std::string rLess = "999";
    std::string rMin = "1000";
    std::string rNegative = "-1";
    std::string sTrue = "true";
    std::string nMax = "1000";
    std::string rAnotherType = "true";

    EXPECT_FALSE(handlerCfg.set(handlerCfgJsonFromValues(rMin, rMore, sTrue, nMax)));
    EXPECT_FALSE(handlerCfg.set(handlerCfgJsonFromValues(rMin, rLess, sTrue, nMax)));
    EXPECT_FALSE(handlerCfg.set(handlerCfgJsonFromValues(rMin, rNegative, sTrue, nMax)));
    EXPECT_FALSE(handlerCfg.set(handlerCfgJsonFromValues(rMin, rAnotherType, sTrue, nMax)));
}

TEST(cfgTests, handlerCFGsetReturnFalseIfIsUpdateIncorrect) {
    CDRHandlerCfg handlerCfg;

    std::string rMin = "1000";
    std::string rMax = "3600000";
    std::string nMax = "1000";

    EXPECT_FALSE(handlerCfg.set(handlerCfgJsonFromValues(rMin, rMax, nMax, nMax)));
}

TEST(cfgTests, handlerCFGsetReturnFalseIfNIncorrect) {
    CDRHandlerCfg handlerCfg;

    std::string rMin = "1000";
    std::string rMax = "3600000";
    std::string sTrue = "true";
    std::string nLess = "0";
    std::string nMore = "1001";
    std::string nNegative = "-1";
    std::string nAnotherType = "true";

    EXPECT_FALSE(handlerCfg.set(handlerCfgJsonFromValues(rMin, rMax, sTrue, nLess)));
    EXPECT_FALSE(handlerCfg.set(handlerCfgJsonFromValues(rMin, rMax, sTrue, nMore)));
    EXPECT_FALSE(handlerCfg.set(handlerCfgJsonFromValues(rMin, rMax, sTrue, nNegative)));
    EXPECT_FALSE(handlerCfg.set(handlerCfgJsonFromValues(rMin, rMax, sTrue, nAnotherType)));

}

TEST(cfgTests, handlerCFGsetReturnFalseIfRminMoreThenRMaxIncorrect) {
    CDRHandlerCfg handlerCfg;

    std::string rMax = "3600000";
    std::string rMin = "1000";
    std::string sTrue = "true";
    std::string nMax = "1000";

    EXPECT_FALSE(handlerCfg.set(handlerCfgJsonFromValues(rMax, rMin, sTrue, nMax)));
}

TEST(cfgTests, handlerCFGgetIndeciesReturnEmptyIfCorrect) {
    CDRHandlerCfg handlerCfg;

    std::string rMax = "3600000";
    std::string rMin = "1000";
    std::string sTrue = "true";
    std::string nMax = "1000";

    handlerCfg.set(handlerCfgJsonFromValues(rMin, rMax, sTrue, nMax));

    auto res = handlerCfg.getIndecesOfDefaultValues();

    EXPECT_TRUE(res.empty());
}

TEST(cfgTests, omCFGgetIndeciesReturnEmptyIfCorrect) {
    OperatorManagerCfg omCfg;

    std::string rMax = "3600000";
    std::string rMin = "1000";

    omCfg.set(omCfgJsonFromValues(rMin, rMax));

    auto res = omCfg.getIndecesOfDefaultValues();

    EXPECT_TRUE(res.empty());
}

TEST(cfgTests, handlerCFGgetIndeciesReturnOneIfIncorrectOne) {
    CDRHandlerCfg handlerCfg;

    std::string rMax = "3600000";
    std::string rMore = "3600001";

    std::string rMin = "1000";
    std::string rLess = "999";

    std::string sTrue = "true";

    std::string nMax = "1000";
    std::string nLess = "0";

    handlerCfg.set(handlerCfgJsonFromValues(rMore, rMax, sTrue, nMax));
    auto res1 = handlerCfg.getIndecesOfDefaultValues();

    handlerCfg.set(handlerCfgJsonFromValues(rMin, rLess, sTrue, nMax));
    auto res2 = handlerCfg.getIndecesOfDefaultValues();

    handlerCfg.set(handlerCfgJsonFromValues(rMin, rMax, nMax, nMax));
    auto res3 = handlerCfg.getIndecesOfDefaultValues();

    handlerCfg.set(handlerCfgJsonFromValues(rMin, rMax, sTrue, nLess));
    auto res4 = handlerCfg.getIndecesOfDefaultValues();


    EXPECT_EQ(res1.size(), 1);
    EXPECT_EQ(res2.size(), 1);
    EXPECT_EQ(res2.size(), 1);
    EXPECT_EQ(res2.size(), 1);
}

TEST(cfgTests, omCFGgetIndeciesReturnOneIfIncorrectOne) {
    OperatorManagerCfg omCfg;

    std::string rMax = "3600000";
    std::string rMore = "3600001";

    std::string rMin = "1000";
    std::string rLess = "999";

    omCfg.set(omCfgJsonFromValues(rMore, rMax));
    auto res1 = omCfg.getIndecesOfDefaultValues();

    omCfg.set(omCfgJsonFromValues(rMin, rLess));
    auto res2 = omCfg.getIndecesOfDefaultValues();

    EXPECT_EQ(res1.size(), 1);
    EXPECT_EQ(res2.size(), 1);
}

TEST(cfgTests, handlerCFGgetIndeciesReturnFourIfIncorrectFour) {
    CDRHandlerCfg handlerCfg;

    std::string rMore = "3600001";

    std::string rLess = "999";

    std::string nLess = "0";

    handlerCfg.set(handlerCfgJsonFromValues(rMore, rLess, nLess, nLess));
    auto res = handlerCfg.getIndecesOfDefaultValues();

    EXPECT_EQ(res.size(), 4);
}

TEST(cfgTests, omCFGgetIndeciesReturnTwoIfIncorrectTwo) {
    OperatorManagerCfg omCfg;

    std::string rMore = "3600001";

    std::string rLess = "999";

    omCfg.set(omCfgJsonFromValues(rMore, rMore));
    auto res = omCfg.getIndecesOfDefaultValues();

    EXPECT_EQ(res.size(), 2);
}