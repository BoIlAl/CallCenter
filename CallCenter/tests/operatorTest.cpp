#include <gtest/gtest.h>

#include "operator.h"

TEST(operatorTest, rememberId) {
    opID_t id = 4;
    Operator op(id);

    EXPECT_EQ(id, op.getId());
}

TEST(operatorTest, fitsInTimeTriggerCallback) {
    opID_t id = 3;
    Operator op(id);

    time_point now = std::chrono::system_clock::now();
    time_point endPoint = std::chrono::system_clock::now() + std::chrono::milliseconds(300);

    CDR* cdr = new CDR;
    auto ptr = std::shared_ptr<CDR>(cdr);

    opID_t i = id;

    op.answer(ptr, [&i](opID_t id) {i += id;}, endPoint);

    EXPECT_EQ(i, id + id);
    EXPECT_NE(cdr->duration, duration_t(0));
    EXPECT_EQ(cdr->operatorID, id);
    EXPECT_NE(cdr->dateTimeAnswer, time_point::min());
    EXPECT_LT(now + std::chrono::milliseconds(300), std::chrono::system_clock::now());
    EXPECT_LT(std::chrono::system_clock::now(), now + std::chrono::milliseconds(400));
}