# 📱 DASHBOARD USER GUIDE

## THREE CONTROL MODES

### MODE 1: FLIGHT CONTROL (Default)
Main flight dashboard with real-time telemetry

**Display shows:**
- ✈️ Flight Status: ARMED / DISARMED
- 💚 System Health: OK / CHECK CONNECTIONS
- 📊 Telemetry (updates every 500ms):
  - PITCH: Forward/backward tilt
  - ROLL: Left/right tilt  
  - YAW RATE: Rotation speed

**Controls:**
- **Throttle Slider**: 0-100% motor speed
- **ARM Button**: Prepare for flight (green)
- **EMERGENCY STOP**: Kill all motors (red)
- **THROTTLE +**: Increase 10%
- **THROTTLE −**: Decrease 10%
- **Motor Status**: M1/M2/M3 connected?

---

### MODE 2: DIAGNOSTICS (🔧 Tab)
Check all connections and identify problems

**Test Results Show:**
- ✅ PASS (Green) = Connected & working
- ❌ FAIL (Red) = Problem detected
- ⚙️ TESTING (Orange) = Checking now

**Tests:**
1. **Power Supply** → Battery connected?
2. **IMU (Pins 20,21)** → Sensor found?
3. **Motor 1 (Pins 5,6)** → Driver connected?
4. **Motor 2 (Pins 4,3)** → Driver connected?
5. **Motor 3 (Pins 2,1)** → Driver connected?
6. **WiFi AP** → Transmitting?

**What to do if test FAILS:**
```
FAIL: IMU not found
→ Check SDA (pin 20) and SCL (pin 21) wires
→ Check 3.3V and GND connections
→ Restart ESP32 (press EN button)

FAIL: Motor 1 Error
→ Check TB6612FNG #1 power (5V + GND)
→ Check IN1 (pin 5) and IN2 (pin 6) wires
→ Verify motor is spinning freely

FAIL: WiFi Broadcasting
→ Check battery is connected to 5V
→ Restart ESP32 (press EN button)
→ Wait 5 seconds for WiFi to start
```

---

### MODE 3: CONNECTION INFO (🌐 Tab)
Show IP address and device settings

**Your IP Address**
- Default: **192.168.4.1**
- Share this with friends to control remotely
- Must be on same WiFi network

**How to Connect Another Device:**
1. Both devices connect to "MiniDrone" WiFi
2. Enter the drone's IP in the input box
3. Click CONNECT
4. Now you have full control!

**WiFi Settings:**
```
SSID:     MiniDrone
Password: 12345678
Mode:     Access Point (creates its own network)
IP:       192.168.4.1
```

**Sensor Status:**
- Shows each component connection status
- Red ❌ = Needs fixing
- Green ✅ = Working

---

## 🎮 BASIC FLIGHT PROCEDURE

### PREFLIGHT CHECKLIST
1. ☑️ Check all wires connected
2. ☑️ Open DIAGNOSTICS tab → RUN TESTS
3. ☑️ All tests should show ✅ PASS
4. ☑️ Propellers clear of objects
5. ☑️ Hands behind propellers

### STARTUP SEQUENCE
1. Connect WiFi "MiniDrone" (password: 12345678)
2. Open http://192.168.4.1 in browser
3. Wait for dashboard to load
4. Check System Health shows ✅ OK
5. Throttle slider at 0%

### FLIGHT OPERATION
1. Click **ARM DRONE** button (green)
2. Move throttle slider slowly (0% → 30%)
3. Watch PITCH/ROLL values in real-time
4. Adjust throttle for desired height
5. At any time, click **EMERGENCY STOP** (red)

### LANDING PROCEDURE
1. Slowly reduce throttle to 0%
2. Wait for drone to land
3. Verify motors stop spinning
4. Click **DISARM** (safest)
5. Power off when ready

---

## ⚠️ ERROR MESSAGES & SOLUTIONS

### "IMU NOT CONNECTED"
```
Cause: Sensor wires loose or wrong pins

Fix 1: Check pin connections
  └─ SDA must be on PIN 20
  └─ SCL must be on PIN 21
  └─ Both need 3.3V and GND

Fix 2: Try I2C Scanner
  └─ Open Arduino IDE
  └─ Search: I2C Scanner example
  └─ Upload and check serial for found devices

Fix 3: Restart everything
  └─ Disconnect battery
  └─ Wait 5 seconds
  └─ Reconnect battery
  └─ Check serial output
```

### "CANNOT ARM - SENSOR ERROR"
```
Cause: IMU not working
Fix: Fix IMU connection first (see above)
```

### "MOTOR 1/2/3 ERROR"
```
Cause: TB6612FNG not powered or pins wrong

Fix 1: Check power
  └─ TB6612FNG VCC → Battery 5V (not 3.3V!)
  └─ TB6612FNG GND → Battery GND

Fix 2: Check pins
  └─ Motor 1: Pins 5 & 6
  └─ Motor 2: Pins 4 & 3
  └─ Motor 3: Pins 2 & 1

Fix 3: Check motor
  └─ Try spinning with hand
  └─ If stuck, clean gear
```

### "CONNECTION LOST"
```
Cause: WiFi dropped or battery low

Fix 1: Reconnect WiFi
  └─ Check "MiniDrone" network appears
  └─ If not, restart ESP32 (press EN button)
  └─ Wait 5 seconds for WiFi to start

Fix 2: Check battery voltage
  └─ Should be 5V+
  └─ Replace battery if dead
```

---

## 🔌 CONNECTING TO SERIAL MONITOR

**To see detailed logs:**

1. Open PlatformIO in VS Code
2. Click "Serial Monitor" button
3. Check settings: 115200 baud
4. Should show messages like:
   ```
   ✓ MINI DRONE FLIGHT CONTROLLER v1.0
   ✓ Motor pins configured
   ✓ MPU-6050 initialized
   ✓ WiFi AP Started: 192.168.4.1
   ✓ All systems initialized
   ```

**Error examples in serial:**
```
✗ IMU: MPU-6050 NOT FOUND! Check wiring (SDA:20, SCL:21)
✗ Motor connection error - Check pins 5,6,4,3,2,1
✗ WiFi failed - Check battery power
```

---

## 🎨 CUSTOMIZING DASHBOARD

Want to change colors or layout?

**Edit HTML section in main.cpp:**
```cpp
const char* html_page = R"(
<!DOCTYPE html>
<html>
<head>
    <style>
        body { background: linear-gradient(135deg, #0f0f1e 0%, #1a1a2e 100%); }
        // Change these colors:
        .btn-primary { background: #00ff00; }     // Green buttons
        .status-value { color: #00ff00; }         // Green text
        .title { color: #00ff00; }                // Title color
    </style>
```

Then upload again via PlatformIO.

---

## 💾 SAVE YOUR SETTINGS

To backup your configuration:
1. Download the repository ZIP
2. Save to cloud (Google Drive, etc)
3. Easy to restore if needed

