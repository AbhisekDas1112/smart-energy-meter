# Smart Energy Smart-Meter Pulse Counter & Analytics Agent

## Domain

Domain 1 – IoT, Embedded & Virtual Sensors

## Project Overview

The Smart Energy Smart-Meter Pulse Counter & Analytics Agent is a Linux-based software project that simulates a smart electricity meter and analyzes energy consumption.

The system uses a virtual meter to generate pulse data. The pulse data is transferred through a Linux character device driver and accessed using `/dev/smart_meter`.

A C++ analytics application reads the meter data and calculates energy consumption, current power consumption, peak power consumption, and estimated electricity cost.

## Objectives

- Simulate a smart electricity meter.
- Generate virtual energy-meter pulses.
- Implement a Linux character device driver.
- Create the `/dev/smart_meter` device.
- Read meter data using C++.
- Calculate energy consumption.
- Calculate power consumption.
- Calculate peak consumption.
- Estimate electricity cost.
- Demonstrate Linux device-driver concepts and software architecture.

## System Architecture

```text
Virtual Meter Simulator
          |
          | Pulse Count
          v
Linux Character Device Driver
          |
          v
   /dev/smart_meter
          |
          | Meter Data
          v
Energy Analytics Application
          |
          v
 Energy / Power / Peak / Cost

Project Components:

1. Linux Character Device Driver
The driver is written in C and provides the character device:
/dev/smart_meter

It maintains the meter pulse count and supports read/write operations.

2. Virtual Meter Simulator
The simulator is written in C++.
It generates virtual meter pulses and simulates different power-consumption levels.
The simulated power is approximately between:
1 kW and 3 kW

3. Energy Analytics Application
The analytics application is written in C++.
It reads the pulse count and calculates:
- Energy consumed
- Current power consumption
- Peak power consumption
- Estimated electricity cost

Calculation:

Energy:
Energy Consumed = Pulse Count × Energy Per Pulse

The project uses:
Energy Per Pulse = 0.001 kWh

Electricity Cost:
Estimated Cost = Energy Consumed × Cost Per kWh

The project uses:
Cost Per kWh = Rs. 7

Power:
Power = Energy Difference / Time Difference

Technologies Used:-
- C
- C++
- Ubuntu Linux
- Linux Character Device Driver
- GCC
- G++
- GNU Make
- Git
- GitHub

Project Structure:

smart-energy-meter/
├── application/
│   ├── main.cpp
│   └── Makefile
├── driver/
│   ├── smart_meter.c
│   └── Makefile
├── simulator/
│   ├── virtual_meter.cpp
│   └── Makefile
├── docs/
│   └── project_documentation.md
└── README.md


Build Instructions:

1. Build the Linux Driver
cd driver
make

This creates the kernel module:
smart_meter.ko

2. Load the Driver
sudo insmod smart_meter.ko

Verify:
lsmod | grep smart_meter

Check the device:
ls -l /dev/smart_meter

3. Build the Simulator

Open another terminal:
cd ~/Desktop/smart-energy-meter/simulator
make

4. Build the Analytics Application

Open another terminal:
cd ~/Desktop/smart-energy-meter/application
make

Running the Project:

Terminal 1 - Start Analytics:

cd ~/Desktop/smart-energy-meter/application
sudo ./smart_energy

Terminal 2 - Start Virtual Meter:

cd ~/Desktop/smart-energy-meter/simulator
sudo ./virtual_meter

The simulator generates pulse data while the analytics application reads the data from:
/dev/smart_meter

Example Output:-

Virtual Meter:
Pulse: 25 | Simulated Power: 1.75 kW
Pulse: 26 | Simulated Power: 2.15 kW
Pulse: 27 | Simulated Power: 1.40 kW

Analytics Application:
Pulse Count       : 23
Energy Consumed   : 0.02 kWh
Consumption Rate  : 1.80 kW
Peak Consumption  : 3.60 kW
Estimated Cost    : Rs. 0.16

The exact values vary depending on the simulated meter activity.
