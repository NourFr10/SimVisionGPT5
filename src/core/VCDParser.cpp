#include "VCDParser.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <vector>
#include <iostream>

static std::string trim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    size_t b = s.find_last_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    return s.substr(a, b - a + 1);
}

// map like "1 ns" -> 1e-9, "10 ps" -> 10e-12 etc.
double VCDParser::decodeTimescale(const std::string& number, const std::string& unit) {
    double n = std::stod(number);
    if (unit == "s")  return n;
    if (unit == "ms") return n * 1e-3;
    if (unit == "us") return n * 1e-6;
    if (unit == "ns") return n * 1e-9;
    if (unit == "ps") return n * 1e-12;
    if (unit == "fs") return n * 1e-15;
    // fallback ns
    return n * 1e-9;
}

void VCDParser::pushScope(const std::string& name) {
    if (currentScopePath.empty()) currentScopePath = name;
    else currentScopePath += "." + name;
}

void VCDParser::popScope() {
    auto pos = currentScopePath.find_last_of('.');
    if (pos == std::string::npos) {
        currentScopePath.clear();
    } else {
        currentScopePath = currentScopePath.substr(0, pos);
    }
}

// record scalar change like "1!" or "0aa"
void VCDParser::addScalarChange(WaveformDatabase& db, const std::string& value, const std::string& id) {
    auto it = db.idCodeMap.find(id);
    if (it == db.idCodeMap.end()) return;
    Signal* sig = it->second;
    sig->transitions.push_back({ currentTime, value });
    db.noteTime(currentTime);
}

// record vector change like "b1010 !" or "bzzzz aa"
void VCDParser::addVectorChange(WaveformDatabase& db, const std::string& value, const std::string& id) {
    auto it = db.idCodeMap.find(id);
    if (it == db.idCodeMap.end()) return;
    Signal* sig = it->second;
    sig->transitions.push_back({ currentTime, value });
    db.noteTime(currentTime);
}

bool VCDParser::parseHeader(std::istream& in, WaveformDatabase& db, std::string& errorMsg) {
    std::string line;
    bool endDefs = false;

    struct PendingVar {
        std::string id;
        std::string ref;
        int width;
    };

    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;

        if (line.rfind("$timescale", 0) == 0) {
            // Example: $timescale 1 ns $end
            // Could also span multiple tokens
            std::stringstream ss(line);
            std::string tok, num, unit;
            ss >> tok; // $timescale
            ss >> num;
            ss >> unit;
            timescaleFactorSec = decodeTimescale(num, unit);
            db.timeScaleSec = timescaleFactorSec;
            continue;
        }

        if (line.rfind("$scope", 0) == 0) {
            // $scope module top $end
            std::stringstream ss(line);
            std::string tok, kind, name, endtok;
            ss >> tok >> kind >> name >> endtok;
            pushScope(name);
            continue;
        }

        if (line.rfind("$upscope", 0) == 0) {
            popScope();
            continue;
        }

        if (line.rfind("$var", 0) == 0) {
            // $var wire 1 ! clk $end
            // $var wire 8 " data [7:0] $end
            std::stringstream ss(line);
            std::string tok, type;
            int width;
            std::string id, ref;
            ss >> tok >> type >> width >> id >> ref;
            // build fullName = currentScopePath + "." + ref
            std::string full = currentScopePath.empty() ? ref : currentScopePath + "." + ref;
            Signal* s = db.addSignal(full, currentScopePath, ref, id, width);
            (void)s;
            continue;
        }

        if (line.rfind("$enddefinitions", 0) == 0) {
            endDefs = true;
            break;
        }
    }

    if (!endDefs) {
        errorMsg = "VCD header ended unexpectedly (no $enddefinitions).";
        return false;
    }
    return true;
}

bool VCDParser::parseBody(std::istream& in, WaveformDatabase& db, std::string& errorMsg) {
    std::string line;
    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;

        if (line[0] == '#') {
            // timestamp
            std::string tstr = line.substr(1);
            currentTime = std::stoull(tstr);
            continue;
        }

        if (line[0] == 'b' || line[0] == 'B') {
            // vector: b1010 !
            // split into "b1010" and "!"
            std::stringstream ss(line);
            std::string vec, id;
            ss >> vec >> id;
            // vec starts with b
            std::string value = vec.substr(1); // drop 'b'
            addVectorChange(db, value, id);
            continue;
        }

        if (line[0] == 'r' || line[0] == 'R') {
            // real value "r3.14 !"
            // we store numeric string as-is
            std::stringstream ss(line);
            std::string rval, id;
            ss >> rval >> id;
            std::string value = rval.substr(1); // drop 'r'
            addVectorChange(db, value, id);
            continue;
        }

        // scalar like "0!" or "1aa" or "x!"
        // first char is value, rest is idCode
        char v = line[0];
        std::string id = line.substr(1);
        std::string value(1, v);
        addScalarChange(db, value, id);
    }

    return true;
}

bool VCDParser::parseFile(const std::string& path, WaveformDatabase& db, std::string& errorMsg) {
    std::ifstream in(path);
    if (!in.is_open()) {
        errorMsg = "Cannot open VCD file: " + path;
        return false;
    }

    currentScopePath.clear();
    currentTime = 0;
    timescaleFactorSec = 1e-9;

    if (!parseHeader(in, db, errorMsg))
        return false;

    if (!parseBody(in, db, errorMsg))
        return false;

    return true;
}
