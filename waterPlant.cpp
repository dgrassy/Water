//
//  waterPlant.cpp
//  Water
//
//  Created by Dane Grassy on 7/6/24.
//

#include <iostream>
#include <curl/curl.h>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <iterator>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h> // Package only available on Raspberry Pi

int hCheck;

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string removeCharacters(std::string str, const char* charsToRemove) 
{
    for (unsigned int i = 0; i < std::strlen(charsToRemove); ++i) {
        str.erase(std::remove(str.begin(), str.end(), charsToRemove[i]), str.end());
    }
    return str;
}

int waterPlants(void)
{
    if (gpioInitialise() < 0) exit(1);
    // First number corresponds to the GPIO pin connected to relay, 255 corresponds to ON
    gpioPWM(14,255);
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(10s); // Sleep duration depends on how much water is needed
    gpioPWM(14,0);
    hCheck = 0;
    return 0;
}

int updateData()
{
    // Uses curl library to read sensor output
    curl_global_init(CURL_GLOBAL_ALL);
    CURL* easyhandle = curl_easy_init();
    std::string readBuffer;
    curl_easy_setopt(easyhandle, CURLOPT_URL, "http://192.168.4.1/get_livedata_info?");
    curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_perform(easyhandle);

    // Creates a vector of each string in the sensor's output
    std::istringstream iss{readBuffer};
        std::vector<std::string> words {
            std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>() };
     
    std::string humidity,
                battery;
    
    // In sensor data the string after humidity/battery is the corresponding value
    for (int i = 0; i < words.size(); i+=2) {
        if (words[i] == "\"battery\":"){
            battery = words[i+1];
        }
        if (words[i] == "\"humidity\":") {
            humidity = words[i+1];
        }
    };
    
    humidity = removeCharacters(humidity, "\"%");
    battery = removeCharacters(battery, "\"");

    int iHumidity, iBattery;
        
    std::stringstream humidityStream(humidity);
    humidityStream >> iHumidity;

    std::stringstream batteryStream(battery);
    batteryStream >> iBattery;

    // Reference "dry" level
    if (iHumidity < 10) {
        hCheck++;
    } else {
        hCheck = 0;
    }

    return 0;
}

int main()
{
    for (int i = 0; i < 10; i++) {
        updateData();
        // Confirms data reads multiple times in case sensor error/movement
        if (hCheck > 4) {
            waterPlants();
        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(30s);
    }
    return 0;
}

