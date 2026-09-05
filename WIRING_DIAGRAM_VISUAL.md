# 🔌 MINI DRONE - VISUAL WIRING DIAGRAM

## STEP-BY-STEP VISUAL GUIDE

---

## 📋 WHAT YOU NEED (ONLY THESE PARTS)

```
┌─────────────────────────────────────────────────────────────────┐
│  1x ESP32-C3 SuperMini (Microcontroller)                        │
│  1x MPU-6050 Sensor (Gyro + Accelerometer)                      │
│  2x TB6612FNG Motor Driver Modules                              │
│  3x Coreless DC Motors (Small drone motors)                     │
│  1x 5V Battery (AAA recommended)                                │
└─────────────────────────────────────────────────────────────────┘
```

---

## 🔋 STEP 1: BATTERY CONNECTION

```
                    5V BATTERY (AAA)
                    ┌─────────────┐
                    │   (+) RED   │
                    │   (-) BLACK │
                    └──┬──────┬───┘
                       │      │
                    [5V]    [GND]
                       ▼      ▼
        ┌──────────────────────────────────┐
        │      ESP32-C3 PINS (Top)          │
        │                                  │
        │  5V ◄── RED WIRE (Battery +)     │
        │  GND ◄── BLACK WIRE (Battery -)  │
        │                                  │
        └──────────────────────────────────┘

⚠️  IMPORTANT: 5V ONLY! NOT 3.3V!
    Connect RED → 5V pin
    Connect BLACK → GND pin
```

---

## 📡 STEP 2: IMU SENSOR (MPU-6050) CONNECTION

```
    MPU-6050 Sensor Module
    ┌─────────────────────────┐
    │ VCC  GND  SDA  SCL  INT │
    └──┬───┬───┬───┬───┬─────┘
       │   │   │   │   │
       │   │   │   │   └── NOT USED
       │   │   │   │
       │   │   │   └─────────────► ESP32-C3 Pin 21 (SCL)
       │   │   │
       │   │   └───────────────► ESP32-C3 Pin 20 (SDA)
       │   │
       │   └───────────────► ESP32-C3 GND
       │
       └───────────────► ESP32-C3 Pin 3.3V

    ┌──────────────────────────────────┐
    │  ESP32-C3 CONNECTIONS            │
    │                                  │
    │  Pin 20 (SDA) ◄── MPU-6050 SDA  │
    │  Pin 21 (SCL) ◄── MPU-6050 SCL  │
    │  3.3V ◄── MPU-6050 VCC           │
    │  GND ◄── MPU-6050 GND            │
    │                                  │
    └──────────────────────────────────┘

⚠️  CRITICAL: Use Pin 20 & 21 ONLY
    These are the I2C pins
    If reversed → IMU won't be detected
```

---

## 🎛️ STEP 3: MOTOR DRIVER 1 (TB6612FNG) - Controls Motor 1

```
    TB6612FNG Module #1
    ┌────────────────────────┐
    │ VCC GND IN1 IN2 OUT1   │
    │           OUT2 AIN1    │
    │ (Bottom side)          │
    └──┬──┬───┬───┬───┬──────┘
       │  │   │   │   │
       │  │   │   │   └─────► Motor 1 Negative (Black)
       │  │   │   │
       │  │   │   └─────► Motor 1 Positive (Red)
       │  │   │
       │  │   └─ IN2 ─────► ESP32-C3 Pin 6
       │  │
       │  ├─ IN1 ─────► ESP32-C3 Pin 5
       │  │
       │  └─ GND ─────► Battery GND (Black)
       │
       └─ VCC ─────► Battery 5V (Red)

    Connection Summary:
    ┌─────────────────────────────────────┐
    │ TB6612FNG #1 → ESP32-C3             │
    ├─────────────────────────────────────┤
    │ VCC → Battery 5V (Red wire)         │
    │ GND → Battery GND (Black wire)      │
    │ IN1 → ESP32-C3 Pin 5                │
    │ IN2 → ESP32-C3 Pin 6                │
    │ OUT1 → Motor 1 Red (Positive)       │
    │ OUT2 → Motor 1 Black (Negative)     │
    └─────────────────────────────────────┘

🔴 Motor 1 Wires:
    Red = OUT1 from TB6612FNG #1
    Black = OUT2 from TB6612FNG #1
```

---

## 🎛️ STEP 4: MOTOR DRIVER 2 (TB6612FNG) - Controls Motor 2

