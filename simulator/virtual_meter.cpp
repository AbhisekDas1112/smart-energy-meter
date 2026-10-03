#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
    const char* device = "/dev/smart_meter";

    // Each pulse represents 0.001 kWh
    const double ENERGY_PER_PULSE = 0.001;

    unsigned long pulses = 0;

    srand(time(nullptr));

    cout << "=====================================" << endl;
    cout << "     VIRTUAL SMART METER SIMULATOR" << endl;
    cout << "=====================================" << endl;
    cout << "Generating realistic meter pulses..." << endl;

    while (true)
    {
        // Simulate power usage between 1 and 3 kW
        double power = 1.0 + (rand() % 200) / 100.0;

        // Time required for one pulse at this power
        double pulse_time =
            (ENERGY_PER_PULSE / power) * 3600.0;

        pulses++;

        ofstream meter(device);

        if (!meter)
        {
            cerr << "Error: Cannot open /dev/smart_meter" << endl;
            return 1;
        }

        meter << pulses;
        meter.close();

        cout << "Pulse: " << pulses
             << " | Simulated Power: "
             << power << " kW" << endl;

        usleep(static_cast<useconds_t>(pulse_time * 1000000));
    }

    return 0;
}
