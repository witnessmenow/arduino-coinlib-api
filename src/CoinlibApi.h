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

#ifndef CoinlibApi_h
#define CoinlibApi_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Client.h>

#define COINLIB_HOST "coinlib.io"
#define COINLIB_TIMEOUT 1500

struct CoinData
{
    char *symbol;
    char *show_symbol;
    char *name;
    int rank;
    double price;
    double market_cap;
    double total_volume_24h;
    double low_24h;
    double high_24h;
    double delta_1h;
    double delta_24h;
    double delta_7d;
    double delta_30d;
};

class CoinlibApi
{
  public:
    CoinlibApi(Client &client, char *key);
    bool makeGetRequest(char *command);
    CoinData getCoinData(char *symbol, char *pref = "");
    int portNumber = 443;
    //bool _checkFingerPrint = true; //Fail request if fingerprint doesnt match
    bool _debug = false;
    Client *client;

  private:
    char *_key;
};

#endif