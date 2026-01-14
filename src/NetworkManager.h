#ifdef ARDUINO
#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <StandardDefines.h>
#include "INetworkManager.h"
#include "service/IWifiService.h"
#include "entity/WifiCredentials.h"
#include <WiFi.h>
#include <Arduino.h>

// Define print macros for Arduino
#define std_print(x) Serial.print(x)
#define std_println(x) Serial.println(x)

/* @Component */
class NetworkManager : public INetworkManager {
    Public Virtual ~NetworkManager() = default;

    /* @Autowired */
    IWifiServicePtr wifiService;

    // Track current mode: "wifi" or "hotspot"
    Private StdString currentMode;

    // Connect to network (WiFi or start hotspot)
    Public Virtual Void ConnectNetwork() override {
        std_println("[NetworkManager] ConnectNetwork() called");
        
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
            
            std_print("[NetworkManager] Connecting to WiFi - SSID: ");
            std_println(ssid.c_str());
            if (!password.empty()) {
                std_println("[NetworkManager] Using password");
            } else {
                std_println("[NetworkManager] No password (open network)");
            }
            
            // Disconnect any existing connection first
            WiFi.disconnect();
            WiFi.mode(WIFI_STA);
            
            // Connect to WiFi
            WiFi.begin(ssid.c_str(), password.empty() ? nullptr : password.c_str());
            currentMode = "wifi";
            
            // Wait for connection and log IP when connected
            int attempts = 0;
            while (WiFi.status() != WL_CONNECTED && attempts < 20) {
                delay(500);
                attempts++;
            }
            
            if (WiFi.status() == WL_CONNECTED) {
                std_print("[NetworkManager] WiFi connected! IP Address: ");
                std_println(WiFi.localIP().toString().c_str());
            } else {
                std_println("[NetworkManager] WiFi connection failed or timeout");
            }
        } else {
            // No last connected WiFi, start hotspot
            std_println("[NetworkManager] No last connected WiFi found, starting hotspot");
            std_println("[NetworkManager] Hotspot SSID: Mishulika (open, no password)");
            
            WiFi.disconnect();
            WiFi.mode(WIFI_AP);
            
            // Start hotspot with SSID "Mishulika" and no password (open)
            Bool apStarted = WiFi.softAP("Mishulika", nullptr);
            if (apStarted) {
                currentMode = "hotspot";
                std_print("[NetworkManager] Hotspot started! AP IP Address: ");
                std_println(WiFi.softAPIP().toString().c_str());
            } else {
                std_println("[NetworkManager] Failed to start hotspot");
            }
        }
    }

    // Disconnect from network (stop WiFi or hotspot)
    Public Virtual Void DisconnectNetwork() override {
        std_println("[NetworkManager] DisconnectNetwork() called");
        
        // Check actual WiFi mode to determine what to disconnect
        WiFiMode_t mode = WiFi.getMode();
        
        if (mode == WIFI_AP || mode == WIFI_AP_STA) {
            // Stop hotspot
            std_println("[NetworkManager] Stopping hotspot");
            WiFi.softAPdisconnect(true);
        }
        
        if (mode == WIFI_STA || mode == WIFI_AP_STA) {
            // Disconnect WiFi
            if (WiFi.status() == WL_CONNECTED) {
                std_print("[NetworkManager] Disconnecting WiFi - Previous IP: ");
                std_println(WiFi.localIP().toString().c_str());
            }
            WiFi.disconnect();
        }
        
        currentMode = "";
        std_println("[NetworkManager] Network disconnected");
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
        std_println("[NetworkManager] RestartNetwork() called");
        DisconnectNetwork();
        delay(1000); // Small delay before reconnecting
        std_println("[NetworkManager] Reconnecting...");
        ConnectNetwork();
    }
};

#endif // NETWORKMANAGER_H
#endif // ARDUINO   
