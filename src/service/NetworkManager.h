#ifdef ARDUINO
#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <StandardDefines.h>
#include "INetworkManager.h"
#include "IWifiService.h"
#include "../entity/WifiCredentials.h"
#include <WiFi.h>

/// @Component
class NetworkManager : public INetworkManager {
    Public Virtual ~NetworkManager() = default;

    /// @Autowired
    IWifiServicePtr wifiService;

    // Track current mode: "wifi" or "hotspot"
    Private StdString currentMode;

    // Connect to network (WiFi or start hotspot)
    Public Virtual Void ConnectNetwork() override {
        // Check if there's a last connected WiFi
        optional<WifiCredentials> lastWifi = wifiService->GetLastConnectedWifi();
        
        if (lastWifi.has_value() && 
            lastWifi.value().ssid.has_value() && 
            !lastWifi.value().ssid.value().empty()) {
            // Connect to last connected WiFi
            StdString ssid = lastWifi.value().ssid.value();
            StdString password = "";
            
            if (lastWifi.value().password.has_value()) {
                password = lastWifi.value().password.value();
            }
            
            // Disconnect any existing connection first
            WiFi.disconnect();
            WiFi.mode(WIFI_STA);
            
            // Connect to WiFi
            WiFi.begin(ssid.c_str(), password.empty() ? nullptr : password.c_str());
            currentMode = "wifi";
        } else {
            // No last connected WiFi, start hotspot
            WiFi.disconnect();
            WiFi.mode(WIFI_AP);
            
            // Start hotspot with SSID "Mishulika" and no password (open)
            WiFi.softAP("Mishulika", nullptr);
            currentMode = "hotspot";
        }
    }

    // Disconnect from network (stop WiFi or hotspot)
    Public Virtual Void DisconnectNetwork() override {
        // Check actual WiFi mode to determine what to disconnect
        WiFiMode_t mode = WiFi.getMode();
        
        if (mode == WIFI_AP || mode == WIFI_AP_STA) {
            // Stop hotspot
            WiFi.softAPdisconnect(true);
        }
        
        if (mode == WIFI_STA || mode == WIFI_AP_STA) {
            // Disconnect WiFi
            WiFi.disconnect();
        }
        
        currentMode = "";
    }

    // Check if network is connected (WiFi or hotspot)
    Public Virtual Bool IsNetworkConnected() override {
        // Check if WiFi is connected
        if (WiFi.status() == WL_CONNECTED) {
            return true;
        }
        
        // Check if hotspot is active
        if (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA) {
            return true;
        }
        
        return false;
    }

    // Restart network (disconnect and reconnect)
    Public Virtual Void RestartNetwork() override {
        DisconnectNetwork();
        delay(1000); // Small delay before reconnecting
        ConnectNetwork();
    }
};

#endif // NETWORKMANAGER_H
#endif // ARDUINO   
