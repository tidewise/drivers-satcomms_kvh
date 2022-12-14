#include <base/Float.hpp>
#include <base/Time.hpp>
#include <boost/regex.hpp>
#include <fstream>
#include <gtest/gtest.h>
#include <satcomms_kvh/SatcommsStatusManager.hpp>

using namespace satcomms_kvh;

struct SatcommsStatusManagerTest : public ::testing::Test {
};

TEST_F(SatcommsStatusManagerTest, it_should_not_crash_when_get_url_is_called)
{
    satcomms_kvh::SatcommsStatusManager satcomms_status_manager;

    satcomms_status_manager.setURL("https://www.google.com/");
    satcomms_status_manager.setTimeout(base::Time::fromMilliseconds(500));
    bool status = satcomms_status_manager.getURLData();
    boost::smatch matches;
    regex_search(satcomms_status_manager.url_data, matches, boost::regex("(google)"));
    std::string result = matches[1];
    ASSERT_STREQ(result.c_str(), "google");
}

TEST_F(SatcommsStatusManagerTest, it_should_return_an_empty_string_when_timeout_is_called)
{
    satcomms_kvh::SatcommsStatusManager satcomms_status_manager;

    satcomms_status_manager.setURL("https://www.google.com/");
    satcomms_status_manager.setTimeout(base::Time::fromMilliseconds(3));
    base::Time timestamp_before = base::Time::now();
    bool status = satcomms_status_manager.getURLData();
    base::Time timestamp_after = base::Time::now();

    ASSERT_FALSE(status);
    ASSERT_GE(timestamp_after.toMilliseconds() - timestamp_before.toMilliseconds(), 3);
}

TEST_F(SatcommsStatusManagerTest, it_should_return_a_SatcommsStatusManagerStruct)
{
    satcomms_kvh::SatcommsStatusManager satcomms_status_manager;
    std::string path = std::getenv("SRC_DIR");
    path += "/satcomms.html";
    std::ifstream file(path);

    if (file.is_open()) {
        std::stringstream strStream;
        strStream << file.rdbuf();
        satcomms_status_manager.url_data = strStream.str();
    }
    else {
        std::cout << "File not found" << std::endl;
    }
    file.close();

    base::Time timestamp_before = base::Time::now();
    SatcommsStatus result = satcomms_status_manager.parseSatcommsStatus();
    base::Time timestamp_after = base::Time::now();

    ASSERT_GE(result.timestamp.toMilliseconds(), timestamp_before.toMilliseconds());
    ASSERT_LE(result.timestamp.toMilliseconds(), timestamp_after.toMilliseconds());
    ASSERT_EQ(result.online_offline_state, "UNKNOWN");
    ASSERT_EQ(result.flrx_snr, static_cast<float>(-999.99));
    ASSERT_TRUE(std::isnan(result.antenna_status_azimuth));
    ASSERT_TRUE(std::isnan(result.antenna_status_elevation));
    ASSERT_EQ(result.antenna_state, "INITIALIZING");
    ASSERT_EQ(result.satellite, "0");
    ASSERT_EQ(result.beam, "UNKNOWN-BAND: UNKNOWN");
    ASSERT_TRUE(std::isnan(result.frequency));
    ASSERT_EQ(result.modem_state, "WAITING_FOR_RX_LOCK");
    ASSERT_EQ(result.last_login, "UNKNOWN");
    ASSERT_EQ(result.rx_stats_fl_state, "UNKNOWN");
    ASSERT_EQ(result.rx_stats_fl_carrier, "UNKNOWN");
    ASSERT_EQ(result.tx_stats_rl_state, "UNKNOWN");
    ASSERT_EQ(result.tx_stats_rl_carrier, "UNKNOWN");
    ASSERT_TRUE(std::isnan(result.tx_stats_rl_power));
}
