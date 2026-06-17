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

    void addStation(const string& id, const string& location, int capacity);
    void removeStation(const string& id);
    void updateStatus(const string& id, const string& newStatus);
    void displayAll() const;
    void searchStation(const string& id) const;
    void incrementWaiting(const string& id);

private:
    void printNode(const ChargingStation* s) const;
    void freeAll();
};
