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

#include <RaisingEngineeringState.h>
#include <Model.h>
#include <ModelPublisher.h>
#include <SafetyController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

RaisingEngineeringState::RaisingEngineeringState() : EnabledState("RaisingEngineeringState") {}

States::Type RaisingEngineeringState::update(UpdateCommand* command) {
    ModelPublisher publishModel{};
    SPDLOG_TRACE("RaisingEngineeringState: update()");
    Model::get().getMirrorRaiseController()->runLoop();
    runLoop();
    return Model::get().getSafetyController()->checkSafety(raiseCompleted() ? States::ActiveEngineeringState
                                                                            : States::NoStateTransition);
}

States::Type RaisingEngineeringState::abortRaiseM1M3(AbortRaiseM1M3Command* command) {
    SPDLOG_INFO("RaisingEngineeringState: abortRaiseM1M3()");
    Model::get().getMirrorLowerController()->abortRaiseM1M3();
    return Model::get().getSafetyController()->checkSafety(States::LoweringEngineeringState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
