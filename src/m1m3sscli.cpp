/*
 * Command line Thermal System client.
 *
 * Developed for the Vera C. Rubin Observatory Telescope & Site Software Systems.
 * This product includes software developed by the Vera C.Rubin Observatory Project
 * (https://www.lsst.org). See the COPYRIGHT file at the top-level directory of
 * this distribution for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifdef SIMULATOR
#include <SimulatedFPGA.h>
#define FPGAClass SimulatedFPGA
#else
#include <FPGA.h>
#define FPGAClass LSST::M1M3::SS::FPGA
#endif

#include <FPGA.h>
#include <ForceActuatorApplicationSettings.h>

#include <cRIO/FPGACliApp.h>
#include <cRIO/ElectromechanicalPneumaticILC.h>
#include <cRIO/PrintILC.h>

#include <iostream>
#include <iomanip>

#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using namespace LSST::cRIO;
using namespace LSST::M1M3::SS;

class M1M3SScli : public FPGACliApp {
public:
    M1M3SScli(const char* name, const char* description);
    int setPower(command_vec cmds);

protected:
    virtual LSST::cRIO::FPGA* newFPGA(const char* dir) override;
    virtual ILCUnits getILCs(command_vec cmds) override;
};

class PrintElectromechanical : public ElectromechanicalPneumaticILC, public PrintILC {
public:
    PrintElectromechanical(uint8_t bus) : ILC(bus), ElectromechanicalPneumaticILC(bus), PrintILC(bus) {}

protected:
    void processHardpointForceStatus(uint8_t address, uint8_t status, int32_t encoderPostion,
                                     float loadCellForce) override;

    void processCalibrationData(uint8_t address, float mainADCK[4], float mainOffset[4],
                                float mainSensitivity[4], float backupADCK[4], float backupOffset[4],
                                float backupSensitivity[4]) override;

    void processMezzaninePressure(uint8_t address, float primaryPush, float primaryPull, float secondaryPush,
                                  float secondaryPull) override;
};

class PrintSSFPGA : public FPGAClass {
public:
#ifdef SIMULATOR
    PrintSSFPGA() : SimulatedFPGA() {}
#else
    PrintSSFPGA() : LSST::M1M3::SS::FPGA() {}
#endif

    void writeCommandFIFO(uint16_t* data, size_t length, uint32_t timeout) override;
    void writeRequestFIFO(uint16_t* data, size_t length, uint32_t timeout) override;
    void readU16ResponseFIFO(uint16_t* data, size_t length, uint32_t timeout) override;
};

M1M3SScli::M1M3SScli(const char* name, const char* description) : FPGACliApp(name, description) {
    addCommand("power", std::bind(&M1M3SScli::setPower, this, std::placeholders::_1), "i", NEED_FPGA, "<0|1>",
               "Power off/on ILC bus");

    addILCCommand(
            "calibration",
            [](ILCUnit u) {
                std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->reportCalibrationData(u.second);
            },
            "Read calibration data");

    addILCCommand(
            "pressure",
            [](ILCUnit u) {
                std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->reportMezzaninePressure(u.second);
            },
            "Read mezzanine pressure");

    addILCCommand(
            "hardpoint-force",
            [](ILCUnit u) {
                std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->reportHardpointForceStatus(
                        u.second);
            },
            "Read hardpoint info");

    addILC(std::make_shared<PrintElectromechanical>(1));
    addILC(std::make_shared<PrintElectromechanical>(2));
    addILC(std::make_shared<PrintElectromechanical>(3));
    addILC(std::make_shared<PrintElectromechanical>(4));
    addILC(std::make_shared<PrintElectromechanical>(5));
}

int M1M3SScli::setPower(command_vec cmds) {
    uint16_t net = 1;
    uint16_t aux = 0;
    switch (cmds.size()) {
        case 0:
            break;
        case 1:
            net = aux = CliApp::onOff(cmds[0]);
            break;
        case 2:
            net = CliApp::onOff(cmds[0]);
            aux = CliApp::onOff(cmds[1]);
            break;
        default:
            std::cerr << "Invalid number of arguments to power command." << std::endl;
            return -1;
    }
    uint16_t pa[16] = {65, aux, 66, aux, 67, aux, 68, aux, 69, net, 70, net, 71, net, 72, net};
    getFPGA()->writeCommandFIFO(pa, 16, 0);
    return 0;
}

LSST::cRIO::FPGA* M1M3SScli::newFPGA(const char* dir) { return new PrintSSFPGA(); }

constexpr int ILC_BUS = 5;

ForceActuatorApplicationSettings forceActuators;

ILCUnits M1M3SScli::getILCs(command_vec cmds) {
    ILCUnits units;
    int ret = -2;

    auto getBus = [](std::string b) -> int {
        if (b.length() != 1) {
            throw std::runtime_error("Invalid bus length - expected one character, got " + b);
        }
        if (b[0] >= 'A' && b[0] <= 'E') {
            return b[0] - 'A';
        }
        if (b[0] >= 'a' && b[0] <= 'e') {
            return b[0] - 'a';
        }
        if (b[0] >= '1' && b[0] <= '5') {
            return b[0] - '1';
        }
        throw std::runtime_error("Invalid bus name: " + b);
    };

    for (auto c : cmds) {
        size_t division = c.find('/');
        int bus = -1;
        int address = -1;
        try {
            if (division != std::string::npos) {
                bus = getBus(c.substr(0, division));
                std::string add_s = c.substr(division + 1);
                if (add_s == "*") {
                    for (int i = 0; i < 156; i++) {
                        ForceActuatorTableRow row = forceActuators.Table[i];
                        if (row.Subnet == bus + 1) {
                            units.push_back(ILCUnit(getILC(row.Subnet - 1), row.Address));
                        }
                    }
                    continue;
                }
                address = std::stoi(add_s);
                if (address <= 0 || address > 46) {
                    std::cerr << "Invalid address " << c << std::endl;
                    continue;
                }
            }
            // try to find bus & address by FA/HP ID
            else {
                int id = std::stoi(c);
                if (id >= 101 && id <= 443) {
                    id = forceActuators.ActuatorIdToZIndex(id);
                    if (id < 0) {
                        std::cerr << "Unknown actuator ID " << c << std::endl;
                        ret = -1;
                        continue;
                    }
                    ForceActuatorTableRow row = forceActuators.Table[id];
                    if (getDebugLevel() > 1) {
                        std::cout << "Id: " << id << " address: " << std::to_string(bus) << "/"
                                  << std::to_string(address) << std::endl;
                    }
                    bus = row.Subnet - 1;
                    address = row.Address;
                }
            }
        } catch (std::logic_error& e) {
            std::cerr << "Non-numeric address: " << c << std::endl;
        }

        if (bus < 0 || bus >= ILC_BUS || address == -1) {
            std::cerr << "Invalid ILC address: " << c << std::endl;
            ret = -1;
            continue;
        }

        units.push_back(ILCUnit(getILC(bus), address));
    }

    if (ret == -2 && units.empty()) {
        std::cout << "Command for all ILC" << std::endl;
        for (int i = 0; i < 156; i++) {
            ForceActuatorTableRow row = forceActuators.Table[i];
            units.push_back(ILCUnit(getILC(row.Subnet - 1), row.Address));
        }
        for (int i = 1; i < 7; i++) {
            units.push_back(ILCUnit(getILC(4), i));
        }
        ret = 0;
    }

    return units;
}

unsigned int _printout = 0;

void _printSepline() {
    if (_printout > 0) {
        std::cout << std::endl;
    }
    _printout++;
}

template <typename t>
void print4(const char* name, t a[4]) {
    std::cout << std::setfill(' ') << std::setw(18) << name;
    for (int i = 0; i < 4; i++) {
        std::cout << " " << std::setw(15) << std::setprecision(10) << a[i];
    }
    std::cout << std::endl;
}

void PrintElectromechanical::processHardpointForceStatus(uint8_t address, uint8_t status,
                                                         int32_t encoderPostion, float loadCellForce) {
    _printSepline();

    auto limitSwitch = [](bool sw) { return (sw ? "CLOSED" : "OPEN"); };

    std::cout << "Status: 0x" << std::setfill('0') << std::setw(2) << std::hex << status << std::dec
              << " (ILC: " << (status & 0x80 ? "FAULT" : "OK") << ", Limit Switch CW "
              << limitSwitch(status & 0x20) << ", Limit Switch CCW " << limitSwitch(status & 0x10) << ")"
              << std::endl;
    std::cout << "Encoder Position: " << encoderPostion << std::endl;
    std::cout << "Load Cell Force: " << std::setprecision(2) << std::fixed << loadCellForce << " N"
              << std::endl;
}

void PrintElectromechanical::processCalibrationData(uint8_t address, float mainADCK[4], float mainOffset[4],
                                                    float mainSensitivity[4], float backupADCK[4],
                                                    float backupOffset[4], float backupSensitivity[4]) {
    _printSepline();
    std::cout << "Calibration data " << std::to_string(getBus()) << "/" << std::to_string(address)
              << std::endl;

    int vi[4] = {1, 2, 3, 4};
    print4("Values", vi);

    print4("Main ADC Kn", mainADCK);
    print4("Main Offset", mainOffset);
    print4("Main Sensitivity", mainSensitivity);

    print4("Backup ADC Kn", backupADCK);
    print4("Backup Offset", backupOffset);
    print4("Backup Sensitivity", backupSensitivity);
}

void PrintElectromechanical::processMezzaninePressure(uint8_t address, float primaryPush, float primaryPull,
                                                      float secondaryPush, float secondaryPull) {
    _printSepline();
    std::cout << "Pressure data " << std::to_string(getBus()) << "/" << std::to_string(address) << std::endl;

    auto printPushPull = [](const char* name, float push, float pull) {
        std::cout << std::setfill(' ') << std::setw(10) << name << ": " << std::setw(8)
                  << std::setprecision(2) << std::fixed << push << " | " << std::setw(8)
                  << std::setprecision(2) << std::fixed << push << std::endl;
    };

    printPushPull("Primary", primaryPush, primaryPull);
    printPushPull("Secondary", secondaryPush, secondaryPull);
}

M1M3SScli cli("M1M3SS", "M1M3 Support System Command Line Interface");

void _printBuffer(std::string prefix, uint16_t* buf, size_t len) {
    if (cli.getDebugLevel() == 0) {
        return;
    }

    std::cout << prefix;
    for (size_t i = 0; i < len; i++) {
        std::cout << std::hex << std::setfill('0') << std::setw(4) << buf[i] << " ";
    }
    std::cout << std::endl;
}

void PrintSSFPGA::writeCommandFIFO(uint16_t* data, size_t length, uint32_t timeout) {
    _printBuffer("C> ", data, length);
    FPGAClass::writeCommandFIFO(data, length, timeout);
}

void PrintSSFPGA::writeRequestFIFO(uint16_t* data, size_t length, uint32_t timeout) {
    _printBuffer("R> ", data, length);
    FPGAClass::writeRequestFIFO(data, length, timeout);
}

void PrintSSFPGA::readU16ResponseFIFO(uint16_t* data, size_t length, uint32_t timeout) {
    FPGAClass::readU16ResponseFIFO(data, length, timeout);
    _printBuffer("R< ", data, length);
}

#if 0

int calSet(command_vec cmds) {
    return callFunction(cmds, [](uint8_t bus, uint8_t address, command_vec::iterator& c, command_vec cmds) {
        uint8_t channel = std::stoi(*(++c));
        float offset = std::stof(*(++c));
        float sensitivity = std::stof(*(++c));
        return ilcs[bus].setOffsetAndSensitivity(address, channel, offset, sensitivity);
    });
}

#endif

int main(int argc, char* const argv[]) { return cli.run(argc, argv); }
