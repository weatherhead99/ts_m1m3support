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

#ifndef POSITIONCONTROLLERSETTINGS_H_
#define POSITIONCONTROLLERSETTINGS_H_

#include <SAL_MTM1M3.h>
#include <M1M3SSPublisher.h>

#include <DataTypes.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Hardpoints/position controller settings. Settings are loaded from
 * PositionControllerSettings in actually used setting set.
 *
 * @see StartCommand
 */
class PositionControllerSettings : public MTM1M3_logevent_positionControllerSettingsC {
public:
    void load(const std::string &filename);

    void log() { M1M3SSPublisher::get().logPositionControllerSettings(this); }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* POSITIONCONTROLLERSETTINGS_H_ */
