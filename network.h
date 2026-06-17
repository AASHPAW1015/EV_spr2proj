#pragma once
#include "station.h"
#include <string>
using namespace std;

class ChargingNetwork {
private:
    ChargingStation* head;

public:
    ChargingNetwork();
    ~ChargingNetwork();

    string addStation(const string& id, const string& location, int capacity);
    string removeStation(const string& id);
    string updateStatus(const string& id, const string& newStatus);
    string searchStation(const string& id) const;
    string incrementWaiting(const string& id);

    void displayAll(const string& highlightId = "") const;

private:
    void printNode(const ChargingStation* s, bool highlight) const;
    bool exists(const string& id) const;
    void freeAll();
};
