#ifndef _SATCOMMS_STATUS_PROJECT_SATCOMMS_STATUS_MANAGER_HPP_
#define _SATCOMMS_STATUS_PROJECT_SATCOMMS_STATUS_MANAGER_HPP_

#include "SatcommsStatus.hpp"
#include <base/Time.hpp>
#include <map>
#include <vector>

namespace satcomms_kvh {
    class SatcommsStatusManager {
    private:
        std::vector<std::string> const m_status_id = {
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

        void setURL(std::string const& url);
        void setTimeout(base::Time const& timeout);
        /** Callback function used by the curl library to fill variable with the HTML
         * content
         */
        static size_t writeCallback(void* contents,
            size_t size,
            size_t nmemb,
            void* userp);
        /** Get the html page data
         */
        bool getURLData();

        /**  This function process the html page and searches all the keywords
         */
        std::map<std::string, std::string> processText(
            std::vector<std::string> const& status_id);
        /**  Return the the struct SatcomsStatus with the obtained data
         */
        SatcommsStatus getSatcommsStatus();

        /**  Fill the the struct SatcomsStatus with the obtained data
         */
        SatcommsStatus parseSatcommsStatus();

        float convertStringToFloat(std::string const& text);
    };

} // end namespace satcomms_kvh

#endif // _SATCOMMSSTATUSPROJECT_SATCOMMSSTATUS_HPP_
