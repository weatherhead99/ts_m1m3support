/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <SettingReader.h>
#include <boost/tokenizer.hpp>
#include <spdlog/spdlog.h>

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SettingReader& SettingReader::get() {
    static SettingReader instance;
    return instance;
}

void SettingReader::setRootPath(std::string rootPath) {
    spdlog::debug("SettingReader: setRootPath(\"{}\")", rootPath);

    auto test_dir = [rootPath](std::string dir) {
        struct stat dirstat;
        if (stat(dir.c_str(), &dirstat)) {
            throw std::runtime_error("Directory " + rootPath + "doesn't exist: " + strerror(errno));
        }
        if (!(dirstat.st_mode & (S_IFLNK | S_IFDIR))) {
            throw std::runtime_error(rootPath + " isn't directory or link");
        }
    };

    test_dir(rootPath);

    this->rootPath = rootPath;

    test_dir(getBasePath(""));
    test_dir(getSetPath(""));

    this->currentSet = "";
    this->currentVersion = "";
}

std::string SettingReader::getFilePath(std::string filename) {
    if (filename[0] == '/') return filename;
    return this->rootPath + "/" + filename;
}

void SettingReader::configure(std::string settingsToApply) {
    spdlog::debug("SettingReader: configure(\"{}\")", settingsToApply);
    currentSet = "";
    currentVersion = "";
    if (settingsToApply.find(',') != std::string::npos) {
        typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
        tokenizer tokenize(settingsToApply);
        tokenizer::iterator token = tokenize.begin();
        this->currentSet = *token;
        ++token;
        this->currentVersion = *token;
    } else {
        AliasApplicationSettings* aliasApplicationSettings = loadAliasApplicationSettings();
        for (uint32_t i = 0; i != aliasApplicationSettings->Aliases.size(); ++i) {
            Alias alias = aliasApplicationSettings->Aliases[i];
            if (alias.Name.compare(settingsToApply) == 0) {
                this->currentSet = alias.Set;
                this->currentVersion = alias.Version;
                break;
            }
        }
    }
    if (currentSet.empty()) {
        throw std::runtime_error("Cannot find configuration for " + settingsToApply + " settings");
    }

    if (currentVersion.empty()) {
        throw std::runtime_error("Empty version for " + settingsToApply + " settings");
    }
}

AliasApplicationSettings* SettingReader::loadAliasApplicationSettings() {
    spdlog::debug("SettingReader: loadAliasApplicationSettings()");
    this->aliasApplicationSettings.load(this->getBasePath("AliasApplicationSettings.xml").c_str());
    return &this->aliasApplicationSettings;
}

ForceActuatorApplicationSettings* SettingReader::loadForceActuatorApplicationSettings() {
    spdlog::debug("SettingReader: loadForceActuatorApplicationSettings()");
    this->forceActuatorApplicationSettings.load(
            this->getBasePath("ForceActuatorApplicationSettings.xml").c_str());
    return &this->forceActuatorApplicationSettings;
}

ForceActuatorSettings* SettingReader::loadForceActuatorSettings() {
    spdlog::debug("SettingReader: loadForceActuatorSettings()");
    this->forceActuatorSettings.load(this->getSetPath("ForceActuatorSettings.xml").c_str());
    return &this->forceActuatorSettings;
}

HardpointActuatorApplicationSettings* SettingReader::loadHardpointActuatorApplicationSettings() {
    spdlog::debug("SettingReader: loadHardpointActuatorApplicationSettings()");
    this->hardpointActuatorApplicationSettings.load(
            this->getBasePath("HardpointActuatorApplicationSettings.xml").c_str());
    return &this->hardpointActuatorApplicationSettings;
}

HardpointActuatorSettings* SettingReader::loadHardpointActuatorSettings() {
    spdlog::debug("SettingReader: loadHardpointActuatorSettings()");
    this->hardpointActuatorSettings.load(this->getSetPath("HardpointActuatorSettings.xml").c_str());
    return &this->hardpointActuatorSettings;
}

