// Do NOT add anything to this file
#include <curl/curl.h>
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    curl_global_init(CURL_GLOBAL_ALL);
    int result = RUN_ALL_TESTS();
    curl_global_cleanup();
    return result;
}
