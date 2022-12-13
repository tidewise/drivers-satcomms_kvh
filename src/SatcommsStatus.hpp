#ifndef _SATCOMMSSTATUSPROJECT_SATCOMMS_STATUS_HPP
#define _SATCOMMSSTATUSPROJECT_SATCOMMS_STATUS_HPP

#include <string>

namespace satcomms_kvh {
    struct SatcommsStatus {
        std::string online_offline_state = "";
        float flrx_snr = 0;
        float antenna_status_azimuth = 0;
        float antenna_status_elevation = 0;
        std::string antenna_state = "";
        std::string satellite = "";
        std::string beam = "";
        float frequency = 0;
        std::string modem_state = "";
        std::string last_login = "";
        std::string rx_stats_fl_state = "";
        std::string rx_stats_fl_carrier = "";
        std::string tx_stats_rl_state = "";
        std::string tx_stats_rl_carrier = "";
        float tx_stats_rl_power = 0;
    };
}

#endif
