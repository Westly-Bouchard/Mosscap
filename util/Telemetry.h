//
// Created by west on 5/7/26.
//

#ifndef INC_441SIM_TELEMETRY_H
#define INC_441SIM_TELEMETRY_H

#include <thread>
#include <utility>
#include <vector>

class TelemetryProvider;

class Telemetry {
public:
    static Telemetry& getInstance();

    Telemetry(const Telemetry&) = delete;
    void operator=(const Telemetry&) = delete;

    void write() const;

    void registerTelemetryProvider(int priority, TelemetryProvider* provider);

    void unregisterTelemetryProvider(TelemetryProvider* provider);

private:
    Telemetry() = default;
    ~Telemetry() = default;

    std::vector<std::pair<int, TelemetryProvider*>> providers;
};

class TelemetryProvider {
public:
    explicit TelemetryProvider(const int priority) : p(priority) {
        Telemetry::getInstance().registerTelemetryProvider(priority, this);
    };

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

    virtual ~TelemetryProvider() {
        Telemetry::getInstance().unregisterTelemetryProvider(this);
    }

    virtual void write() const = 0;

protected:
    std::mutex telemMtx;

private:
    int p;
};

#endif //INC_441SIM_TELEMETRY_H
