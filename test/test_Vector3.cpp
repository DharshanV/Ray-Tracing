#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include <sstream>
#include "Vector3.h"

using namespace testing;

class Vector3Tester : public ::testing::Test {
  protected:
  public:
    Vector3Tester() { }
};

TEST_F(Vector3Tester, TEST_INDEXING) {
  Vector3<int> vec3 = Vector3<int>(1,2,3);
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

TEST_F(Vector3Tester, TEST_PRINT) {
  Vector3<int> vec3 = Vector3<int>(1,2,3);
  std::stringstream stream;
  vec3.print(stream);
  ASSERT_EQ(stream.str(),"[1,2,3]");
}

TEST_F(Vector3Tester, TEST_NORMALIZE) {
  Vector3<float> vec3 = Vector3<float>(-1,2,3);
  float scale = vec3.normalize();
  ASSERT_FLOAT_EQ(scale,sqrtf32(1+4+9));
}

TEST_F(Vector3Tester, TEST_GET_NORMALIZED) {
  Vector3<float> vec3 = Vector3<float>(-1,2,3);
  Vector3<float> nVec3 = vec3.getNormalized();

  float scale = vec3.normalize();
  float trueValues[] = {-1.0f/scale,2.0f/scale,3.0f/scale};
  for(int i=0;i<3;i++){
    ASSERT_FLOAT_EQ(nVec3[i],trueValues[i]);
  }

  ASSERT_FLOAT_EQ(nVec3.length(),1);
}