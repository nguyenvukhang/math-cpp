#include <gtest/gtest.h>

#include "label.h"
#include "str.h"

TEST(cpp_properties, string_slicing) {
  std::string base("This is a cpp std library string.");
  std::string_view sub = std::string_view(base).substr(10, 19);
  EXPECT_EQ(sub, std::string_view("cpp std library str"));
  base.data()[10] = 'x';
  EXPECT_EQ(sub, std::string_view("xpp std library str"));
}

// Demonstrate some basic assertions.
TEST(minimath, label_to_tex) {
  Label label("f12_43d");
  EXPECT_STREQ("\\label{f12_43d}", label.to_tex().c_str());
}

TEST(str, split_string) {
  auto y = str("hello world").split(' ');
  EXPECT_TRUE(y);
  EXPECT_EQ(y->v1, "hello");
  EXPECT_EQ(y->v2, "world");
}

TEST(str, split_by_mark) {
  str buf = R"(Proposition{Projection on a closed convex set})";
  auto y = buf.split_by_mark();
  EXPECT_TRUE(y);
  EXPECT_EQ(y->v1, "Proposition");
  EXPECT_EQ(y->v2, "{Projection on a closed convex set}");

  buf = R"(Yayayayayayaya hello)";
  EXPECT_FALSE(buf.split_by_mark());
}

TEST(str, split_by_label) {
  str buf = R"({Riemann Hypothesis}\label{xxxxxxx})";
  auto y = buf.split_by_label();
  EXPECT_TRUE(y);
  EXPECT_EQ(y->v1, "{Riemann Hypothesis}");
  EXPECT_EQ(y->v2, "xxxxxxx");
}

TEST(str, parse_mark) {
  str buf = R"(\Proposition{Riemann Hypothesis}\label{xxxxxxx})";
  auto y = buf.parse_mark();
  EXPECT_TRUE(y);
  EXPECT_EQ(y->v1, "Proposition");
  EXPECT_EQ(y->v2, "Riemann Hypothesis");
  EXPECT_EQ(y->v3, "xxxxxxx");

  buf = R"(\Proposition[Riemann Hypothesis]\label{xxxxxxx})";
  EXPECT_FALSE(buf.parse_mark());

  buf = R"(\Proposition{Riemann Hypothesis})";
  EXPECT_FALSE(buf.parse_mark());

  buf = R"(\Proposition{}\label{xxxxxxx})";
  EXPECT_TRUE(y = buf.parse_mark());
  EXPECT_EQ(y->v2, "");
}

TEST(str, parse_subsection) {
  str buf = R"(\subsection{Dank Memes}\label{xxxxxxx})";
  auto y = buf.parse_subsection();
  EXPECT_TRUE(y);
  EXPECT_EQ(y->v1, "Dank Memes");
  EXPECT_EQ(y->v2, "xxxxxxx");
}
