# Industrial_Motor_fault_Detection_system

### Build the Embedded System using ATmega328P

---

## 📌 Project Overview

This project is a **DC Motor Fault Detection & Protection System** designed using **ATmega328P microcontroller** and **Embedded C programming**.

The system continuously monitors:

✅ Motor Temperature  
✅ Motor Current  
✅ Fault Conditions  

and automatically protects the motor from:

⚠️ Overload Faults  
⚠️ Overheating Conditions

This project simulates **real industrial motor protection logic** used in:

- Industrial Automation
- Conveyor Systems
- Pump Protection
- Manufacturing Equipment
- Motor Monitoring Systems

---

# 📷 Project Preview

## Complete Project Setup

<img width="400" height="250" alt="image" src="https://github.com/user-attachments/assets/1b1b3c06-9593-4033-b743-fa44cbcec58b" />


---

## Proteus Simulation

<img width="400" height="250" alt="Screenshot 2026-05-19 135435" src="https://github.com/user-attachments/assets/8ea2b342-d5e3-4bb9-83a1-02aa4496e8e6" />


---

## Serial Monitor Output


<img width="400" height="250" alt="image" src="https://github.com/user-attachments/assets/daea3c46-9b68-4db0-9412-08d8bd8f941d" />



---

---

# 🎯 Features

✔ Bare-Metal Programming (No readymate Boards)

✔ Real-Time Current Monitoring

✔ Real-Time Temperature Monitoring

✔ Relay Based Motor Protection

✔ LED Status Indication

✔ UART Serial Monitoring

✔ Industrial Fault Logic Implementation

✔ Embedded C Register Programming

---

# ⚙️ Hardware Components Used

| Component | Purpose |
|-----------|----------|
| ATmega328P | Main Microcontroller |
| SMPS | 220V to 12V Generation for motor input |
| LM35 | Temperature Sensor |
| ACS712 | Current Sensor |
| Relay Module | Motor Power Cutoff |
| RS-555 DC Motor | Test Motor |
| LEDs | Status Indication |
| USBASP | Code Uploading |
| Crystal Oscillator | Clock Generation |
| Capacitors | Stability / Filtering |
| Power Supply | System Power |

---

# 🔥 Working Principle

The system continuously monitors:

### 1️⃣ Temperature Monitoring

LM35 senses motor temperature.

LM35 Output Equation:

```math
Temperature = Voltage × 100
```

Example:

```text
30°C → 0.30V
60°C → 0.60V
```

---

### 2️⃣ Current Monitoring

ACS712 monitors motor current.

Current Equation:

```math
Current=(Vout−2.5)/0.185
```

Where:

- 2.5V → Offset Voltage
- 0.185V/A → Sensor Sensitivity

---

### 3️⃣ Fault Detection Logic

#### Normal Condition

```text
Current < Threshold
Temperature < Threshold
```

Result:

✅ Green LED ON

✅ Relay ON

✅ Motor Running

---

#### Overload Condition

```text
Current ≥ Threshold
```

Result:

🔴 Red LED ON

🔴 Relay OFF

🔴 Motor Protection Activated

---

#### Overheat Condition

```text
Temperature ≥ Threshold
```

Result:

🔴 Red LED ON

🔴 Relay OFF

🔴 Motor Protection Activated

---

# 🔄 System Workflow

```text
Motor Operation
      ↓
LM35 Temperature Sensing
ACS712 Current Sensing
      ↓
ATmega328P ADC Processing
      ↓
Fault Analysis
      ↓
Decision Logic
      ↓
Relay Control + Alerts
```

---

# 🧪 Proteus Simulation

Project simulated using **Proteus Design Suite**.

Simulation includes:

- ATmega328P
- LM35 Sensor
- ACS712 Sensor
- RS-555 Motor
- Relay Protection
- UART Monitoring

---

# 📊 Sample UART Output

```text
SYSTEM STARTED

T=30 C I=0.05 A
NORMAL

T=31 C I=0.12 A
OVERLOAD

T=65 C I=0.06 A
OVERHEAT
```

---

# 📈 Industrial Analogy

This project demonstrates practical industrial scenarios:

| Demo Event | Real Industry Example |
|------------|----------------------|
| Shaft Blocking | Conveyor Belt Jam |
| Current Increase | Mechanical Overload |
| Temperature Rise | Pump Overheating |
| Relay Trip | Industrial Motor Protection |

---

# 🧠 Technical Concepts Used

- Embedded Systems
- Embedded C Programming
- Bare-Metal AVR Programming
- ADC Conversion
- Sensor Interfacing
- UART Communication
- Motor Protection Systems
- Industrial Automation Concepts

---

# 🚀 Future Scope

- IoT Dashboard Monitoring
- GSM Alert System
- Cloud Integration
- Predictive Maintenance
- AI-Based Fault Prediction
- Industrial IIoT Deployment

---

# 🧑‍💻 Author

**Piyush Chinde**  
3rd Year ENTC Engineering Student  
Embedded Systems | Hardware | Automation

LinkedIn:[ YOUR_LINKEDIN_LINK](https://www.linkedin.com/in/piyush-chinde-60423b29b)
---

# ⭐ If you like this project

Consider giving this repository a ⭐
