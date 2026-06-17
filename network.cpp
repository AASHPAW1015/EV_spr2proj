#include "network.h"
#include <iostream>
#include <iomanip>
using namespace std;

static const string DIV = "  +------------+------------------------------+----------+-------------+---------+";

ChargingNetwork::ChargingNetwork() : head(nullptr) {}

ChargingNetwork::~ChargingNetwork() { freeAll(); }

void ChargingNetwork::freeAll() {
    ChargingStation* cur = head;
    while (cur) {
        ChargingStation* next = cur->next;
        delete cur;
        cur = next;
    }
    head = nullptr;
}

// ── add ──────────────────────────────────────────────────────────────────────

void ChargingNetwork::addStation(const string& id,
                                 const string& location,
                                 int capacity) {
    ChargingStation* cur = head;
    while (cur) {
        if (cur->id == id) {
            cout << "  [!] Station ID \"" << id << "\" already exists.\n";
            return;
        }
        cur = cur->next;
    }

    ChargingStation* node = new ChargingStation(id, location, capacity);
    if (!head) { head = node; }
    else {
        cur = head;
        while (cur->next) cur = cur->next;
        cur->next = node;
    }
    cout << "  [+] Station " << id << " added.\n";
}

// ── remove ───────────────────────────────────────────────────────────────────

void ChargingNetwork::removeStation(const string& id) {
    if (!head) { cout << "  [!] Network is empty.\n"; return; }

    if (head->id == id) {
        ChargingStation* del = head;
        head = head->next;
        delete del;
        cout << "  [-] Station " << id << " removed.\n";
        return;
    }

    ChargingStation* prev = head;
    ChargingStation* cur  = head->next;
    while (cur) {
        if (cur->id == id) {
            prev->next = cur->next;
            delete cur;
            cout << "  [-] Station " << id << " removed.\n";
            return;
        }
        prev = cur;
        cur  = cur->next;
    }
    cout << "  [!] Station ID \"" << id << "\" not found.\n";
}

// ── updateStatus ─────────────────────────────────────────────────────────────

void ChargingNetwork::updateStatus(const string& id, const string& newStatus) {
    ChargingStation* cur = head;
    while (cur) {
        if (cur->id == id) {
            cur->status = newStatus;
            cout << "  [~] Station " << id << " status -> " << newStatus << "\n";
            return;
        }
        cur = cur->next;
    }
    cout << "  [!] Station ID \"" << id << "\" not found.\n";
}

// ── searchStation ─────────────────────────────────────────────────────────────

void ChargingNetwork::searchStation(const string& id) const {
    const ChargingStation* cur = head;
    while (cur) {
        if (cur->id == id) { printNode(cur); return; }
        cur = cur->next;
    }
    cout << "  [!] Station ID \"" << id << "\" not found.\n";
}

// ── incrementWaiting ─────────────────────────────────────────────────────────

void ChargingNetwork::incrementWaiting(const string& id) {
    ChargingStation* cur = head;
    while (cur) {
        if (cur->id == id) {
            ++cur->waitingCount;
            cout << "  [Q] Station " << id
                 << " -- waiting count: " << cur->waitingCount << "\n";
            return;
        }
        cur = cur->next;
    }
    cout << "  [!] Station ID \"" << id << "\" not found.\n";
}

// ── displayAll ───────────────────────────────────────────────────────────────

void ChargingNetwork::displayAll() const {
    if (!head) { cout << "  [!] No stations in network.\n"; return; }

    cout << "\n" << DIV << "\n";
    cout << "  | "
         << left << setw(10) << "ID"      << " | "
         << setw(28) << "Location"         << " | "
         << setw(8)  << "Cap(kW)"          << " | "
         << setw(11) << "Status"           << " | "
         << setw(7)  << "Waiting" << " |\n";
    cout << DIV << "\n";

    const ChargingStation* cur = head;
    while (cur) {
        printNode(cur);
        cur = cur->next;
    }
    cout << DIV << "\n\n";
}

// ── printNode (private) ───────────────────────────────────────────────────────

void ChargingNetwork::printNode(const ChargingStation* s) const {
    cout << "  | "
         << left  << setw(10) << s->id       << " | "
         << setw(28) << s->location           << " | "
         << right << setw(8)  << s->capacity  << " | "
         << left  << setw(11) << s->status    << " | "
         << right << setw(7)  << s->waitingCount << " |\n";
}
