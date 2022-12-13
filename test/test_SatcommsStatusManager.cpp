#include <boost/regex.hpp>
#include <fstream>
#include <gtest/gtest.h>
#include <satcomms_kvh/SatcommsStatusManager.hpp>

using namespace satcomms_kvh;

struct SatcommsStatusManagerTest : public ::testing::Test {
};

TEST_F(SatcommsStatusManagerTest, it_should_not_crash_when_set_url_is_called)
{
    satcomms_kvh::SatcommsStatusManager satcommsstatusmanager;
    satcommsstatusmanager.setURL("https://www.google.com/");

    ASSERT_STREQ(satcommsstatusmanager.url_link.c_str(), "https://www.google.com/");
}

TEST_F(SatcommsStatusManagerTest, it_should_not_crash_when_set_timeout_is_called)
{
    satcomms_kvh::SatcommsStatusManager satcommsstatusmanager;
    satcommsstatusmanager.setTimeout(300);

    ASSERT_EQ(satcommsstatusmanager.timeout_miliseconds, 300);
}

TEST_F(SatcommsStatusManagerTest, it_should_not_crash_when_get_url_is_called)
{
    satcomms_kvh::SatcommsStatusManager satcommsstatusmanager;

    satcommsstatusmanager.setURL("https://www.google.com/");
    satcommsstatusmanager.setTimeout(1000);
    satcommsstatusmanager.getURLData();
    boost::smatch matches;
    regex_search(satcommsstatusmanager.url_data, matches, boost::regex("(google)"));
    std::string result = matches[1];
    ASSERT_STREQ(result.c_str(), "google");
}

TEST_F(SatcommsStatusManagerTest, it_should_return_an_empty_string_when_timeout_is_called)
{
    satcomms_kvh::SatcommsStatusManager satcommsstatusmanager;

    satcommsstatusmanager.setURL("https://www.google.com/");
    satcommsstatusmanager.setTimeout(1);
    satcommsstatusmanager.getURLData();
    boost::smatch matches;
    regex_search(satcommsstatusmanager.url_data, matches, boost::regex("(google)"));
    std::string result = matches[1];
    ASSERT_STREQ(result.c_str(), "");
}

TEST_F(SatcommsStatusManagerTest, it_should_return_a_map_with_results)
{
    satcomms_kvh::SatcommsStatusManager satcommsstatusmanager;
    std::string path = std::getenv("SRC_DIR");
    path += "/satcomms.html";
    std::ifstream file(path);

    if (file.is_open()) { // checking whether the file is open
        std::stringstream strStream;
        strStream << file.rdbuf(); // read the file
        satcommsstatusmanager.url_data = strStream.str();
    }
    else {
        std::cout << "File not found" << std::endl;
    }
    file.close();

    const std::vector<std::string> features = {
        "online_offline_state",
        "flrx_snr",
        "antenna_status_az",
        "antenna_status_el",
        "antenna_state",
        "satelliteOrb",
        "beamInfo",
        "satellite_dlfreq",
        "modem_state",
        "login_last",
        "flrx_state",
        "flrx_carrier",
        "rltx_state",
        "rltx_carrier",
        "rltx_power",
    };

    std::map<std::string, std::string> result =
        satcommsstatusmanager.processText(features);
    ASSERT_EQ(result.size(), 15);
}

TEST_F(SatcommsStatusManagerTest, it_should_return_a_SatcommsStatusManagerStruct)
{
    satcomms_kvh::SatcommsStatusManager satcommsstatusmanager;
    std::string path = std::getenv("SRC_DIR");
    path += "/satcomms.html";
    std::ifstream file(path);

    if (file.is_open()) {
        std::stringstream strStream;
        strStream << file.rdbuf();
        satcommsstatusmanager.url_data = strStream.str();
    }
    else {
        std::cout << "File not found" << std::endl;
    }
    file.close();

    SatcommsStatus result = satcommsstatusmanager.getSatcommsStatus();

    SatcommsStatus expected;
    expected.online_offline_state = "UNKNOWN";
    expected.flrx_snr = -999.99;
    expected.antenna_status_azimuth = 0;
    expected.antenna_status_elevation = 0;
    expected.antenna_state = "INITIALIZING";
    expected.satellite = "0";
    expected.beam = "UNKNOWN-BAND: UNKNOWN";
    expected.frequency = 0;
    expected.modem_state = "WAITING_FOR_RX_LOCK";
    expected.last_login = "UNKNOWN";
    expected.rx_stats_fl_state = "UNKNOWN";
    expected.rx_stats_fl_carrier = "UNKNOWN";
    expected.tx_stats_rl_state = "UNKNOWN";
    expected.tx_stats_rl_carrier = "UNKNOWN";
    expected.tx_stats_rl_power = 0;

    ASSERT_EQ(result.online_offline_state, expected.online_offline_state);
}
