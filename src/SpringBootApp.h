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
#else
#include <iostream>
// Define print macros for desktop
#define std_print(x) std::cout << x
#define std_println(x) std::cout << x << std::endl
#endif

/* @Component */
class SpringBootApp : public ISpringBootApp {
    Public Virtual ~SpringBootApp() = default;

    /* @Autowired */
    Private INetworkManagerPtr networkManager;

    /* @Autowired */
    Private IHttpRequestManagerPtr requestManager;

    Public SpringBootApp() {
        std_println("[SpringBootApp] Constructor called");
#ifdef ARDUINO
       // Serial.begin(115200);
        //delay(1000);
        std_println("[SpringBootApp] Serial initialized at 115200 baud");
#endif
        
        // Check if dependencies are injected
        if (networkManager == nullptr) {
            std_println("[SpringBootApp] WARNING: networkManager is nullptr!");
        } else {
            std_println("[SpringBootApp] networkManager injected successfully");
        }
        
        if (requestManager == nullptr) {
            std_println("[SpringBootApp] WARNING: requestManager is nullptr!");
        } else {
            std_println("[SpringBootApp] requestManager injected successfully");
        }
    }

    Public Virtual Bool StartApp() override {
        std_println("[SpringBootApp] StartApp() called");
        
        // Check if dependencies are available
        if (networkManager == nullptr) {
            std_println("[SpringBootApp] ERROR: networkManager is nullptr! Cannot start app.");
            return false;
        }
        
        if (requestManager == nullptr) {
            std_println("[SpringBootApp] ERROR: requestManager is nullptr! Cannot start app.");
            return false;
        }
        
        // Connect to network first
        std_println("[SpringBootApp] Step 1: Connecting to network...");
        networkManager->ConnectNetwork();
        
        // Check network connection status
        Bool networkConnected = networkManager->IsNetworkConnected();
        if (networkConnected) {
            std_println("[SpringBootApp] Network connected successfully");
        } else {
            std_println("[SpringBootApp] WARNING: Network connection status is false");
        }
        
        // Then start the HTTP server
        std_println("[SpringBootApp] Step 2: Starting HTTP server...");
        std_println("[SpringBootApp] Using default port: 8080");
        Bool serverStarted = requestManager->StartServer();
        
        if (serverStarted) {
            std_println("[SpringBootApp] HTTP server started successfully");
            std_println("[SpringBootApp] StartApp() completed successfully");
        } else {
            std_println("[SpringBootApp] ERROR: Failed to start HTTP server");
            std_println("[SpringBootApp] Check the logs above for detailed error information");
        }
        
        return serverStarted;
    }

    Public Virtual Void StopApp() override {
        std_println("[SpringBootApp] StopApp() called");
        
        if (requestManager == nullptr) {
            std_println("[SpringBootApp] WARNING: requestManager is nullptr! Cannot stop server.");
        } else {
            std_println("[SpringBootApp] Step 1: Stopping HTTP server...");
            requestManager->StopServer();
            std_println("[SpringBootApp] HTTP server stopped");
        }
        
        if (networkManager == nullptr) {
            std_println("[SpringBootApp] WARNING: networkManager is nullptr! Cannot disconnect network.");
        } else {
            std_println("[SpringBootApp] Step 2: Disconnecting from network...");
            networkManager->DisconnectNetwork();
            std_println("[SpringBootApp] Network disconnected");
        }
        
        std_println("[SpringBootApp] StopApp() completed");
    }

    Public Virtual Bool RestartApp() override {
        std_println("[SpringBootApp] RestartApp() called");
        std_println("[SpringBootApp] Step 1: Stopping application...");
        
        // Stop the application
        StopApp();
        
        std_println("[SpringBootApp] Step 2: Starting application...");
        // Start the application
        Bool result = StartApp();
        
        if (result) {
            std_println("[SpringBootApp] RestartApp() completed successfully");
        } else {
            std_println("[SpringBootApp] RestartApp() failed");
        }
        
        return result;
    }

    Public Virtual Void ListenToRequest() override {
        // Check if requestManager is available
        if (requestManager == nullptr) {
            std_println("[SpringBootApp] ERROR: requestManager is nullptr! Cannot listen to requests.");
            return;
        }
        
        // Retrieve incoming requests
        Bool requestRetrieved = requestManager->RetrieveRequest();
        if (requestRetrieved) {
            std_println("[SpringBootApp] Request retrieved from server");
        }
        
        // Process requests
        Bool requestProcessed = requestManager->ProcessRequest();
        if (requestProcessed) {
            std_println("[SpringBootApp] Request processed");
        }
        
        // Process responses
        Bool responseProcessed = requestManager->ProcessResponse();
        if (responseProcessed) {
            std_println("[SpringBootApp] Response processed");
        }
    }
};

#endif // SPRINGBOOTAPP_H

