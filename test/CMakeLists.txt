add_executable (test_Vector3 test_Vector3.cpp)
target_link_libraries(test_Vector3 PRIVATE gtest_main)
add_test(test_Vector3 test_Vector3)