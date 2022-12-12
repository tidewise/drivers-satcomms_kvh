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

TEST_F(SatcommsStatusManagerTest, it_should_not_crash_when_get_url_is_called)
{
    satcomms_kvh::SatcommsStatusManager satcommsstatusmanager;

    satcommsstatusmanager.setURL("https://www.google.com/");
    satcommsstatusmanager.getURLData();

    boost::smatch matches;
    regex_search(satcommsstatusmanager.url_data, matches, boost::regex("google"));
    ASSERT_GT(matches.size(), 0);
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

    std::map<std::string, std::string> result = satcommsstatusmanager.processText(features);
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
    expected.flrx_snr = "-999.99dB";
    expected.antenna_status_az = "&deg;";
    expected.antenna_status_el = "&deg;";
    expected.antenna_state = "INITIALIZING";
    expected.satelliteOrb = "0";
    expected.beamInfo = "UNKNOWN-BAND: UNKNOWN";
    expected.satellite_dlfreq = "UNKNOWN";
    expected.modem_state = "WAITING_FOR_RX_LOCK";
    expected.login_last = "UNKNOWN";
    expected.flrx_state = "UNKNOWN";
    expected.flrx_carrier = "UNKNOWN";
    expected.rltx_state = "UNKNOWN";
    expected.rltx_carrier = "UNKNOWN";
    expected.rltx_power = "UNKNOWN";

    ASSERT_EQ(result.online_offline_state, expected.online_offline_state);
}
