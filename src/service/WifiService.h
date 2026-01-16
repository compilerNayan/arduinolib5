#ifndef WIFISERVICE_H
#define WIFISERVICE_H

#include <StandardDefines.h>
#include "IWifiService.h"
#include "../repository/WifiCredentialsRepository.h"
#include "../repository/WifiConnectionRepository.h"
#include "../entity/WifiCredentials.h"
#include "../entity/WifiConnection.h"

/* @Service */
class WifiService : public IWifiService {
    Public Virtual ~WifiService() = default;

    /* @Autowired */
    WifiCredentialsRepositoryPtr wifiCredentialsRepository;

    /* @Autowired */
    WifiConnectionRepositoryPtr wifiConnectionRepository;

    // Add WiFi credentials
    Public Virtual WifiCredentials AddWifiCredentials(const WifiCredentials& credentials) override {
        // Save credentials
        WifiCredentials creds = credentials;
        WifiCredentials saved = wifiCredentialsRepository->Save(creds);
        
        return saved;
    }

    // Update WiFi credentials
    Public Virtual WifiCredentials UpdateWifiCredentials(const WifiCredentials& credentials) override {
        // Update credentials
        WifiCredentials creds = credentials;
        WifiCredentials updated = wifiCredentialsRepository->Update(creds);
        
        return updated;
    }

    // Delete WiFi credentials by SSID
    Public Virtual Void DeleteWifiCredentials(const StdString& ssid) override {
        // Check if this SSID is the last connected one
        Vector<WifiConnection> connections = wifiConnectionRepository->FindAll();
        if (!connections.empty()) {
            WifiConnection connection = connections[0];
            if (connection.lastConnectedSsid.has_value() && 
                connection.lastConnectedSsid.value() == ssid) {
                // Clear the last connected SSID since we're deleting it
                connection.lastConnectedSsid = std::nullopt;
                wifiConnectionRepository->Update(connection);
            }
        }
        
        // Delete the credentials
        wifiCredentialsRepository->DeleteById(ssid);
    }

    // Read WiFi credentials by SSID
    Public Virtual WifiCredentials GetWifiCredentials(const StdString& ssid) override {
        optional<WifiCredentials> result = wifiCredentialsRepository->FindById(ssid);
        if (result.has_value()) {
            return result.value();
        }
        // Return empty WifiCredentials if not found
        return WifiCredentials();
    }

    // Get all WiFi credentials
    Public Virtual Vector<WifiCredentials> GetAllWifiCredentials() override {
        return wifiCredentialsRepository->FindAll();
    }

    // Get last connected WiFi details
    Public Virtual optional<WifiCredentials> GetLastConnectedWifi() override {
        // Get all connection records (should only be one, but we'll check all)
        Vector<WifiConnection> connections = wifiConnectionRepository->FindAll();
        
        if (connections.empty()) {
            return std::nullopt;
        }
        
        // Get the first connection record (assuming single record for last connected)
        // If multiple exist, we'll use the first one
        WifiConnection connection = connections[0];
        
        if (!connection.lastConnectedSsid.has_value() || connection.lastConnectedSsid.value().empty()) {
            return std::nullopt;
        }
        
        // Fetch credentials using the last connected SSID
        optional<WifiCredentials> credentials = wifiCredentialsRepository->FindById(connection.lastConnectedSsid.value());
        
        // Return nullopt if credentials don't exist
        if (!credentials.has_value()) {
            return std::nullopt;
        }
        
        return credentials;
    }

    // Update last connected SSID (internal helper method)
    Private Void UpdateLastConnectedSsid(const StdString& ssid) {
        // Get all connection records
        Vector<WifiConnection> connections = wifiConnectionRepository->FindAll();
        
        WifiConnection connection;
        
        if (connections.empty()) {
            // Create new connection record
            connection.id = 1;
            connection.lastConnectedSsid = ssid;
            wifiConnectionRepository->Save(connection);
        } else {
            // Update existing connection record (use first one)
            connection = connections[0];
            connection.lastConnectedSsid = ssid;
            wifiConnectionRepository->Update(connection);
        }
    }
};

#endif // WIFISERVICE_H

