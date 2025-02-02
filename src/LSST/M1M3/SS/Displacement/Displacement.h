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

#ifndef DISPLACEMENT_H_
#define DISPLACEMENT_H_

#include <DataTypes.h>
#include <DisplacementSensorSettings.h>
#include <SupportFPGAData.h>
#include <SafetyController.h>

struct MTM1M3_imsDataC;
struct MTM1M3_logevent_displacementSensorWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * The class used to process displacement data.
 */
class Displacement {
public:
    /*!
     * Instantiates the displacement sensor.
     * @param[in] displacementSensorSettings The displacement settings.
     * @param[in] _fpgaData The fpga data.
     * @param[in] safetyController The safety controller.
     */
    Displacement(DisplacementSensorSettings* displacementSensorSettings, SupportFPGAData* fpgaData,
                 SafetyController* safetyController);

    /*!
     * Processes currently available displacement sensor data and publish it.
     */
    void processData();

private:
    DisplacementSensorSettings* _displacementSensorSettings;
    SupportFPGAData* _fpgaData;
    SafetyController* _safetyController;

    MTM1M3_imsDataC* _imsData;
    MTM1M3_logevent_displacementSensorWarningC* _displacementWarning;

    uint64_t _lastSampleTimestamp;
    uint64_t _lastErrorTimestamp;
    bool _errorCleared;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DISPLACEMENT_H_ */