ILCApplicationSettings* SettingReader::loadILCApplicationSettings() {
    spdlog::debug("SettingReader: loadILCApplicationSettings()");
    this->ilcApplicationSettings.load(this->getBasePath("ILCApplicationSettings.xml").c_str());
    return &this->ilcApplicationSettings;
}

RecommendedApplicationSettings* SettingReader::loadRecommendedApplicationSettings() {
    spdlog::debug("SettingReader: loadRecommendedApplicationSettings()");
    this->recommendedApplicationSettings.load(
            this->getBasePath("RecommendedApplicationSettings.xml").c_str());
    return &this->recommendedApplicationSettings;
}

SafetyControllerSettings* SettingReader::loadSafetyControllerSettings() {
    spdlog::debug("SettingReader: loadSafetyControllerSettings()");
    this->safetyControllerSettings.load(this->getSetPath("SafetyControllerSettings.xml").c_str());
    return &this->safetyControllerSettings;
}

PositionControllerSettings* SettingReader::loadPositionControllerSettings() {
    spdlog::debug("SettingReader: loadPositionControllerSettings()");
    this->positionControllerSettings.load(this->getSetPath("PositionControllerSettings.xml").c_str());
    return &this->positionControllerSettings;
}

AccelerometerSettings* SettingReader::loadAccelerometerSettings() {
    spdlog::debug("SettingReader: loadAccelerometerSettings()");
    this->accelerometerSettings.load(this->getSetPath("AccelerometerSettings.xml").c_str());
    return &this->accelerometerSettings;
}

DisplacementSensorSettings* SettingReader::loadDisplacementSensorSettings() {
    spdlog::debug("SettingReader: loadDisplacementSensorSettings()");
    this->displacementSensorSettings.load(this->getSetPath("DisplacementSensorSettings.xml").c_str());
    return &this->displacementSensorSettings;
}

HardpointMonitorApplicationSettings* SettingReader::loadHardpointMonitorApplicationSettings() {
    spdlog::debug("SettingReader: loadHardpointMonitorApplicationSettings()");
    this->hardpointMonitorApplicationSettings.load(
            this->getBasePath("HardpointMonitorApplicationSettings.xml").c_str());
    return &this->hardpointMonitorApplicationSettings;
}

InterlockApplicationSettings* SettingReader::loadInterlockApplicationSettings() {
    spdlog::debug("SettingReader: loadInterlockApplicationSettings()");
    this->interlockApplicationSettings.load(this->getBasePath("InterlockApplicationSettings.xml").c_str());
    return &this->interlockApplicationSettings;
}

GyroSettings* SettingReader::loadGyroSettings() {
    spdlog::debug("SettingReader: loadGyroSettings()");
    this->gyroSettings.load(this->getSetPath("GyroSettings.xml").c_str());
    return &this->gyroSettings;
}

ExpansionFPGAApplicationSettings* SettingReader::loadExpansionFPGAApplicationSettings() {
    spdlog::debug("SettingReader: loadExpansionFPGAApplicationSettings()");
    this->expansionFPGAApplicationSettings.load(
            this->getBasePath("ExpansionFPGAApplicationSettings.xml").c_str());
    return &this->expansionFPGAApplicationSettings;
}

PIDSettings* SettingReader::loadPIDSettings() {
    spdlog::debug("SettingReader: loadPIDSettings()");
    this->pidSettings.load(this->getSetPath("PIDSettings.xml"));
    return &this->pidSettings;
}

InclinometerSettings* SettingReader::loadInclinometerSettings() {
    spdlog::debug("SettingReader: loadInclinometerSettings()");
    this->inclinometerSettings.load(this->getSetPath("InclinometerSettings.xml"));
    return &this->inclinometerSettings;
}

std::string SettingReader::getBasePath(std::string file) { return this->rootPath + "/Base/" + file; }

std::string SettingReader::getSetPath(std::string file) {
    return this->rootPath + "/Sets/" + this->currentSet + "/" + this->currentVersion + "/" + file;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
