#include "network.h"
#include "ui.h"
#include <iostream>
#include <string>
using namespace std;

static const int W_ID = 8, W_LOC = 24, W_CAP = 7, W_STAT = 13, W_WAIT = 7;

static string makeRule(const string& l, const string& m, const string& r) {
    const int w[5] = {W_ID, W_LOC, W_CAP, W_STAT, W_WAIT};
    string s = l;
    for (int i = 0; i < 5; ++i) {
        for (int k = 0; k < w[i] + 2; ++k) s += "─";
        s += (i < 4 ? m : r);
    }
    return s;
}

static string cellL(const string& s, int w, const string& c) {
    return " " + c + padRight(s, w) + RESET + " ";
}
static string cellR(const string& s, int w, const string& c) {
    return " " + c + padLeft(s, w) + RESET + " ";
}

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

bool ChargingNetwork::exists(const string& id) const {
    for (const ChargingStation* c = head; c; c = c->next)
        if (c->id == id) return true;
    return false;
}

string ChargingNetwork::addStation(const string& id,
                                   const string& location,
                                   int capacity) {
    if (exists(id))
        return YELLOW + "⚠ " + RESET + "Station \"" + id + "\" already exists.";

    ChargingStation* node = new ChargingStation(id, location, capacity);
    if (!head) {
        head = node;
    } else {
        ChargingStation* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = node;
    }
    return GREEN + "✔ " + RESET + "Station " + BOLD + id + RESET + " installed.";
}

string ChargingNetwork::removeStation(const string& id) {
    if (!head) return YELLOW + "⚠ " + RESET + "Network is empty.";

    if (head->id == id) {
        ChargingStation* del = head;
        head = head->next;
        delete del;
        return GREEN + "✔ " + RESET + "Station " + BOLD + id + RESET + " removed.";
    }

    ChargingStation* prev = head;
    ChargingStation* cur  = head->next;
    while (cur) {
        if (cur->id == id) {
            prev->next = cur->next;
            delete cur;
            return GREEN + "✔ " + RESET + "Station " + BOLD + id + RESET + " removed.";
        }
        prev = cur;
        cur  = cur->next;
    }
    return RED + "✘ " + RESET + "Station \"" + id + "\" not found.";
}

string ChargingNetwork::updateStatus(const string& id, const string& newStatus) {
    for (ChargingStation* cur = head; cur; cur = cur->next) {
        if (cur->id == id) {
            cur->status = newStatus;
            return CYAN + "✔ " + RESET + id + " → " +
                   statusColor(newStatus) + newStatus + RESET;
        }
    }
    return RED + "✘ " + RESET + "Station \"" + id + "\" not found.";
}

string ChargingNetwork::searchStation(const string& id) const {
    if (exists(id))
        return CYAN + "✔ " + RESET + "Found " + BOLD + id + RESET +
               GRAY + "  (highlighted below)" + RESET;
    return RED + "✘ " + RESET + "Station \"" + id + "\" not found.";
}

string ChargingNetwork::incrementWaiting(const string& id) {
    for (ChargingStation* cur = head; cur; cur = cur->next) {
        if (cur->id == id) {
            ++cur->waitingCount;
            return ORANGE + "● " + RESET + id + " queue → " +
                   ORANGE + to_string(cur->waitingCount) + RESET + " waiting.";
        }
    }
    return RED + "✘ " + RESET + "Station \"" + id + "\" not found.";
}

void ChargingNetwork::displayAll(const string& highlightId) const {
    const string V = CYAN + "│" + RESET;

    cout << "  " << CYAN << makeRule("╭", "┬", "╮") << RESET << "\n";
    cout << "  " << V
         << cellL("ID",       W_ID,   BOLD + WHITE) << V
         << cellL("Location", W_LOC,  BOLD + WHITE) << V
         << cellR("Cap(kW)",  W_CAP,  BOLD + WHITE) << V
         << cellL("Status",   W_STAT, BOLD + WHITE) << V
         << cellR("Waiting",  W_WAIT, BOLD + WHITE) << V << "\n";
    cout << "  " << CYAN << makeRule("├", "┼", "┤") << RESET << "\n";

    if (!head) {
        const int interior = (W_ID+2)+(W_LOC+2)+(W_CAP+2)+(W_STAT+2)+(W_WAIT+2)+4;
        cout << "  " << V << DIM
             << center("(no stations yet — press 1 to install one)", interior)
             << RESET << V << "\n";
    } else {
        int avail = 0, occ = 0, maint = 0;
        for (const ChargingStation* c = head; c; c = c->next) {
            printNode(c, !highlightId.empty() && c->id == highlightId);
            if (c->status == "Available")        ++avail;
            else if (c->status == "Occupied")    ++occ;
            else if (c->status == "Maintenance") ++maint;
        }
        cout << "  " << CYAN << makeRule("╰", "┴", "╯") << RESET << "\n";
        int total = avail + occ + maint;
        cout << "  " << YELLOW << "⚡ " << RESET << BOLD << total << RESET
             << GRAY << " stations    " << RESET
             << GREEN  << "● " << RESET << avail << " Available    "
             << RED    << "● " << RESET << occ   << " Occupied    "
             << YELLOW << "● " << RESET << maint << " Maintenance\n";
        return;
    }
    cout << "  " << CYAN << makeRule("╰", "┴", "╯") << RESET << "\n";
}

void ChargingNetwork::printNode(const ChargingStation* s, bool hl) const {
    const string V = CYAN + "│" + RESET;
    const string idCol  = hl ? (BOLD + CYAN) : WHITE;
    const string waitCol = s->waitingCount > 0 ? ORANGE : GRAY;

    const string& sc = statusColor(s->status);
    int vis   = 2 + (int)s->status.size();
    int trail = W_STAT - vis; if (trail < 0) trail = 0;
    string statusCell = " " + sc + "●" + RESET + " " + sc + s->status + RESET +
                        string(trail, ' ') + " ";

    cout << "  " << V
         << cellL(s->id,               W_ID,  idCol)   << V
         << cellL(s->location,         W_LOC, GRAY)    << V
         << cellR(to_string(s->capacity), W_CAP, WHITE) << V
         << statusCell                                   << V
         << cellR(to_string(s->waitingCount), W_WAIT, waitCol) << V;
    if (hl) cout << "  " << CYAN << "◀ found" << RESET;
    cout << "\n";
}
