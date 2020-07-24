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

#include <OfflineState.h>
#include <Model.h>
#include <DigitalInputOutput.h>
#include <PowerController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

OfflineState::OfflineState(M1M3SSPublisher* publisher) : State(publisher, "OfflineState") {}

States::Type OfflineState::update(UpdateCommand* command, Model* model) {
    return rejectCommandInvalidState(command, "Update");
}

States::Type OfflineState::enterControl(EnterControlCommand* command, Model* model) {
    spdlog::info("OfflineState: enterControl()");
    model->publishRecommendedSettings();
    // model->getDigitalInputOutput()->turnAirOff();
    // model->getDigitalInputOutput()->turnCellLightsOff();
    model->getDigitalInputOutput()->turnCellLightsOn();
    model->getDigitalInputOutput()->turnAirOn();
    // TODO: May need to change power controller to act like digital input output
    // model->getPowerController()->setBothPowerNetworks(false);
    return States::StandbyState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
