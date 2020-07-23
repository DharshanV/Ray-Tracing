#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include "Vector3.h"

using namespace testing;

class Vector3Tester : public ::testing::Test {
  protected:
  Vector3<int> vec3;
  public:
    Vector3Tester() {
      vec3 = Vector3<int>(1,2,3);
    }
};

TEST_F(Vector3Tester, TEST_INDEXING) {
  ASSERT_EQ(vec3[0],1);
  ASSERT_EQ(vec3[1],2);
  ASSERT_EQ(vec3[2],3);

  vec3[0] = 4;
  vec3[1] = 5;
  vec3[2] = 6;

  ASSERT_EQ(vec3[0],4);
  ASSERT_EQ(vec3[1],5);
  ASSERT_EQ(vec3[2],6);
}