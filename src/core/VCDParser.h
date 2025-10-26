#pragma once
#include "WaveformDatabase.h"
#include <string>

class VCDParser {
public:
    bool parseFile(const std::string& path, WaveformDatabase& db, std::string& errorMsg);

private:
    bool parseHeader(std::istream& in, WaveformDatabase& db, std::string& errorMsg);
    bool parseBody(std::istream& in, WaveformDatabase& db, std::string& errorMsg);

    // helper during parsing
    std::string currentScopePath; // e.g. "top.u1.u2"
    uint64_t currentTime = 0;
    double timescaleFactorSec = 1e-9; // default 1ns if not found

    void pushScope(const std::string& name);
    void popScope();

    void addScalarChange(WaveformDatabase& db, const std::string& value, const std::string& id);
    void addVectorChange(WaveformDatabase& db, const std::string& value, const std::string& id);

    static double decodeTimescale(const std::string& number, const std::string& unit);
};
