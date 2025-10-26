#pragma once
#include <string>
#include <vector>
#include <cstdint>

// Represents one value change at a given time
struct Transition {
    uint64_t time;     // in simulation time units (raw from VCD after scaling)
    std::string value; // e.g. "0", "1", "x", "z", "1010"
};

// One HDL signal in the design
class Signal {
public:
    std::string fullName;    // e.g. "top.u1.clk"
    std::string reference;   // leaf name e.g. "clk"
    std::string scopePath;   // e.g. "top.u1"
    std::string idCode;      // VCD id like "!" or "aa"
    int width = 1;

    std::vector<Transition> transitions;
};
