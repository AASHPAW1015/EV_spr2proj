#pragma once
#include <string>
using namespace std;

struct ChargingStation {
    string id;
    string location;
    int    capacity;
    string status;
    int    waitingCount;
    ChargingStation* next;

    ChargingStation(const string& id,
                    const string& location,
                    int capacity)
        : id(id), location(location), capacity(capacity),
          status("Available"), waitingCount(0), next(nullptr) {}
};
