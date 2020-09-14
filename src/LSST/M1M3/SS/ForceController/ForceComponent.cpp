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

#include <ForceComponent.h>
#include <cstring>
#include <cmath>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ForceComponent::ForceComponent() {
    name = "UNDEFINED";

    enabled = false;
    disabling = false;
    maxRateOfChange = 15000.0;
    nearZeroValue = 10.0;

    disabling = false;
    enabled = false;

    memset(xCurrent, 0, sizeof(xCurrent));
    memset(yCurrent, 0, sizeof(yCurrent));
    memset(zCurrent, 0, sizeof(zCurrent));
    memset(xTarget, 0, sizeof(xTarget));
    memset(yTarget, 0, sizeof(yTarget));
    memset(zTarget, 0, sizeof(zTarget));
    memset(xOffset, 0, sizeof(xOffset));
    memset(yOffset, 0, sizeof(yOffset));
    memset(zOffset, 0, sizeof(zOffset));
}

ForceComponent::~ForceComponent() {}

bool ForceComponent::isEnabled() { return enabled; }
bool ForceComponent::isDisabling() { return disabling; }

void ForceComponent::enable() {
    // Enable and set the target to 0N
    spdlog::debug("{}ForceComponent: enable()", name);
    enabled = true;
    disabling = false;
    memset(xTarget, 0, sizeof(xTarget));
    memset(yTarget, 0, sizeof(yTarget));
    memset(zTarget, 0, sizeof(zTarget));
    postEnableDisableActions();
}

void ForceComponent::disable() {
    // Start disabling and driving to 0N
    spdlog::debug("{}ForceComponent: disable()", name);
    enabled = false;
    disabling = true;
    memset(xTarget, 0, sizeof(xTarget));
    memset(yTarget, 0, sizeof(yTarget));
    memset(zTarget, 0, sizeof(zTarget));
}

void ForceComponent::update() {
    if (disabling) {
        // If we are disabling we need to keep driving this force component to 0N
        // Once we are near zero we consider our action complete and that the force
        // component is actually disabled
        bool nearZero = true;
        for (int i = 0; i < 156 && nearZero; ++i) {
            if (i < 12) {
                nearZero = nearZero && xCurrent[i] < nearZeroValue && xCurrent[i] > -nearZeroValue;
            }

            if (i < 100) {
                nearZero = nearZero && yCurrent[i] < nearZeroValue && yCurrent[i] > -nearZeroValue;
            }

            nearZero = nearZero && zCurrent[i] < nearZeroValue && zCurrent[i] > -nearZeroValue;
        }
        if (nearZero) {
            spdlog::debug("{}ForceComponent: disabled()", name);
            disabling = false;
            enabled = false;
            memset(xCurrent, 0, sizeof(xCurrent));
            memset(yCurrent, 0, sizeof(yCurrent));
            memset(zCurrent, 0, sizeof(zCurrent));
            postEnableDisableActions();
            postUpdateActions();
        }
    }
    if (enabled || disabling) {
        // If this force component is enabled then we need to keep trying
        // to drive this force component to it's target value.
        // To do this we need to find the vector with the largest delta
        // and scale all other vectors based off how long it will take to
        // drive that delta to 0N.
        float largestDelta = 0.0;
        for (int i = 0; i < 156; ++i) {
            if (i < 12) {
                xOffset[i] = xTarget[i] - xCurrent[i];
                if (std::abs(xOffset[i]) > largestDelta) {
                    largestDelta = std::abs(xOffset[i]);
                }
            }

            if (i < 100) {
                yOffset[i] = yTarget[i] - yCurrent[i];
                if (std::abs(yOffset[i]) > largestDelta) {
                    largestDelta = std::abs(yOffset[i]);
                }
            }

            zOffset[i] = zTarget[i] - zCurrent[i];
            if (std::abs(zOffset[i]) > largestDelta) {
                largestDelta = std::abs(zOffset[i]);
            }
        }
        // Determine how many outer loop cycles it will take to drive the
        // largest delta to 0N and use that as a scalar for all other
        // actuator deltas.
        float scalar = largestDelta / maxRateOfChange;
        if (scalar > 1) {
            // If it is more than 1 outer loop cycle keep working, we aren't
            // then we need to keep working!
            for (int i = 0; i < 156; ++i) {
                if (i < 12) {
                    xOffset[i] /= scalar;
                    xCurrent[i] += xOffset[i];
                }

                if (i < 100) {
                    yOffset[i] /= scalar;
                    yCurrent[i] += yOffset[i];
                }

                zOffset[i] /= scalar;
                zCurrent[i] += zOffset[i];
            }
        } else {
            // If it is less than 1 outer loop cycle just set current as the target
            // we do this to prevent rounding errors from making it so when we
            // request 100N we don't put 99.998N and claim that is what we where asked
            // to produce.
            for (int i = 0; i < 156; ++i) {
                if (i < 12) {
                    xCurrent[i] = xTarget[i];
                }

                if (i < 100) {
                    yCurrent[i] = yTarget[i];
                }

                zCurrent[i] = zTarget[i];
            }
        }
        postUpdateActions();
    }
}

void ForceComponent::reset() {
    disabling = false;
    enabled = false;
    postEnableDisableActions();

    memset(xCurrent, 0, sizeof(xCurrent));
    memset(yCurrent, 0, sizeof(yCurrent));
    memset(zCurrent, 0, sizeof(zCurrent));
    memset(xTarget, 0, sizeof(xTarget));
    memset(yTarget, 0, sizeof(yTarget));
    memset(zTarget, 0, sizeof(zTarget));
    memset(xOffset, 0, sizeof(xOffset));
    memset(yOffset, 0, sizeof(yOffset));
    memset(zOffset, 0, sizeof(zOffset));
    postUpdateActions();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
