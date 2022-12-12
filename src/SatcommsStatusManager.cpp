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
    this->url_link = url;
}

void SatcommsStatusManager::getURLData()
{
    CURL* curl_handle;

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, this->url_link.c_str());
    // disable progress meter, set to 0L to enable it
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, this->writeCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &this->url_data);
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
        if (regex_search(this->url_data,
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

    status_map = processText(this->status_id);

    SatcommsStatus status;
    status.online_offline_state = status_map["online_offline_state"];
    status.flrx_snr = status_map["flrx_snr"];
    status.antenna_status_az = status_map["antenna_status_az"];
    status.antenna_status_el = status_map["antenna_status_el"];
    status.antenna_state = status_map["antenna_state"];
    status.satelliteOrb = status_map["satelliteOrb"];
    status.beamInfo = status_map["beamInfo"];
    status.satellite_dlfreq = status_map["satellite_dlfreq"];
    status.modem_state = status_map["modem_state"];
    status.login_last = status_map["login_last"];
    status.flrx_state = status_map["flrx_state"];
    status.flrx_carrier = status_map["flrx_carrier"];
    status.rltx_state = status_map["rltx_state"];
    status.rltx_carrier = status_map["rltx_carrier"];
    status.rltx_power = status_map["rltx_power"];

    return status;
}