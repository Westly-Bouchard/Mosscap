//
// Created by west on 5/7/26.
//

#ifndef INC_441SIM_TELEMETRY_H
#define INC_441SIM_TELEMETRY_H

#include <thread>
#include <utility>
#include <vector>

// Forward declare
class TelemetryProvider;

/**
 * Singleton that manages all telemetry writing
 * to the ImGui telemetry window.
 */
class Telemetry {
public:
    /**
     * Singleton
     * @return Telemetry instance
     */
    static Telemetry& getInstance();

    /**
     * Deleted because singleton
     */
    Telemetry(const Telemetry&) = delete;
    void operator=(const Telemetry&) = delete;

    /**
     * Write from all providers
     */
    void write() const;

    /**
     * Register a provider with the Telemetry system.
     * The robot / simulator writes with priority zero. So values above
     * any robot telemetry should write with a negative priority and
     * values that should appear below the robot telemetry should use a
     * positive priority.
     * @param priority Priority of provider
     * @param provider Pointer to obj that implements TelemetryProvider
     */
    void registerTelemetryProvider(int priority, TelemetryProvider* provider);

    /**
     * This provides a method for a provider that goes out of scope to remove
     * itself from the list of providers.
     * @param provider Provider to remove
     */
    void unregisterTelemetryProvider(TelemetryProvider* provider);

    /**
     * This just wraps the ImgGui::CollapsingHeader function
     * The goal is to insulate implementers from direct ImGui calls
     * in the common case.
     *
     * @param name Name of header at the top of the section
     * @return Whether the header is collapsed
     */
    static bool section(const std::string& name);

    /**
     * Similarly to Telemetry::section, this just wraps the ImGui::Text
     * function to insulate implementers from direct ImGui calls in the
     * common case.
     * @param text Text to display
     */
    static void text(const std::string& text);

private:
    /**
     * Private because singleton
     */
    Telemetry() = default;
    ~Telemetry() = default;

    /**
     * The vector of providers
     */
    std::vector<std::pair<int, TelemetryProvider*>> providers;

    /**
     * Guards against erroneous ImGui calls when not drawing a frame
     */
    inline static bool writing = false;
};

/**
 * Interface for objects to implement that allows them to write telemetry
 * data to the ImGui telemetry window.
 */
class TelemetryProvider {
public:
    /**
     * Implementer must specify the priority
     * @param priority Priority of data
     */
    explicit TelemetryProvider(const int priority) : p(priority) {
        Telemetry::getInstance().registerTelemetryProvider(priority, this);
    };

    /**
     * Copy constructor and copy assignment to ensure that copied Providers will
     * still be written even if the original provider goes out of scope.
     */
    TelemetryProvider(const TelemetryProvider& other) : p(other.p){
        Telemetry::getInstance().registerTelemetryProvider(other.p, this);
    }

    TelemetryProvider &operator=(const TelemetryProvider& other) {
        if (this != &other) {
            p = other.p;
            Telemetry::getInstance().registerTelemetryProvider(other.p, this);
        }

        return *this;
    }

    /**
     * Dtor unregisters the object from the telemetry system
     */
    virtual ~TelemetryProvider() {
        Telemetry::getInstance().unregisterTelemetryProvider(this);
    }

    /**
     * Write function to be implemented by derived classes
     */
    virtual void write() const = 0;

protected:
    /**
     * Included so that objects that run on the Arduino thread can provide
     * telemetry if desired. At the moment, properly synchronizing threads
     * is the responsibility of the implementer, this is not ideal.
     */
    std::mutex telemMtx;

private:
    int p;
};

#endif //INC_441SIM_TELEMETRY_H
