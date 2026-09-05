# 🚁 MINI DRONE - COMPLETE WIRING DIAGRAM

## 📌 PINS USED (ONLY THESE 11)

```
ESP32-C3 PINS:
├─ 5V        → POWER (Battery +)
├─ GND       → GROUND (Battery -)
├─ 3.3V      → IMU VCC
├─ Pin 0     → MOTOR 3 IN2
├─ Pin 1     → MOTOR 3 IN1
├─ Pin 2     → MOTOR 2 IN2
├─ Pin 3     → MOTOR 2 IN1
├─ Pin 4     → MOTOR 2 IN2 (alternative)
├─ Pin 5     → MOTOR 1 IN1
├─ Pin 6     → MOTOR 1 IN2
├─ Pin 20    → I2C SDA (IMU)
└─ Pin 21    → I2C SCL (IMU)
```

---

## 🔋 CONNECTION DIAGRAM

### STEP 1: BATTERY CONNECTION
```
┌─────────────────────────────┐
│   5V BATTERY (AAA)          │
│   (+) RED  (−) BLACK        │
└────┬────────────────┬───────┘
     │                │
     ↓                ↓
  [5V PIN]       [GND PIN]
  ESP32-C3       ESP32-C3
```

### STEP 2: IMU SENSOR (MPU-6050)
```
┌────────────────────────────┐
│    MPU-6050 SENSOR         │
│  (Gyro + Accelerometer)    │
├────────────────────────────┤
│ VCC  → ESP32 Pin 3.3V      │
│ GND  → ESP32 GND           │
│ SDA  → ESP32 Pin 20        │  ← Most Important!
│ SCL  → ESP32 Pin 21        │
│ INT  → (not used)          │
└────────────────────────────┘
```

### STEP 3: MOTOR DRIVER 1 (TB6612FNG) - For Motor 1
```
┌──────────────────────────────┐
│   TB6612FNG Motor Driver #1   │
├──────────────────────────────┤
│ VCC → Battery 5V             │
│ GND → Battery GND            │
│ IN1 → ESP32 Pin 5            │
│ IN2 → ESP32 Pin 6            │
│ OUT1 → Motor 1 RED (+)       │
│ OUT2 → Motor 1 BLACK (−)     │
└──────────────────────────────┘
```

### STEP 4: MOTOR DRIVER 2 (TB6612FNG) - For Motor 2
```
┌──────────────────────────────┐
│   TB6612FNG Motor Driver #2   │
├──────────────────────────────┤
│ VCC → Battery 5V             │
│ GND → Battery GND            │
│ IN1 → ESP32 Pin 4            │
│ IN2 → ESP32 Pin 3            │
│ OUT1 → Motor 2 RED (+)       │
│ OUT2 → Motor 2 BLACK (−)     │
└──────────────────────────────┘
```

### STEP 5: MOTOR DRIVER 3 (TB6612FNG) - For Motor 3
```
┌──────────────────────────────┐
│   TB6612FNG Motor Driver #3   │
├──────────────────────────────┤
│ VCC → Battery 5V             │
│ GND → Battery GND            │
│ IN1 → ESP32 Pin 1            │
│ IN2 → ESP32 Pin 0            │
│ OUT1 → Motor 3 RED (+)       │
│ OUT2 → Motor 3 BLACK (−)     │
└──────────────────────────────┘
```

---

## ✅ QUICK CONNECTION CHECKLIST

- [ ] Battery 5V to ESP32 VIN (5V pin)
- [ ] Battery GND to ESP32 GND
- [ ] MPU-6050 VCC to ESP32 3.3V
- [ ] MPU-6050 GND to ESP32 GND
- [ ] MPU-6050 SDA to ESP32 Pin 20
- [ ] MPU-6050 SCL to ESP32 Pin 21
- [ ] TB6612FNG #1 VCC to Battery 5V
- [ ] TB6612FNG #1 GND to Battery GND
- [ ] TB6612FNG #1 IN1 to ESP32 Pin 5
- [ ] TB6612FNG #1 IN2 to ESP32 Pin 6
- [ ] TB6612FNG #1 OUT1/OUT2 to Motor 1
- [ ] TB6612FNG #2 VCC to Battery 5V
- [ ] TB6612FNG #2 GND to Battery GND
- [ ] TB6612FNG #2 IN1 to ESP32 Pin 4
- [ ] TB6612FNG #2 IN2 to ESP32 Pin 3
- [ ] TB6612FNG #2 OUT1/OUT2 to Motor 2
- [ ] TB6612FNG #3 VCC to Battery 5V
- [ ] TB6612FNG #3 GND to Battery GND
- [ ] TB6612FNG #3 IN1 to ESP32 Pin 1
- [ ] TB6612FNG #3 IN2 to ESP32 Pin 0
- [ ] TB6612FNG #3 OUT1/OUT2 to Motor 3

---

## 🔴 COMMON MISTAKES

| Mistake | Problem | Fix |
|---------|---------|-----|
| SDA/SCL reversed | IMU not detected | Swap pins 20 & 21 |
| Wrong battery voltage | ESP32 dead | Use 5V, NOT 3.3V |
| GND not connected | Random errors | Connect all GND together |
| Motor pins swapped | Motors spin wrong way | Reverse motor wires |
| TB6612FNG no power | Motors don't spin | Check VCC to 5V |
| Loose wires | Intermittent errors | Use solid core wire |

---

## 🧪 TEST YOUR WIRING

1. **Power test**: LED on ESP32 should light up
2. **Serial output**: Should show "All systems initialized"
3. **WiFi test**: Search for "MiniDrone" network
4. **IMU test**: Tilting shows PITCH/ROLL changes
5. **Motor test**: Motors spin when throttle > 0%

