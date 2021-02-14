// Copyright 2020 LabradorWithShades
// 70892548+LabradorWithShades@users.noreply.github.com

#include <gtest/gtest.h>

#include <stack.hpp>

TEST(General, TestTraits) {
  EXPECT_TRUE(std::is_move_constructible_v<Stack<int>>);
  EXPECT_TRUE(std::is_move_assignable_v<Stack<int>>);
  EXPECT_FALSE(std::is_copy_constructible_v<Stack<int>>);
  EXPECT_FALSE(std::is_copy_assignable_v<Stack<int>>);
}

TEST(BaseType, PushAndPop) {
  Stack<int> stack{};
  stack.push(5);
  stack.push(3);
  EXPECT_EQ(stack.pop(), 3);
  EXPECT_EQ(stack.pop(), 5);
  EXPECT_THROW(stack.pop(), std::runtime_error);
}

static size_t c_count{0}, cc_count{0}, mc_count{0}, ca_count{0}, ma_count{0},
    d_count{0};

class ComplexClass {
 public:
  explicit ComplexClass(int val) : mp_field(new int(val)) { ++c_count; }
  ComplexClass(const ComplexClass& other) : mp_field(new int(*other.mp_field)) {
    ++cc_count;
  }
  ComplexClass(ComplexClass&& other) noexcept : mp_field(other.mp_field) {
    ++mc_count;
    other.mp_field = nullptr;
  }
  ComplexClass& operator=(const ComplexClass& other) {
    ++ca_count;
    if (this == &other) {
      return *this;
    }
    mp_field = new int(*other.mp_field);
    return *this;
  }
  ComplexClass& operator=(ComplexClass&& other) noexcept {
    ++ma_count;
    mp_field = other.mp_field;
    return *this;
  }
  ~ComplexClass() {
    ++d_count;
    delete mp_field;
  }

  bool operator==(const ComplexClass& other) const {
    return (*mp_field) == (*other.mp_field);
  }

 private:
  int* mp_field;
};

TEST(ComplexType, PushVariants) {
  c_count = 0;
  cc_count = 0;
  mc_count = 0;
  ca_count = 0;
  ma_count = 0;
  d_count = 0;
  {
    Stack<ComplexClass> stack;
    ComplexClass x{5};
    stack.push(x);
    stack.push(std::move(x));
    stack.push_emplace(4);
  }
  EXPECT_EQ(c_count, 2);
  EXPECT_EQ(cc_count, 1);
  EXPECT_EQ(mc_count, 1);
  EXPECT_EQ(ca_count, 0);
  EXPECT_EQ(ma_count, 0);
  EXPECT_EQ(d_count, 4);
}

TEST(ComplexType, IntegrityTest) {
  c_count = 0;
  cc_count = 0;
  mc_count = 0;
  ca_count = 0;
  ma_count = 0;
  d_count = 0;
  {
    Stack<ComplexClass> stack;
    stack.push_emplace(4);
    stack.push_emplace(7);
    stack.push_emplace(10);
    ComplexClass val1 = stack.pop();
    ComplexClass val2 = stack.pop();
    ComplexClass val3 = stack.pop();
    EXPECT_EQ(c_count, 3);
    EXPECT_EQ(cc_count, 0);
    EXPECT_EQ(mc_count, 3);
    EXPECT_EQ(ca_count, 0);
    EXPECT_EQ(ma_count, 0);
    EXPECT_EQ(d_count, 3);
    EXPECT_EQ(val1, ComplexClass(10));
    EXPECT_EQ(val2, ComplexClass(7));
    EXPECT_EQ(val3, ComplexClass(4));
    EXPECT_THROW(stack.pop(), std::runtime_error);
  }
}

TEST(ComplexType, DestructorCallTest) {
  c_count = 0;
  cc_count = 0;
  mc_count = 0;
  ca_count = 0;
  ma_count = 0;
  d_count = 0;
  {
    Stack<ComplexClass> stack;
    stack.push_emplace(4);
    stack.push_emplace(7);
    stack.push_emplace(10);
  }
  EXPECT_EQ(c_count, 3);
  EXPECT_EQ(cc_count, 0);
  EXPECT_EQ(mc_count, 0);
  EXPECT_EQ(ca_count, 0);
  EXPECT_EQ(ma_count, 0);
  EXPECT_EQ(d_count, 3);
}
