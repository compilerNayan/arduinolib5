#ifndef ISPRINGBOOTAPP_H
#define ISPRINGBOOTAPP_H

#include <StandardDefines.h>

DefineStandardPointers(ISpringBootApp)
class ISpringBootApp {
    Public Virtual ~ISpringBootApp() = default;

    /**
     * @brief Starts the Spring Boot application
     * Connects to network and starts the HTTP server
     * @return true if application started successfully, false otherwise
     */
    Public Virtual Bool StartApp() = 0;

    /**
     * @brief Stops the Spring Boot application
     * Disconnects from network and stops the HTTP server
     */
    Public Virtual Void StopApp() = 0;

    /**
     * @brief Restarts the Spring Boot application
     * Stops and then starts the application
     * @return true if application restarted successfully, false otherwise
     */
    Public Virtual Bool RestartApp() = 0;

    /**
     * @brief Listens for and processes HTTP requests
     * Retrieves requests, processes them, and sends responses
     */
    Public Virtual Void ListenToRequest() = 0;
};

#endif // ISPRINGBOOTAPP_H

