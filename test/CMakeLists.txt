rock_gtest(test_suite suite.cpp
   test_SatcommsStatusManager.cpp
   DEPS satcomms_kvh)

set_tests_properties(test-test_suite-cxx PROPERTIES ENVIRONMENT
   "SRC_DIR=${CMAKE_CURRENT_SOURCE_DIR}")