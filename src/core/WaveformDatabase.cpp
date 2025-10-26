#include "WaveformDatabase.h"
#include <sstream>

WaveformDatabase::HierNode* WaveformDatabase::getOrCreateScopeNode(const std::string& scopePath) {
    HierNode* node = root.get();
    if (scopePath.empty()) return node;

    std::stringstream ss(scopePath);
    std::string part;
    while (std::getline(ss, part, '.')) {
        if (part.empty()) continue;
        auto it = node->children.find(part);
        if (it == node->children.end()) {
            auto newNode = std::make_unique<HierNode>();
            newNode->name = part;
            auto ptr = newNode.get();
            node->children[part] = std::move(newNode);
            node = ptr;
        } else {
            node = it->second.get();
        }
    }
    return node;
}

Signal* WaveformDatabase::addSignal(const std::string& fullName,
                                    const std::string& scopePath,
                                    const std::string& reference,
                                    const std::string& idCode,
                                    int width) {
    auto sigPtr = std::make_unique<Signal>();
    sigPtr->fullName = fullName;
    sigPtr->scopePath = scopePath;
    sigPtr->reference = reference;
    sigPtr->idCode = idCode;
    sigPtr->width = width;

    Signal* raw = sigPtr.get();
    signalsByName[fullName] = std::move(sigPtr);
    idCodeMap[idCode] = raw;

    // attach to hierarchy
    HierNode* scopeNode = getOrCreateScopeNode(scopePath);
    scopeNode->signalList.push_back(raw);

    return raw;
}

void WaveformDatabase::noteTime(uint64_t t) {
    if (signalsByName.empty()) {
        minTime = maxTime = t;
    } else {
        if (t < minTime) minTime = t;
        if (t > maxTime) maxTime = t;
    }
}