```
    TB6612FNG Module #2
    ┌────────────────────────┐
    │ VCC GND IN1 IN2 OUT1   │
    │           OUT2 AIN1    │
    └──┬──┬───┬───┬───┬──────┘
       │  │   │   │   │
       │  │   │   │   └─────► Motor 2 Negative (Black)
       │  │   │   │
       │  │   │   └─────► Motor 2 Positive (Red)
       │  │   │
       │  │   └─ IN2 ─────► ESP32-C3 Pin 3
       │  │
       │  ├─ IN1 ─────► ESP32-C3 Pin 4
       │  │
       │  └─ GND ─────► Battery GND (Black)
       │
       └─ VCC ─────► Battery 5V (Red)

    Connection Summary:
    ┌─────────────────────────────────────┐
    │ TB6612FNG #2 → ESP32-C3             │
    ├─────────────────────────────────────┤
    │ VCC → Battery 5V (Red wire)         │
    │ GND → Battery GND (Black wire)      │
    │ IN1 → ESP32-C3 Pin 4                │
    │ IN2 → ESP32-C3 Pin 3                │
    │ OUT1 → Motor 2 Red (Positive)       │
    │ OUT2 → Motor 2 Black (Negative)     │
    └─────────────────────────────────────┘

🟢 Motor 2 Wires:
    Red = OUT1 from TB6612FNG #2
    Black = OUT2 from TB6612FNG #2
```

---

## 🎛️ STEP 5: MOTOR DRIVER 3 (TB6612FNG) - Controls Motor 3

```
    TB6612FNG Module #3
    ┌────────────────────────┐
    │ VCC GND IN1 IN2 OUT1   │
    │           OUT2 AIN1    │
    └──┬──┬───┬───┬───┬──────┘
       │  │   │   │   │
       │  │   │   │   └─────► Motor 3 Negative (Black)
       │  │   │   │
       │  │   │   └─────► Motor 3 Positive (Red)
       │  │   │
       │  │   └─ IN2 ─────► ESP32-C3 Pin 0
       │  │
       │  ├─ IN1 ─────► ESP32-C3 Pin 1
       │  │
       │  └─ GND ─────► Battery GND (Black)
       │
       └─ VCC ─────► Battery 5V (Red)

    Connection Summary:
    ┌─────────────────────────────────────┐
    │ TB6612FNG #3 → ESP32-C3             │
    ├─────────────────────────────────────┤
    │ VCC → Battery 5V (Red wire)         │
    │ GND → Battery GND (Black wire)      │
    │ IN1 → ESP32-C3 Pin 1                │
    │ IN2 → ESP32-C3 Pin 0                │
    │ OUT1 → Motor 3 Red (Positive)       │
    │ OUT2 → Motor 3 Black (Negative)     │
    └─────────────────────────────────────┘

🔵 Motor 3 Wires:
    Red = OUT1 from TB6612FNG #3
    Black = OUT2 from TB6612FNG #3
```

---

## 🔌 COMPLETE PIN MAP

```
ESP32-C3 Top View:
┌─────────────────────────────────────────┐
│  Pin 5 ──► MOTOR 1 IN1                  │
│  Pin 6 ──► MOTOR 1 IN2                  │
│  Pin 4 ──► MOTOR 2 IN1                  │
│  Pin 3 ──► MOTOR 2 IN2                  │
│  Pin 1 ──► MOTOR 3 IN1                  │
│  Pin 0 ──► MOTOR 3 IN2                  │
│  Pin 20 ──► MPU-6050 SDA (I2C Data)     │
│  Pin 21 ──► MPU-6050 SCL (I2C Clock)    │
│  3.3V ──► MPU-6050 VCC                  │
│  5V ──► Battery Power                   │
│  GND ──► All Grounds Connected Together │
└─────────────────────────────────────────┘
```

---

## 📊 COMPLETE WIRING TABLE

| Component | Pin/Wire | Connected To | Color |
|-----------|----------|--------------|-------|
| Battery + | | ESP32-C3 5V | RED |
| Battery - | | ESP32-C3 GND | BLACK |
| MPU-6050 VCC | | ESP32-C3 3.3V | ORANGE |
| MPU-6050 GND | | ESP32-C3 GND | BLACK |
| MPU-6050 SDA | | ESP32-C3 Pin 20 | YELLOW |
| MPU-6050 SCL | | ESP32-C3 Pin 21 | GREEN |
| TB6612FNG #1 VCC | | Battery 5V | RED |
| TB6612FNG #1 GND | | Battery GND | BLACK |
| TB6612FNG #1 IN1 | | ESP32-C3 Pin 5 | BLUE |
| TB6612FNG #1 IN2 | | ESP32-C3 Pin 6 | BLUE |
| TB6612FNG #1 OUT1 | | Motor 1 + | RED |
| TB6612FNG #1 OUT2 | | Motor 1 - | BLACK |
| TB6612FNG #2 VCC | | Battery 5V | RED |
| TB6612FNG #2 GND | | Battery GND | BLACK |
| TB6612FNG #2 IN1 | | ESP32-C3 Pin 4 | BLUE |
| TB6612FNG #2 IN2 | | ESP32-C3 Pin 3 | BLUE |
| TB6612FNG #2 OUT1 | | Motor 2 + | RED |
| TB6612FNG #2 OUT2 | | Motor 2 - | BLACK |
| TB6612FNG #3 VCC | | Battery 5V | RED |
| TB6612FNG #3 GND | | Battery GND | BLACK |
| TB6612FNG #3 IN1 | | ESP32-C3 Pin 1 | BLUE |
| TB6612FNG #3 IN2 | | ESP32-C3 Pin 0 | BLUE |
| TB6612FNG #3 OUT1 | | Motor 3 + | RED |
| TB6612FNG #3 OUT2 | | Motor 3 - | BLACK |

