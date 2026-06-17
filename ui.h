#pragma once
#include <string>
#include <iostream>
using namespace std;

// ── ANSI styling (256-color palette) ─────────────────────────────────────────
inline const string RESET  = "\033[0m";
inline const string BOLD   = "\033[1m";
inline const string DIM    = "\033[2m";

inline const string GREEN  = "\033[38;5;46m";
inline const string RED    = "\033[38;5;196m";
inline const string YELLOW = "\033[38;5;220m";
inline const string CYAN   = "\033[38;5;51m";
inline const string BLUE   = "\033[38;5;39m";
inline const string ORANGE = "\033[38;5;208m";
inline const string GRAY   = "\033[38;5;245m";
inline const string WHITE  = "\033[38;5;231m";

// ── screen control ────────────────────────────────────────────────────────────
inline void clearScreen() { cout << "\033[2J\033[3J\033[H" << flush; }
inline void hideCursor()  { cout << "\033[?25l" << flush; }
inline void showCursor()  { cout << "\033[?25h" << flush; }

// ── text helpers ──────────────────────────────────────────────────────────────
inline string padRight(const string& s, size_t w) {
    return s.size() >= w ? s : s + string(w - s.size(), ' ');
}
inline string padLeft(const string& s, size_t w) {
    return s.size() >= w ? s : string(w - s.size(), ' ') + s;
}
inline string center(const string& s, size_t w) {
    if (s.size() >= w) return s;
    size_t pad = w - s.size(), l = pad / 2;
    return string(l, ' ') + s + string(pad - l, ' ');
}

inline string trim(const string& s) {
    const string ws = " \t\r\n";
    size_t start = s.find_first_not_of(ws);
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(ws);
    return s.substr(start, end - start + 1);
}

inline const string& statusColor(const string& st) {
    if (st == "Available")   return GREEN;
    if (st == "Occupied")    return RED;
    if (st == "Maintenance") return YELLOW;
    return WHITE;
}
