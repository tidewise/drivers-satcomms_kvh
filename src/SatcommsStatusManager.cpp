#include "SatcommsStatusManager.hpp"
#include <base-logging/Logging.hpp>
#include <base/Float.hpp>
#include <boost/regex.hpp>
#include <curl/curl.h>

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
    static_cast<string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

void SatcommsStatusManager::setURL(string const& url)
{
    url_link = url;
}

void SatcommsStatusManager::setTimeout(base::Time const& timeout)
{
    this->timeout = timeout;
}

bool SatcommsStatusManager::getURLData()
{
    CURL* curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url_link.c_str());
    // disable progress meter, set to 0L to enable it
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT_MS, timeout.toMilliseconds());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeCallback);

    string data;
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &data);

    int curl_code = curl_easy_perform(curl_handle);
    if (curl_code != CURLE_OK) {
        return false;
    }

    long http_code = 0;
    curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_code);

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    url_data = data;
    return (http_code == 200);
}

map<string, string> SatcommsStatusManager::processText(vector<string> const& status_id)
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
    if (!getURLData()) {
        throw std::runtime_error("Could not acquire URL data.");
    }
    else {
        return parseSatcommsStatus();
    }
}

SatcommsStatus SatcommsStatusManager::parseSatcommsStatus()
{
    map<string, string> status_map = processText(m_status_id);

    SatcommsStatus status;
    status.timestamp = base::Time::now();
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

float SatcommsStatusManager::convertStringToFloat(string const& text)
{
    try {
        return stof(text);
    }
    catch (invalid_argument const&) {
        LOG_ERROR_S << "could not parse " << text << " as float" << endl;
        return base::unknown<float>();
    }
}