#ifndef _SATCOMMSSTATUSPROJECT_SATCOMMS_STATUS_HPP
#define _SATCOMMSSTATUSPROJECT_SATCOMMS_STATUS_HPP

#include <string>

namespace satcomms_kvh {
    struct SatcommsStatus {
        std::string online_offline_state = "";
        std::string flrx_snr = "";
        std::string antenna_status_az = "";
        std::string antenna_status_el = "";
        std::string antenna_state = "";
        std::string satelliteOrb = "";
        std::string beamInfo = "";
        std::string satellite_dlfreq = "";
        std::string modem_state = "";
        std::string login_last = "";
        std::string flrx_state = "";
        std::string flrx_carrier = "";
        std::string rltx_state = "";
        std::string rltx_carrier = "";
        std::string rltx_power = "";
    };
}

#endif
