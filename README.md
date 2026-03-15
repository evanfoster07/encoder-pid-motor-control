# Motor Speed Control System
Embedded motor control system implemented on Arduino using PID feedback, encoder-based speed measurement, and real-time display output.

## Overview
This project implements a closed-loop control system to regulate the rotational speed of a DC motor. The system uses quadrature encoder feedback to measure motor speed and applies a PID controller to adjust PWM output in real time.
A rotary encoder allows the user to adjust the target speed interactively, while an OLED display provides live feedback showing:

- Target speed
- Measured motor speed
- Moving-average speed over a 1-second window

The goal of the project was to explore embedded control systems, real-time feedback, and hardware/software interaction.

## Features
- Closed-loop motor speed control using a PID controller
- Interrupt-driven encoder feedback for accurate speed measurement
- Rotary encoder input for adjustable speed setpoint
- Feedforward compensation to overcome motor deadband
- Moving-average tracking for stable speed display
- OLED interface for real-time system monitoring

## Hardware
- Arduino Uno microcontroller (or equivalent)
- DC motor with quadrature encoder
- Rotary encoder for speed input
- SH1106 OLED display (128x64)
- L293D motor driver

## Software Architecture
The firmware is organized into modular components:

### Motor Class
Handles motor control and encoder processing:
- Direction control
- PWM output
- Encoder interrupt handling
- Speed calculation

### PID Controller
Implements a configurable PID controller used to compute the control signal based on target and measured speed.

### Control Loop
A timed control loop runs periodically to:
- Read motor speed
- Compute PID correction
- Apply feedforward compensation
- Update the motor PWM signal

### Display Interface 
Updates the OLED display periodically with system telemetry.

## Control Strategy
The controller uses PID feedback:

`control = PID(target_speed − measured_speed)`

combined with feedforward compensation to overcome motor friction and deadband effects.
This allows the controller to respond quickly to changes in target speed while maintaining stable operation.

## Learning Highlights
Key concepts explored during this project:

- Interrupt-driven sensor feedback
- Embedded control loop timing
- PID controller tuning
- Handling motor deadband and nonlinearities
- Embedded system modularization
- Real-time display interfaces

## Future Improvements & Applications
Potential extensions for the system:

- Automatic PID tuning
- Improved feedforward modeling
- Faster steady-state settling time
- Acceleration limiting
- Serial telemetry for data logging

Possible applications include: 

- Mobile robotics requiring accurate speed control
- Automation systems requiring stable motor speed
- Closed-loop fan or pump speed regulation