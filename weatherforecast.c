#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>

int main() {
    CURL *curl;
    CURLcode res;
    char url[256];
    char city_name[256];
    char api_key[256];
    double temp, humidity, wind_speed, wind_direction;
    char* data;
    json_error_t error;
    json_t* root, *main, *wind;

    // API endpoint and API key
    strcpy(api_key, "cf87caec981c552fe6909bd59b17c832");
    strcpy(url, "https://api.openweathermap.org/data/2.5/weather?q=");
    strcat(url, city_name);
    strcat(url, "&appid=");
    strcat(url, api_key);

    // User input
    printf("Enter city name: ");
    scanf("%s", city_name);

    // Set up curl
    curl = curl_easy_init();
    if (curl) {
        // Make API request
        curl_easy_setopt(curl, CURLOPT_URL, url);
        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            // Parse JSON response
            json_loads(data, 0, &error);
            root = json_loads(data, 0, &error);
            if (!root) {
                fprintf(stderr, "Error parsing JSON: %s\n", error.text);
                return 1;
            }

            // Get weather data
            main = json_object_get(root, "main");
            temp = (json_number_value(json_object_get(main, "temp")) - 273.15);
            humidity = json_number_value(json_object_get(main, "humidity"));
            wind = json_object_get(root, "wind");
            wind_speed = json_number_value(json_object_get(wind, "speed"));
            wind_direction = json_number_value(json_object_get(wind, "deg"));

            // Display weather data
            printf("Current weather in %s:\n", city_name);
            printf("Temperature(in Kelvin unit)= %.1f °C\n", temp);
            printf("atmospheric pressure(in hPa unit)= %.1f m/s\n", wind_speed);
            printf("humidity(in percentage)= %.0f %%\n", humidity);
            printf("description= %.0f °\n", wind_direction);
        } 
        else {
            printf(stderr, "Error making API request: %s\n", curl_easy_strerror(res));
            return 1;
        }
        // Clean up curl
        curl_easy_cleanup(curl);
    } else {
        printf(stderr, "Error setting up curl.\n");
        return 1;
    }
    return 0;
}
