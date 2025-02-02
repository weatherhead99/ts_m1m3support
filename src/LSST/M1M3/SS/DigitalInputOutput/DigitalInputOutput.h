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

#ifndef DIGITALINPUTOUTPUT_H_
#define DIGITALINPUTOUTPUT_H_

#include <DataTypes.h>
#include <IFPGA.h>
#include <SafetyController.h>

#include <chrono>

struct MTM1M3_logevent_airSupplyStatusC;
struct MTM1M3_logevent_airSupplyWarningC;
struct MTM1M3_logevent_cellLightStatusC;
struct MTM1M3_logevent_cellLightWarningC;
struct MTM1M3_logevent_interlockStatusC;
struct MTM1M3_logevent_interlockWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * The class used to process digital inputs and outputs.
 */
class DigitalInputOutput {
public:
    /*!
     * Instantiates the accelerometer.
     */
    DigitalInputOutput();

    /*!
     * Sets the safety controller.
     * @param[in] safetyController The safety controller.
     */
    void setSafetyController(SafetyController* safetyController);

    /*!
     * Processes currently available digital input and output data and publish it.
     */
    void processData();

    /**
     * @brief Toggles the heartbeat signal if a configured amount of time has passed.
     *
     * Both Global Interlock System and software (logevent) heartbeats are
     * triggered.
     */
    void tryToggleHeartbeat();

    /*!
     * Turns the air on.
     */
    void turnAirOn();
    /*!
     * Turns the air off.
     */
    void turnAirOff();
    /*!
     * Turns the cell lights on.
     */
    void turnCellLightsOn();
    /*!
     * Turns the cell lights off.
     */
    void turnCellLightsOff();

private:
    SafetyController* _safetyController;

    MTM1M3_logevent_airSupplyStatusC* _airSupplyStatus;
    MTM1M3_logevent_airSupplyWarningC* _airSupplyWarning;
    MTM1M3_logevent_cellLightStatusC* _cellLightStatus;
    MTM1M3_logevent_cellLightWarningC* _cellLightWarning;
    MTM1M3_logevent_interlockStatusC* _interlockStatus;

    uint64_t _lastDOTimestamp;
    uint64_t _lastDITimestamp;

    double _lastToggleTimestamp;

    std::chrono::steady_clock::time_point _airToggledTime;
    std::chrono::steady_clock::time_point _lightToggledTime;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DIGITALINPUTOUTPUT_H_ */
