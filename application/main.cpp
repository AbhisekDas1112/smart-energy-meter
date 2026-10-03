#include <iostream>
#include <fstream>
#include <unistd.h>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace chrono;

int main()
{
    const char* device = "/dev/smart_meter";

    const double ENERGY_PER_PULSE = 0.001;
    const double COST_PER_KWH = 7.0;

    unsigned long previous_pulses = 0;
    double peak_power = 0.0;

    bool first_reading = true;

    auto previous_time = steady_clock::now();

    while (true)
    {
        ifstream meter(device);

        if (!meter)
        {
            cerr << "Error: Cannot open /dev/smart_meter" << endl;
            return 1;
        }

        unsigned long pulses;

        meter >> pulses;
        meter.close();

        auto current_time = steady_clock::now();

        double elapsed_seconds =
            duration<double>(current_time - previous_time).count();

        double current_power = 0.0;

        if (first_reading)
        {
            previous_pulses = pulses;
            previous_time = current_time;
            first_reading = false;
        }
        else
        {
            unsigned long pulse_difference = 0;

            if (pulses >= previous_pulses)
            {
                pulse_difference = pulses - previous_pulses;
            }

            double energy_difference =
                pulse_difference * ENERGY_PER_PULSE;

            if (elapsed_seconds > 0)
            {
                current_power =
                    energy_difference /
                    (elapsed_seconds / 3600.0);
            }

            if (current_power > peak_power)
            {
                peak_power = current_power;
            }

            previous_pulses = pulses;
            previous_time = current_time;
        }

        double energy = pulses * ENERGY_PER_PULSE;
        double cost = energy * COST_PER_KWH;

        cout << "\033[2J\033[H";

        cout << "========================================" << endl;
        cout << "       SMART ENERGY ANALYTICS" << endl;
        cout << "========================================" << endl;

        cout << fixed << setprecision(2);

        cout << "Pulse Count       : " << pulses << endl;
        cout << "Energy Consumed   : " << energy << " kWh" << endl;
        cout << "Consumption Rate  : " << current_power << " kW" << endl;
        cout << "Peak Consumption  : " << peak_power << " kW" << endl;
        cout << "Estimated Cost    : Rs. " << cost << endl;

        cout << "========================================" << endl;

        sleep(2);
    }

    return 0;
}
