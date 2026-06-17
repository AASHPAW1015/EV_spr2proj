#include <iostream>
#include <string>
#include <limits>
#include "network.h"
using namespace std;

static void drawBanner() {
    cout << R"(
  +===========================================================+
  |          SMART EV CHARGING NETWORK  --  DSA Project      |
  |          ITM Skills University  |  B.Tech CSE 2025-29    |
  +===========================================================+
)";
}

static void menu() {
    cout <<
        "\n  +-----------------------------------+\n"
        "  |          NETWORK MENU             |\n"
        "  +-----------------------------------+\n"
        "  |  1. Add Station                   |\n"
        "  |  2. Remove Station                |\n"
        "  |  3. Update Station Status         |\n"
        "  |  4. Display All Stations          |\n"
        "  |  5. Search Station                |\n"
        "  |  6. Increment Waiting Count       |\n"
        "  |  7. Exit                          |\n"
        "  +-----------------------------------+\n"
        "  Choice: ";
}

static void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    drawBanner();

    ChargingNetwork network;
    int choice = 0;

    while (true) {
        menu();
        if (!(cin >> choice)) { clearInput(); choice = 0; }
        clearInput();

        cout << "\n";

        if (choice == 1) {
            string id, location;
            int cap;
            cout << "  Station ID    : "; getline(cin, id);
            cout << "  Location      : "; getline(cin, location);
            cout << "  Capacity (kW) : "; cin >> cap; clearInput();
            network.addStation(id, location, cap);

        } else if (choice == 2) {
            string id;
            cout << "  Station ID to remove: "; getline(cin, id);
            network.removeStation(id);

        } else if (choice == 3) {
            string id, status;
            cout << "  Station ID : "; getline(cin, id);
            cout << "  New status (Available / Occupied / Maintenance): ";
            getline(cin, status);
            network.updateStatus(id, status);

        } else if (choice == 4) {
            network.displayAll();

        } else if (choice == 5) {
            string id;
            cout << "  Station ID to search: "; getline(cin, id);
            network.searchStation(id);

        } else if (choice == 6) {
            string id;
            cout << "  Station ID: "; getline(cin, id);
            network.incrementWaiting(id);

        } else if (choice == 7) {
            cout << "  Shutting down network. Memory freed. Goodbye.\n\n";
            break;

        } else {
            cout << "  [!] Invalid choice. Enter 1-7.\n";
        }
    }

    return 0;
}
