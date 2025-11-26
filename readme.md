
# Field-Oriented Control (FOC) for Permanent Magnet Synchronous Motor (PSM)

## Project Overview

This project implements a complete Field-Oriented Control (FOC) system for a permanent magnet synchronous motor in MATLAB Simulink. The implementation includes cascaded control architecture with speed regulation, coordinate transformations, and three-phase hysteresis current control.

## System Architecture

### Control Hierarchy

1. **Outer Loop: Speed Controller**
   - PI controller regulating angular velocity
   - Outputs quadrature current reference (iq)

2. **Coordinate Transformation Layer**
   - Inverse Park transformation (dq to alphabeta)
   - Inverse Clarke transformation (alphabeta to abc)
   - Generates three-phase current setpoints

3. **Inner Loop: Current Controllers**
   - Three independent hysteresis relay controllers
   - One per phase (U, V, W)
   - Two-point switching with configurable hysteresis

4. **Plant Model**
   - Mechanical subsystem with inertia and friction
   - Electrical subsystem with RL dynamics per phase

## Mathematical Foundation

### Mechanical Model

The motor mechanical dynamics follow:

```
T_net = T_motor - T_friction
T_motor = kt * iq
T_friction = B * omega
J * (domega/dt) = T_net
gamma = integral(omega)
```

Where:
- `J`: Moment of inertia [kg·m²]
- `kt`: Torque constant [Nm/A]
- `B`: Friction coefficient [Nm·s/rad]
- `omega`: Angular velocity [rad/s]
- `gamma`: Rotor angle [rad]

### Inverse Park Transformation

Transforms dq-frame to stationary alphabeta-frame:

```
i_alpha = -iq * sin(gamma)
i_beta = iq * cos(gamma)
```

Note: id is assumed to be zero for surface-mounted PMSMs.

### Inverse Clarke Transformation

Transforms alphabeta-frame to three-phase abc-frame:

```
iU = i_alpha
iV = -0.5 * i_alpha + (sqrt(3)/2) * i_beta
iW = -0.5 * i_alpha - (sqrt(3)/2) * i_beta
```

### Electrical Model

Each phase follows RL circuit dynamics:

```
U_L = U_applied - R * i
L * (di/dt) = U_L
i = integral(U_L / L)
```

Where:
- `R`: Phase resistance [Ohm]
- `L`: Phase inductance [H]
- `U_applied`: Applied voltage (saturated at ±Udc)

### Hysteresis Current Control

Two-point relay with hysteresis band:

```
if (i_error > +threshold): U_out = +Udc
if (i_error < -threshold): U_out = -Udc
```

This prevents chattering while maintaining current regulation.

## System Parameters

### Mechanical Parameters
```matlab
J = 0.001      % Moment of inertia [kg·m²]
kt = 0.2       % Torque constant [Nm/A]
B = 0.005      % Friction coefficient
```

### Electrical Parameters
```matlab
R = 1.0        % Phase resistance [Ohm]
L = 0.01       % Phase inductance [H]
Udc = 30       % DC-link voltage [V]
```

### Speed Controller Parameters
```matlab
omega_soll = 62.83  % Target speed: 2*pi*10 [rad/s]
Kp_omega = 0.02     % Proportional gain
Ki_omega = 1.0      % Integral gain
iq_max = 3.0        % Current saturation limit [A]
iq_min = -3.0       % Current saturation limit [A]
```

### Current Controller Parameters
```matlab
Hysteresis_band = ±0.1  % [A] - Prevents excessive switching
Relay_outputs = ±1      % Normalized voltage command
```

### Simulation Parameters
```matlab
Solver = ode45
StopTime = 1.5 s
MaxStep = 1e-4 s
ZeroCrossAlgorithm = Adaptive
MaxConsecutiveZCs = 10000
```

## Usage Instructions

### Building the Model

Execute the build script in MATLAB:

```matlab
build_stromregelung
```

This will:
1. Clear existing workspace and cached files
2. Create new Simulink model `psm_stromregelung.slx`
3. Configure all blocks and connections
4. Set simulation parameters
5. Save and open the model

### Running Simulation

After the model opens:

1. Click the Run button in Simulink
2. Observe the four scope outputs

### Expected Behavior

**Scope_Speed:**
- Signal 1 (omega_soll): Constant at 62.83 rad/s
- Signal 2 (omega_meas): Ramps from 0 to 62.83 rad/s
- Signal 3 (iq): Starts at ~3A, settles to ~1.6A
- Settling time: approximately 0.2-0.3 seconds

**Scope_Soll:**
- Three sinusoidal current references
- 120 degrees phase shift between phases
- Amplitude: proportional to iq (~1.6A steady-state)
- Frequency: proportional to omega

