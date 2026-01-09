#ifndef WIFI_CREDENTIALS_CONTROLLER_H
#define WIFI_CREDENTIALS_CONTROLLER_H

#include <StandardDefines.h>
#include "../service/IWifiService.h"
#include "../entity/WifiCredentials.h"

/// @RestController
/// @RequestMapping("/wifi-credentials")
/// @Component
class WifiCredentialsController {
    Public:
        /// @Autowired
        IWifiServicePtr wifiService;

        // Create - Add new WiFi credentials
        /// @PostMapping("")
        WifiCredentials CreateWifiCredentials(WifiCredentials credentials) {
            return wifiService->AddWifiCredentials(credentials);
        }

        // Read - Get WiFi credentials by SSID
        /// @GetMapping("/{ssid}")
        optional<WifiCredentials> GetWifiCredentials(const StdString& ssid) {
            return wifiService->GetWifiCredentials(ssid);
        }

        // Read All - Get all WiFi credentials
        /// @GetMapping("")
        vector<WifiCredentials> GetAllWifiCredentials() {
            return wifiService->GetAllWifiCredentials();
        }

        // Update - Update existing WiFi credentials
        /// @PutMapping("")
        WifiCredentials UpdateWifiCredentials(WifiCredentials credentials) {
            return wifiService->UpdateWifiCredentials(credentials);
        }

        // Delete - Delete WiFi credentials by SSID
        /// @DeleteMapping("/{ssid}")
        Void DeleteWifiCredentials(const StdString& ssid) {
            wifiService->DeleteWifiCredentials(ssid);
        }

        // Get last connected WiFi
        /// @GetMapping("/last-connected")
        optional<WifiCredentials> GetLastConnectedWifi() {
            return wifiService->GetLastConnectedWifi();
        }
};

#endif // WIFI_CREDENTIALS_CONTROLLER_H

