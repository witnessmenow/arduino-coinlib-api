/*
   Copyright (c) 2018 Brian Lough. All right reserved.

   CoinlibApi - An Arduino library to wrap the Coinlib.io API

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "CoinlibApi.h"

CoinlibApi::CoinlibApi(Client &client, char *key)
{
    this->client = &client;
    this->_key = key;
}

bool CoinlibApi::makeGetRequest(char *command)
{

    client->setTimeout(COINLIB_TIMEOUT);
    if (!client->connect(COINLIB_HOST, portNumber))
    {
        Serial.println(F("Connection failed"));
        return false;
    }

    //Serial.println(F("Connected!"));

    // Default client doesnt have a verify, need to figure something else out.
    // if (_checkFingerPrint && !client->verify(TWITCH_FINGERPRINT, TWITCH_HOST))
    // {
    //     Serial.println(F("certificate doesn't match"));
    //     return false;
    // }

    // give the esp a breather
    yield();

    // Send HTTP request
    client->print(F("GET "));
    client->print(command);
    client->println(F(" HTTP/1.1"));

    //Headers
    client->print(F("Host: "));
    client->println(COINLIB_HOST);

    client->println(F("Cache-Control: no-cache"));

    if (client->println() == 0)
    {
        Serial.println(F("Failed to send request"));
        return false;
    }

    // Check HTTP status
    char status[32] = {0};
    client->readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status, "HTTP/1.1 200 OK") != 0)
    {
        Serial.print(F("Unexpected response: "));
        Serial.println(status);
        return false;
    }

    // Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!client->find(endOfHeaders))
    {
        Serial.println(F("Invalid response"));
        return false;
    }

    // Let the caller of this method parse the JSon from the client
    return true;
}

CoinData CoinlibApi::getCoinData(char *symbol, char *pref)
{
    char command[200] = "api/v1/coin?key=";
    strcat(command, _key);
    strcat(command, "&symbol=");
    strcat(command, symbol);
    if(strcmp(pref, "") != 0){
        strcat(command, "&pref=");
        strcat(command, pref);
    }
    const size_t bufferSize = 4*JSON_ARRAY_SIZE(3) + 9*JSON_OBJECT_SIZE(3) + 3*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(16) + 1500;

    CoinData coin;
    if (makeGetRequest(command))
    {
        // Allocate JsonBuffer
        // Use arduinojson.org/assistant to compute the capacity.
        DynamicJsonBuffer jsonBuffer(bufferSize);

        // Parse JSON object
        JsonObject &root = jsonBuffer.parseObject(*client);
        if (!root.success())
        {
            Serial.println(F("Parsing failed!"));
            return coin;
        }

        coin.symbol = (char *)root["symbol"].as<char*>();
        coin.show_symbol = (char *)root["show_symbol"].as<char*>();
        coin.name = (char *)root["name"].as<char*>();
        coin.rank = root["rank"].as<int>();
        coin.price = root["price"].as<double>(); 
        coin.market_cap = root["market_cap"].as<double>();
        coin.total_volume_24h = root["total_volume_24h"].as<double>();
        coin.low_24h = root["low_24h"].as<double>();
        coin.high_24h = root["high_24h"].as<double>();
        coin.delta_1h = root["delta_1h"].as<double>();
        coin.delta_24h = root["delta_24h"].as<double>();
        coin.delta_7d = root["delta_7d"].as<double>();
        coin.delta_30d = root["delta_30d"].as<double>();

        return coin;
    }
}