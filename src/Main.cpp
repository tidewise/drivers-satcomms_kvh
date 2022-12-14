#include <curl/curl.h>
#include <iostream>
#include <satcomms_kvh/SatcommsStatus.hpp>
#include <satcomms_kvh/SatcommsStatusManager.hpp>

using namespace std;
using namespace satcomms_kvh;

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Please provide Satcomms IP Address: %s <URL>\n", argv[0]);
        return 1;
    }

    curl_global_init(CURL_GLOBAL_ALL);

    SatcommsStatusManager satcomms_status_manager;

    satcomms_status_manager.setURL(argv[1]);
    satcomms_status_manager.setTimeout(base::Time::fromSeconds(10));
    SatcommsStatus result = satcomms_status_manager.getSatcommsStatus();

    cout << "timestamp: " << result.timestamp << endl;
    cout << "online_offline_state: " << result.online_offline_state << endl;
    cout << "flrx_snr: " << result.flrx_snr << endl;
    cout << "antenna_status_azimuth: " << result.antenna_status_azimuth << endl;
    cout << "antenna_status_elevation: " << result.antenna_status_elevation << endl;
    cout << "antenna_state: " << result.antenna_state << endl;
    cout << "satellite: " << result.satellite << endl;
    cout << "beam: " << result.beam << endl;
    cout << "frequency: " << result.frequency << endl;
    cout << "modem_state: " << result.modem_state << endl;
    cout << "last_login: " << result.last_login << endl;
    cout << "rx_stats_fl_state: " << result.rx_stats_fl_state << endl;
    cout << "rx_stats_fl_carrier: " << result.rx_stats_fl_carrier << endl;
    cout << "tx_stats_rl_state: " << result.tx_stats_rl_state << endl;
    cout << "tx_stats_rl_carrier: " << result.tx_stats_rl_carrier << endl;
    cout << "tx_stats_rl_power: " << result.tx_stats_rl_power << endl;

    curl_global_cleanup();
    return 0;
}
