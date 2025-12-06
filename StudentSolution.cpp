#include "acequia_manager.h"
#include <iostream>
using namespace std;

void solveProblems(AcequiaManager& manager)
{
    auto canals = manager.getCanals();
    auto regions = manager.getRegions();

    // Run until solved or max time reached
    while (!manager.isSolved && manager.hour != manager.SimulationMax)
    {
        // Simple strategy:
        //  - If a region is in drought, open a canal from a region that is not.
        //  - If a region is flooded, stop sending it water.
        for (auto canal : canals)
        {
            Region* src = canal->sourceRegion;
            Region* dest = canal->destinationRegion;

            // Skip if either pointer is null (safety check)
            if (!src || !dest) continue;

            // Determine ratios (how full each region is relative to capacity)
            double srcRatio = src->waterLevel / src->waterCapacity;
            double destRatio = dest->waterLevel / dest->waterCapacity;

            // If source has plenty of water and destination needs more
            if (srcRatio > 0.6 && destRatio < 0.4)
            {
                canal->setFlowRate(0.5);     // Send water
                canal->toggleOpen(true);
            }
            // If destination is already near capacity, close canal
            else if (destRatio >= 0.9 || dest->isFlooded)
            {
                canal->toggleOpen(false);
            }
            else
            {
                canal->toggleOpen(false);    // Default: closed
            }
        }

        // Move simulation forward one hour
        manager.nexthour();
    }

    cout << "Simulation complete.\n";
}
