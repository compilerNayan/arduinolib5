#ifndef SPRINGBOOTAPP_H
#define SPRINGBOOTAPP_H

#include <StandardDefines.h>
#include "ISpringBootApp.h"
#include "INetworkManager.h"
#include <IHttpRequestManager.h>

#ifdef ARDUINO
#include <Arduino.h>
// Define print macros for Arduino
#define std_print(x) Serial.print(x)
#define std_println(x) Serial.println(x)
#endif

/* @Component */
class SpringBootApp : public ISpringBootApp {
    Public Virtual ~SpringBootApp() = default;

    /* @Autowired */
    Private INetworkManagerPtr networkManager;

    /* @Autowired */
    Private IHttpRequestManagerPtr requestManager;

    Public SpringBootApp() {
#ifdef ARDUINO
        Serial.begin(115200);
        delay(1000);
        std_println("[SpringBootApp] Serial initialized at 115200 baud");
#endif
    }

    Public Virtual Bool StartApp() override {
        // Check if dependencies are available
        if (networkManager == nullptr) {
            return false;
        }
        
        if (requestManager == nullptr) {
            return false;
        }
        
        // Connect to network first
        networkManager->ConnectNetwork();
        
        // Check network connection status
        Bool networkConnected = networkManager->IsNetworkConnected();
#ifdef ARDUINO
        if (networkConnected) {
            std_println("[SpringBootApp] Network connected successfully");
        } else {
            std_println("[SpringBootApp] WARNING: Network connection status is false");
        }

        std_println("[SpringBootApp] Step 2: Starting HTTP server...");
        std_println("[SpringBootApp] Using default port: 8080");
        #endif        

        // Then start the HTTP server
        Bool serverStarted = requestManager->StartServer();
        
#ifdef ARDUINO
        if (serverStarted) {
            std_println("[SpringBootApp] HTTP server started successfully");
            std_println("[SpringBootApp] StartApp() completed successfully");
        } else {
            std_println("[SpringBootApp] ERROR: Failed to start HTTP server");
            std_println("[SpringBootApp] Check the logs above for detailed error information");
        }
#endif        
        return serverStarted;
    }

    Public Virtual Void StopApp() override {
        
        if (requestManager != nullptr) {
            requestManager->StopServer();
        }
        
        if (networkManager != nullptr) {
            networkManager->DisconnectNetwork();
        }
    }

    Public Virtual Bool RestartApp() override {
        // Stop the application
        StopApp();
        
        // Start the application
        Bool result = StartApp();
        
        return result;
    }

    Public Virtual Void ListenToRequest() override {
        // Check if requestManager is available
        if (requestManager == nullptr) {
            return;
        }
        
        // Retrieve incoming requests
        Bool requestRetrieved = requestManager->RetrieveRequest();
        
        // Process requests
        Bool requestProcessed = requestManager->ProcessRequest();
        
        // Process responses
        Bool responseProcessed = requestManager->ProcessResponse();
    }
};

#endif // SPRINGBOOTAPP_H

