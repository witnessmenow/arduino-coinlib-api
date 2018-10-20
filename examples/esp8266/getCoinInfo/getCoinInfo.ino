/*******************************************************************
 *  Get Crypto coin info from Coinlib.io API
 *
 *  By Brian Lough
 *  https://www.youtube.com/channel/UCezJOfu7OtqGzd5xrP3q6WA
 *******************************************************************/

#include <CoinlibApi.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <ArduinoJson.h> // This Sketch doesn't technically need this, but the library does so it must be installed.

//------- Replace the following! ------
char ssid[] = "SSID";         // your network SSID (name)
char password[] = "password"; // your network key

// Create a new application on https://dev.twitch.tv/
#define COINLIB_API_KEY "12345678"

WiFiClientSecure client;
CoinlibApi coinlib(client, COINLIB_API_KEY);

unsigned long delayBetweenRequests = 60000; // Time between requests (1 minute)
unsigned long requestDueTime;               //time when request due

void setup()
{

    Serial.begin(115200);

    // kss._debug = true;

    // Set WiFi to station mode and disconnect from an AP if it was Previously
    // connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    // Attempt to connect to Wifi network:
    Serial.print("Connecting Wifi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    IPAddress ip = WiFi.localIP();
    Serial.println(ip);
}

void loop()
{

    if (millis() > requestDueTime)
    {
        //Serial.print("Free Heap: ");
        //Serial.println(ESP.getFreeHeap());

        Serial.print("Getting Data for: ");
        Serial.println("BTC");
        CoinData coinData = coinlib.getCoinData("BTC");
        Serial.println("---------Coin Details ---------");
        Serial.print("symbol: ");
        Serial.println(CoinData.symbol);

        Serial.print("show_symbol: ");
        Serial.println(CoinData.show_symbol);

        Serial.print("name: ");
        Serial.println(CoinData.name);

        Serial.print("rank: ");
        Serial.println(CoinData.rank);

        Serial.print("price: ");
        Serial.println(CoinData.price);

        Serial.print("market_cap: ");
        Serial.println(CoinData.market_cap);

        Serial.print("low_24h: ");
        Serial.println(CoinData.low_24h);

        Serial.print("high_24h: ");
        Serial.println(CoinData.high_24h);

        Serial.print("delta_1h: ");
        Serial.println(CoinData.delta_1h);

        Serial.print("delta_24h: ");
        Serial.println(CoinData.delta_24h);

        Serial.print("delta_7d: ");
        Serial.println(CoinData.delta_7d);

        Serial.print("delta_30d: ");
        Serial.println(CoinData.delta_30d);
        Serial.println("------------------------");

        requestDueTime = millis() + delayBetweenRequests;
    }
}