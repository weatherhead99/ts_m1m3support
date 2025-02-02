/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#ifndef ILCMESSAGEFACTORY_H_
#define ILCMESSAGEFACTORY_H_

#include <DataTypes.h>
#include <ModbusBuffer.h>

namespace LSST {
namespace M1M3 {
namespace SS {

struct ILCApplicationSettings;

class ILCMessageFactory {
public:
    ILCMessageFactory(ILCApplicationSettings* ilcApplicationSettings);

    void reportServerID(ModbusBuffer* buffer, uint8_t address);

    /**
     * Calls ILC function 18 (0x12) (ReportServerStatus). Retrieves ILC
     * failures bits.
     *
     * @param buffer ModbusBuffer where request will be written
     * @param address ILC address
     */
    void reportServerStatus(ModbusBuffer* buffer, uint8_t address);
    void changeILCMode(ModbusBuffer* buffer, uint8_t address, uint16_t mode);
    void reportILCMode(ModbusBuffer* buffer, uint8_t address);
    void broadcastStepMotor(ModbusBuffer* buffer, uint8_t broadcastCounter, int8_t* steps);
    void unicastStepMotor(ModbusBuffer* buffer, uint8_t address, int8_t steps);
    void electromechanicalForceAndStatus(ModbusBuffer* buffer, uint8_t address);
    void broadcastElectromechanicalFreezeSensorValues(ModbusBuffer* buffer, uint8_t broadcastCounter);
    void broadcastPneumaticFreezeSensorValues(ModbusBuffer* buffer, uint8_t broadcastCounter);
    void setBoostValveDCAGains(ModbusBuffer* buffer, uint8_t address, float primaryGain, float secondaryGain);
    void readBoostValveDCAGains(ModbusBuffer* buffer, uint8_t address);
    void broadcastForceDemand(ModbusBuffer* buffer, uint8_t broadcastCounter, bool slewFlag,
                              int32_t* saaPrimarySetpoint, int32_t* daaPrimarySetpoint,
                              int32_t* daaSecondarySetpoint);
    void unicastForceDemand(ModbusBuffer* buffer, uint8_t address, bool slewFlag, int32_t primarySetpoint,
                            int32_t secondarySetpoint);
    void unicastSingleAxisForceDemand(ModbusBuffer* buffer, uint8_t address, bool slewFlag,
                                      int32_t primarySetpoint);
    void unicastDualAxisForceDemand(ModbusBuffer* buffer, uint8_t address, bool slewFlag,
                                    int32_t primarySetpoint, int32_t secondarySetpoint);

    /**
     * Calls ILC function 76 (0x4C) Pneumatic ILC - Force [N] and Status
     * Request. This retrieves forces measured on ILC.
     *
     * @param buffer ModbusBuffer where request will be written
     * @param address ILC address
     */
    void pneumaticForceStatus(ModbusBuffer* buffer, uint8_t address);
    void setADCScanRate(ModbusBuffer* buffer, uint8_t address, uint8_t rate);
    void reportADCScanRate(ModbusBuffer* buffer, uint8_t address);
    void setADCChannelOffsetAndSensitivity(ModbusBuffer* buffer, uint8_t address, uint8_t channel,
                                           float offset, float sensitivity);
    void reset(ModbusBuffer* buffer, uint8_t address);
    void readCalibration(ModbusBuffer* buffer, uint8_t address);
    void reportDCAPressure(ModbusBuffer* buffer, uint8_t address);
    void reportDCAID(ModbusBuffer* buffer, uint8_t address);
    void reportDCAStatus(ModbusBuffer* buffer, uint8_t address);
    void reportLVDT(ModbusBuffer* buffer, uint8_t address);
    void nopReportLVDT(ModbusBuffer* buffer, uint8_t address);

private:
    ILCApplicationSettings* _ilcApplicationSettings;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ILCMESSAGEFACTORY_H_ */
