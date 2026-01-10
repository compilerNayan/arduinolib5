#ifndef WIFI_CREDENTIALS_CONTROLLER_H
#define WIFI_CREDENTIALS_CONTROLLER_H

#include <StandardDefines.h>
#include "IWifiCredentialsController.h"
#include "../service/IWifiService.h"
#include "../entity/WifiCredentials.h"

/// @RestController
/// @RequestMapping("/wifi-credentials")
/// @Component
class WifiCredentialsController : public IWifiCredentialsController {
    Public
        /// @Autowired
        IWifiServicePtr wifiService;

        // Create - Add new WiFi credentials
        /// @PostMapping("")
        Public Virtual WifiCredentials CreateWifiCredentials(WifiCredentials credentials) override {
            return wifiService->AddWifiCredentials(credentials);
        }

        // Read - Get WiFi credentials by SSID
        /// @GetMapping("/{ssid}")
        Public Virtual WifiCredentials GetWifiCredentials(GetWifiCredentialsRequestDto request) override {
            if (!request.ssid.has_value() || request.ssid.value().empty()) {
                return WifiCredentials();
            }
            return wifiService->GetWifiCredentials(request.ssid.value());
        }

        // Read All - Get all WiFi credentials
        /// @GetMapping("")
        Public Virtual vector<WifiCredentials> GetAllWifiCredentials() override {
            return wifiService->GetAllWifiCredentials();
        }

        // Update - Update existing WiFi credentials
        /// @PutMapping("")
        Public Virtual WifiCredentials UpdateWifiCredentials(WifiCredentials credentials) override {
            return wifiService->UpdateWifiCredentials(credentials);
        }

        // Delete - Delete WiFi credentials by SSID
        /// @DeleteMapping("/{ssid}")
        Public Virtual Void DeleteWifiCredentials(DeleteWifiCredentialsRequestDto request) override {
            if (request.ssid.has_value() && !request.ssid.value().empty()) {
                wifiService->DeleteWifiCredentials(request.ssid.value());
            }
        }

        // Get last connected WiFi
        /// @GetMapping("/last-connected")
        Public Virtual optional<WifiCredentials> GetLastConnectedWifi() override {
            return wifiService->GetLastConnectedWifi();
        }
};

#endif // WIFI_CREDENTIALS_CONTROLLER_H