**Scope_Currents:**
- Actual phase currents tracking references
- High-frequency ripple from hysteresis switching
- Maintains sinusoidal envelope

**Scope_Gates:**
- Switching voltages: ±30V
- Variable pulse widths
- Frequency varies with current error

## Model Structure

### Block Hierarchy

```
Speed Control Layer
├── omega_soll_const (Constant)
├── Sum_omega (Comparator)
├── PI_omega (PID Controller)
└── Mechanical Model
    ├── Torque_Gain (kt)
    ├── Friction (B)
    ├── Sum_Torque (Adder)
    ├── InvJ (1/J)
    ├── omega_meas (Integrator)
    └── Gamma (Integrator)

Transformation Layer
├── sin_gamma (Trig Function)
├── cos_gamma (Trig Function)
├── prod_alpha (Product)
├── prod_beta (Product)
└── ABC Transformation
    ├── IU_SOLL (Gain)
    ├── IV_SOLL (Sum)
    └── IW_SOLL (Sum)

Current Control Layer (per phase)
├── Switch_X (Mode selection)
├── Sum_X (Error calculation)
├── Relay_X (Hysteresis controller)
├── Sat_X (Voltage limiter)
└── X_gate (Output port)

Motor Model (per phase)
├── Sum_Motor_X (Voltage sum)
├── InvL_X (1/L gain)
├── Integrator_X (Current integration)
├── K_X (Measurement gain)
└── R_X (Resistive drop)
```

## Implementation Notes

### Cache Management

The build script includes comprehensive cache clearing to prevent stale model issues:
- Closes all open block diagrams
- Clears workspace variables
- Deletes compiled model cache (.slxc)
- Removes simulation project directory (slprj)
- Deletes existing model file

### Anti-Windup

The PI speed controller includes built-in saturation limits preventing integrator windup during:
- Startup acceleration
- Current limit conditions
- Load transients

### Zero-Crossing Detection

Adaptive zero-crossing algorithm handles the discontinuous relay blocks efficiently:
- MaxConsecutiveZCs set to 10000 for reliable switching detection
- Prevents solver warnings during high-frequency switching

### Hysteresis Tuning

Current hysteresis band (±0.1A) represents a compromise:
- Larger values: Reduced switching frequency, larger current ripple
- Smaller values: Increased switching frequency, risk of chattering

## Laboratory Integration Path

### Current Status (Simulation)

- Complete FOC algorithm implementation
- Virtual motor plant models
- Software-based angle generation

### Required Modifications for Hardware

1. **Replace Ramp with Encoder (Aufgabe 4.3)**
   - Integrate DS1104ENC_POS block
   - Implement encoder zero-position calibration
   - Calculate gamma from mechanical position: gamma = p * theta + offset

2. **Replace Virtual ADC (Aufgabe 4.1)**
   - Integrate DS1104ADC_C5/C6/C7 blocks
   - Apply current sensor scaling factors
   - Implement offset compensation

3. **Replace Virtual Gates**
   - Integrate DS1104SL_DSP_BIT_OUT_C3/C4/C5 blocks
   - Configure gate driver interface
   - Add dead-time compensation if required

4. **Tune Parameters**
   - Measure actual motor parameters (R, L, kt, J, B)
   - Adjust controller gains for hardware dynamics
   - Calibrate current sensors and encoder

## Performance Characteristics

### Closed-Loop Bandwidth

Speed loop bandwidth limited by:
- Mechanical time constant: tau_mech = J/B = 0.2 s
- Current loop response time: tau_elec = L/R = 0.01 s
- PI controller gains

### Steady-State Error

- Speed regulation: Zero (integral action)
- Current tracking: Bounded by hysteresis band (±0.1A)

### Transient Response

- Rise time: ~0.15 s
- Overshoot: Minimal (<5%) with optimized gains
- Settling time: ~0.3 s

## File Structure

```
FOC_PSM/
├── build_stromregelung.m     (Build script)
├── psm_stromregelung.slx     (Generated model)
└── README.md                  (This file)
```

## References

1. Laboratory exercise documentation for FOC implementation
2. Park transformation: R.H. Park, "Two-Reaction Theory of Synchronous Machines"
3. Clarke transformation: E. Clarke, "Circuit Analysis of AC Power Systems"
4. Field-Oriented Control principles: K. Hasse, F. Blaschke

## Version History

- Current: Complete FOC with speed control and mechanical model
- Implements exercises 4.1, 4.2, and 4.4
- Exercise 4.3 (encoder integration) pending hardware availability

## Author Notes

This implementation serves as preparation for laboratory work with dSPACE hardware. The model structure intentionally mirrors the block diagram specifications provided in the laboratory manual to facilitate direct hardware integration.

