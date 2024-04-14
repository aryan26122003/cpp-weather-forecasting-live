#include <iostream>
#include <curl/curl.h>
#include <json/json.h>

const std::string API_KEY = "4fbe2c01a4d83c4d1037f081fb0769f7";

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

int main() {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (!curl) {                 
        std::cerr << "curl initialization failed." << std::endl;
        return 1;
    }

    std::string city;
    std::cout<<"Enter city name : ";
    std::cin>>city;

    std::string apiURL = "http://api.weatherstack.com/current?access_key="+ API_KEY +"&query="+city+"";

    curl_easy_setopt(curl, CURLOPT_URL, apiURL.c_str());

    curl_easy_setopt(curl, CURLOPT_CAINFO, "D:\\API\\cacert.pem");

    std::string response;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return 1;
    }

    //std::cout << "Weather Data for " << city << ":\n" << response << std::endl;

    curl_easy_cleanup(curl);

    Json::CharReaderBuilder reader;
    Json::Value root;
    std::istringstream stream(response);
    std::string errs;
    Json::parseFromStream(reader, stream, &root, &errs);

    std::cout << "\n\n\n";
    std::cout << "City: " << root["location"]["name"].asString() << std::endl;
    std::cout << "Country: " << root["location"]["country"].asString() << std::endl;
    std::cout << "Temperature: " << root["current"]["temperature"].asInt() << " °C" << std::endl;
    std::cout << "Wind Speed: " << root["current"]["wind_speed"].asInt() << " km/h" << std::endl;
    std::cout << "Humidity: " << root["current"]["humidity"].asInt() << "%" << std::endl;
    return 0;
}
