#ifdef signals
#undef signals
#endif

#pragma once
#include "Signal.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <cstdint>

class WaveformDatabase {
public:
    // timescale in seconds per tick (ex: 1e-9 for 1ns)
    double timeScaleSec = 1e-9;

    // raw min/max time seen in file
    uint64_t minTime = 0;
    uint64_t maxTime = 0;

    // All signals by full hierarchical name
    std::unordered_map<std::string, std::unique_ptr<Signal>> signalsByName;

    // Map from VCD id code -> Signal*
    std::unordered_map<std::string, Signal*> idCodeMap;

    // Hierarchy tree for GUI
    // Each node has children scopes and leaf signals
    struct HierNode {
        std::string name;                      // scope name at this level
        std::unordered_map<std::string, std::unique_ptr<HierNode>> children;
        std::vector<Signal*> signals;          // signals directly under this scope
    };

    std::unique_ptr<HierNode> root = std::make_unique<HierNode>();

    WaveformDatabase() {
        root->name = "root";
    }

    // Create or get a scope node like top.u1.u2
    HierNode* getOrCreateScopeNode(const std::string& scopePath);

    // Add a signal to DB and hierarchy
    Signal* addSignal(const std::string& fullName,
                      const std::string& scopePath,
                      const std::string& reference,
                      const std::string& idCode,
                      int width);

    // Update min/max time
    void noteTime(uint64_t t);
};
