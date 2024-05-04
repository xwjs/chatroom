#include "../../include/message.h"
#include <cstring>
#include <gtest/gtest.h>


TEST(Text, TestName) {
    Message m("");

    EXPECT_EQ(m.get_type(), Message::Type::Null);

    char a[100] = " hello world "    ;
    m.set_text(a);

    EXPECT_EQ(m.get_text(), a);
    EXPECT_EQ(m.get_type(), Message::Type::Text);
    EXPECT_EQ(m.get_size(), strlen(a));
    EXPECT_EQ(m.get_binary(), std::vector<char>());

    char b[100] = "\"\"\\\r\t\n";
    m.set_text(b);

    EXPECT_EQ(m.get_text(), b);
    EXPECT_EQ(m.get_type(), Message::Type::Text);
    EXPECT_EQ(m.get_size(), 6);
    EXPECT_EQ(m.get_binary(), std::vector<char>());

    char c[100]="";
    m.set_text(c);

    EXPECT_EQ(m.get_text(), c);
    EXPECT_EQ(m.get_type(), Message::Type::Null);
    EXPECT_EQ(m.get_size(), 0);
    EXPECT_EQ(m.get_binary(), std::vector<char>());

}

TEST(Binary, test_name){
    std::vector<char> binary_data = {0x00, 0x01, 0x02, 0x03};
    Message a(binary_data);

    EXPECT_EQ(a.get_binary(),binary_data);
    EXPECT_EQ(a.get_size(),4);
    EXPECT_EQ(a.get_type(),Message::Type::Binary);
    EXPECT_EQ(a.get_text(),"");

    Message b{std::vector<char>()};
    EXPECT_EQ(b.get_binary(),std::vector<char>());
    EXPECT_EQ(b.get_size(),0);
    EXPECT_EQ(b.get_type(),Message::Type::Null);
    EXPECT_EQ(b.get_text(),"");


    b.set_text("123");
    EXPECT_EQ(b.get_text(), "123");
    EXPECT_EQ(b.get_binary(), std::vector<char>());
    EXPECT_EQ(b.get_type(), Message::Type::Text);
    EXPECT_EQ(b.get_size(), 3);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
