#ifndef SPRINGBOOTAPP_H
#define SPRINGBOOTAPP_H

#include <StandardDefines.h>
#include "ISpringBootApp.h"
#include "INetworkManager.h"
#include <IHttpRequestManager.h>

/// @Component
class SpringBootApp : public ISpringBootApp {
    Public Virtual ~SpringBootApp() = default;

    /// @Autowired
    Private INetworkManagerPtr networkManager;

    /// @Autowired
    Private IHttpRequestManagerPtr requestManager;

    Public SpringBootApp() {
#ifdef ARDUINO
        Serial.begin(115200);
        delay(1000);
#endif
    }

    Public Virtual Bool StartApp() override {
        // Connect to network first
        networkManager->ConnectNetwork();
        
        // Then start the HTTP server
        return requestManager->StartServer();
    }

    Public Virtual Void StopApp() override {
        // Stop the HTTP server
        requestManager->StopServer();
        
        // Disconnect from network
        networkManager->DisconnectNetwork();
    }

    Public Virtual Bool RestartApp() override {
        // Stop the application
        StopApp();
        
        // Start the application
        return StartApp();
    }

    Public Virtual Void ListenToRequest() override {
        // Retrieve incoming requests
        requestManager->RetrieveRequest();
        
        // Process requests
        requestManager->ProcessRequest();
        
        // Process responses
        requestManager->ProcessResponse();
    }
};

#endif // SPRINGBOOTAPP_H

