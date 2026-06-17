#include <iostream>
#include <string>
#include <limits>
#include "network.h"
#include "ui.h"
using namespace std;

static const int BOX_W = 59;

static void drawBanner() {
    string top = "╭"; for (int i = 0; i < BOX_W; ++i) top += "─"; top += "╮";
    string bot = "╰"; for (int i = 0; i < BOX_W; ++i) bot += "─"; bot += "╯";

    cout << "  " << CYAN << top << RESET << "\n";
    cout << "  " << CYAN << "│" << RESET << BOLD << WHITE
         << center("SMART  EV  CHARGING  NETWORK", BOX_W) << RESET
         << CYAN << "│" << RESET << "\n";
    cout << "  " << CYAN << "│" << RESET << GRAY
         << center("Singly Linked List  -  ITM Skills University", BOX_W) << RESET
         << CYAN << "│" << RESET << "\n";
    cout << "  " << CYAN << bot << RESET << "\n";
}

static void drawMenu() {
    auto opt = [](const string& n, const string& label) {
        return CYAN + "[" + n + "]" + RESET + " " + WHITE + label + RESET + "   ";
    };
    cout << "\n  " << GRAY << "──────────────────────────────────────────────────"
         << "──────────" << RESET << "\n  "
         << opt("1", "Add") << opt("2", "Remove") << opt("3", "Status")
         << opt("4", "Refresh") << "\n  "
         << opt("5", "Search") << opt("6", "Queue+") << opt("7", "Exit") << "\n";
}

static void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static string ask(const string& label) {
    cout << "  " << CYAN << "▸ " << RESET << label << ": " << WHITE << flush;
    string s; getline(cin, s);
    cout << RESET;
    return s;
}

int main() {
    ChargingNetwork network;
    string message   = GRAY + "Welcome. Add a station to begin." + RESET;
    string highlight = "";

    while (true) {
        clearScreen();
        drawBanner();
        cout << "\n";
        network.displayAll(highlight);
        cout << "\n  " << message << "\n";
        drawMenu();
        cout << "\n  " << CYAN << "▸ " << RESET << "choice: " << WHITE << flush;

        int choice = 0;
        if (!(cin >> choice)) { clearInput(); choice = 0; }
        clearInput();
        cout << RESET;

        highlight = "";

        if (choice == 1) {
            string id  = ask("Station ID");
            string loc = ask("Location");
            cout << "  " << CYAN << "▸ " << RESET << "Capacity (kW): " << WHITE << flush;
            int cap = 0;
            if (!(cin >> cap)) { clearInput(); message = YELLOW + "⚠ " + RESET + "Invalid capacity."; continue; }
            clearInput();
            message = network.addStation(id, loc, cap);

        } else if (choice == 2) {
            message = network.removeStation(ask("Station ID to remove"));

        } else if (choice == 3) {
            string id = ask("Station ID");
            cout << "  " << GRAY << "  status → " << GREEN << "Available  "
                 << RED << "Occupied  " << YELLOW << "Maintenance" << RESET << "\n";
            string st = ask("New status");
            message = network.updateStatus(id, st);

        } else if (choice == 4) {
            message = GRAY + "Dashboard refreshed." + RESET;

        } else if (choice == 5) {
            string id = ask("Station ID to search");
            message   = network.searchStation(id);
            highlight = id;

        } else if (choice == 6) {
            message = network.incrementWaiting(ask("Station ID (car joins queue)"));

        } else if (choice == 7) {
            clearScreen();
            cout << "\n  " << CYAN << "⚡ Network shut down. All memory freed. ⚡"
                 << RESET << "\n\n";
            break;

        } else {
            message = YELLOW + "⚠ " + RESET + "Invalid choice — pick 1-7.";
        }
    }

    return 0;
}
