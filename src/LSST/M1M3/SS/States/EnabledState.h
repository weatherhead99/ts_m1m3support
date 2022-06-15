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

#ifndef ENABLEDSTATE_H_
#define ENABLEDSTATE_H_

#include <State.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Parent class for all enabled sub-states. Enabled state is defined in SAL,
 * this class provides basic functionality for it. Subclasses needs to
 * implement update method (and shall implement commands methods).
 */
class EnabledState : public State {
public:
    EnabledState(std::string name);

    virtual States::Type storeTMAAzimuthSample(TMAAzimuthSampleCommand* command) override;
    virtual States::Type storeTMAElevationSample(TMAElevationSampleCommand* command) override;
    virtual States::Type setAirSlewFlag(SetAirSlewFlagCommand* command) override;

protected:
    /**
     * Actions to be performed during a loop in enabled sub-state. Calculate
     * forces, send updates, collects data needed for telemetry.
     */
    void runLoop();

    /**
     * Collects and sends telemetry.
     */
    void sendTelemetry();

    /**
     * Query mirror raising completion.
     *
     * @return true if mirror is raised
     */
    bool raiseCompleted();

    /**
     * Query mirror lowering completion.
     *
     * @return true if mirror is lowered
     */
    bool lowerCompleted();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ENABLEDSTATE_H_ */
