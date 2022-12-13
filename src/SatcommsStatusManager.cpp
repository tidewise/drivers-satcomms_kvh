#include "SatcommsStatusManager.hpp"

using namespace std;
using namespace satcomms_kvh;

SatcommsStatusManager::SatcommsStatusManager()
{
}

size_t SatcommsStatusManager::writeCallback(void* contents,
    size_t size,
    size_t nmemb,
    void* userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void SatcommsStatusManager::setURL(string url)
{
    url_link = url;
}

void SatcommsStatusManager::setTimeout(uint16_t timeout)
{
    timeout_miliseconds = timeout;
}

void SatcommsStatusManager::getURLData()
{
    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url_link.c_str());
    // disable progress meter, set to 0L to enable it
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT_MS, timeout_miliseconds);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &url_data);
    curl_easy_perform(curl_handle);

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
}

map<string, string> SatcommsStatusManager::processText(vector<string> status_id)
{
    map<string, string> status;
    boost::smatch matches;
    for (const auto& feature : status_id) {
        if (regex_search(url_data,
                matches,
                boost::regex("id=\"" + feature + "\".*?>(.*?)<"))) {
            status[feature] = matches[1];
        }
    }
    return status;
}

SatcommsStatus SatcommsStatusManager::getSatcommsStatus()
{
    map<string, string> status_map;

    status_map = processText(m_status_id);

    SatcommsStatus status;
    status.online_offline_state = status_map["online_offline_state"];
    status.flrx_snr = convertStringToFloat(status_map["flrx_snr"]);
    status.antenna_status_azimuth = convertStringToFloat(status_map["antenna_status_az"]);
    status.antenna_status_elevation =
        convertStringToFloat(status_map["antenna_status_el"]);
    status.antenna_state = status_map["antenna_state"];
    status.satellite = status_map["satelliteOrb"];
    status.beam = status_map["beamInfo"];
    status.frequency = convertStringToFloat(status_map["satellite_dlfreq"]);
    status.modem_state = status_map["modem_state"];
    status.last_login = status_map["login_last"];
    status.rx_stats_fl_state = status_map["flrx_state"];
    status.rx_stats_fl_carrier = status_map["flrx_carrier"];
    status.tx_stats_rl_state = status_map["rltx_state"];
    status.tx_stats_rl_carrier = status_map["rltx_carrier"];
    status.tx_stats_rl_power = convertStringToFloat(status_map["rltx_power"]);

    return status;
}

float SatcommsStatusManager::convertStringToFloat(std::string text)
{
    float number;
    try {
        number = std::stof(text);
    }
    catch (std::invalid_argument) {
        number = 0;
    }
    return number;
}