---

## ✅ ASSEMBLY CHECKLIST

After wiring, verify EVERY connection:

### Battery Connections
- [ ] Battery RED (+) to ESP32-C3 5V pin
- [ ] Battery BLACK (-) to ESP32-C3 GND pin
- [ ] Battery connected to ALL TB6612FNG VCC pins (5V)
- [ ] Battery connected to ALL TB6612FNG GND pins (Black)

### IMU Sensor (MPU-6050)
- [ ] VCC to ESP32-C3 3.3V pin
- [ ] GND to ESP32-C3 GND pin
- [ ] SDA to ESP32-C3 Pin 20 (CRITICAL!)
- [ ] SCL to ESP32-C3 Pin 21 (CRITICAL!)
- [ ] INT pin NOT connected (not used)

### Motor Driver 1 (TB6612FNG)
- [ ] VCC to Battery 5V
- [ ] GND to Battery GND
- [ ] IN1 to ESP32-C3 Pin 5
- [ ] IN2 to ESP32-C3 Pin 6
- [ ] OUT1 to Motor 1 RED (+)
- [ ] OUT2 to Motor 1 BLACK (-)

### Motor Driver 2 (TB6612FNG)
- [ ] VCC to Battery 5V
- [ ] GND to Battery GND
- [ ] IN1 to ESP32-C3 Pin 4
- [ ] IN2 to ESP32-C3 Pin 3
- [ ] OUT1 to Motor 2 RED (+)
- [ ] OUT2 to Motor 2 BLACK (-)

### Motor Driver 3 (TB6612FNG)
- [ ] VCC to Battery 5V
- [ ] GND to Battery GND
- [ ] IN1 to ESP32-C3 Pin 1
- [ ] IN2 to ESP32-C3 Pin 0
- [ ] OUT1 to Motor 3 RED (+)
- [ ] OUT2 to Motor 3 BLACK (-)

### General
- [ ] NO loose wires
- [ ] ALL wires twisted (not parallel)
- [ ] ALL connections tight
- [ ] Battery voltage 5V (checked with multimeter)
- [ ] GND connections all together

---

## 🔍 TROUBLESHOOTING CHECKLIST

| Problem | Cause | Solution |
|---------|-------|----------|
| LED on ESP32 doesn't light | No power | Check Battery 5V & GND connections |
| Serial shows "MPU-6050 NOT FOUND" | Wrong pins or no power | Verify pins 20, 21 + 3.3V + GND |
| Serial shows "Motor error" | TB6612FNG no power | Check Battery 5V connection |
| Motors don't spin | Pins wrong or motor stuck | Check pins 0-6, try spinning by hand |
| WiFi not broadcasting | ESP32 no power | Check Battery 5V connection |
| Dashboard loads but no data | Connection issue | Check WiFi & Serial Monitor |

---

## ⚡ VOLTAGE REFERENCE

```
Battery (5V):
  ├─ To ESP32-C3 5V pin ✓
  ├─ To ALL TB6612FNG VCC pins ✓
  └─ DO NOT use 3.3V for battery power ✗

ESP32-C3 3.3V Pin (Internal Regulator):
  ├─ To MPU-6050 VCC ✓
  └─ DO NOT connect battery here ✗

ESP32-C3 GND:
  ├─ Connected to Battery GND ✓
  ├─ Connected to ALL TB6612FNG GND ✓
  ├─ Connected to MPU-6050 GND ✓
  └─ ALL GND connected together ✓
```

---

## 🎯 FINAL CHECK BEFORE POWERING ON

1. **Disconnect battery**
2. **Check every connection visually**
3. **Use continuity tester** (beeper mode):
   - Battery RED to ESP32-C3 5V → Should beep
   - Battery BLACK to ESP32-C3 GND → Should beep
   - Pin 5 to TB6612FNG #1 IN1 → Should beep
   - Pin 6 to TB6612FNG #1 IN2 → Should beep
   - (Check all motor pins similarly)
4. **Verify no shorts** (use ohm meter, should show high resistance between VCC and GND)
5. **Reconnect battery slowly**
6. **Check LED on ESP32 lights up**
7. **Check serial monitor for initialization messages**

---

## 📸 IF STILL HAVING ISSUES

Take photos of:
1. ESP32-C3 with all connected wires
2. MPU-6050 sensor connections
3. Each TB6612FNG module connections
4. Battery connections
5. Serial monitor output (error messages)

Share serial error messages for diagnosis!

