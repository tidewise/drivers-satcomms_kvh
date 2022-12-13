#ifndef _SATCOMMS_STATUS_PROJECT_SATCOMMS_STATUS_MANAGER_HPP_
#define _SATCOMMS_STATUS_PROJECT_SATCOMMS_STATUS_MANAGER_HPP_

#include "SatcommsStatus.hpp"
#include <boost/regex.hpp>
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <base/Time.hpp>

namespace satcomms_kvh {
    class SatcommsStatusManager {
    private:
        const std::vector<std::string> m_status_id = {
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

    public:
        std::string url_link;
        std::string url_data;
        base::Time timeout = base::Time::fromSeconds(1);
        SatcommsStatusManager();

        void setURL(std::string url);
        void setTimeout(base::Time timeout);
        /** Callback function used by the curl library to fill variable with the HTML
         * content
         */
        static size_t writeCallback(void* contents,
            size_t size,
            size_t nmemb,
            void* userp);
        /** Get the html page data
         */
        void getURLData();

        /**  This function process the html page and searches all the keywords
         */
        std::map<std::string, std::string> processText(
            std::vector<std::string> status_id);
        /**  Fill the the struct SatcomsStatus with the obtained data
         */
        SatcommsStatus getSatcommsStatus();

        float convertStringToFloat(std::string text);
    };

} // end namespace satcomms_kvh

#endif // _SATCOMMSSTATUSPROJECT_SATCOMMSSTATUS_HPP_
