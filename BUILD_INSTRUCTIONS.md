# 🛠️ QUICK BUILD GUIDE - 5 MINUTES

## ⏱️ STEP 1: Install Tools (2 min)

### Download Visual Studio Code
```
https://code.visualstudio.com/
```

### Install PlatformIO Extension
1. Open VS Code
2. Click Extensions (left sidebar, looks like squares)
3. Search: "PlatformIO"
4. Click "Install" (Microsoft official)
5. Restart VS Code

---

## 📥 STEP 2: Download This Project (1 min)

### Option A: GitHub (Easiest)
1. Go to: https://github.com/nikovhristo78/mini-drone-flight-controller
2. Click green **"Code"** button
3. Click **"Download ZIP"**
4. Unzip to folder
5. Open folder in VS Code

### Option B: Git Command
```bash
git clone https://github.com/nikovhristo78/mini-drone-flight-controller.git
cd mini-drone-flight-controller
```

---

## 🔌 STEP 3: Wire Hardware (Follow WIRING_COMPLETE.md)

**Quick checklist:**
- [ ] Battery 5V → ESP32 VIN
- [ ] Battery GND → ESP32 GND  
- [ ] MPU-6050 SDA → Pin 20
- [ ] MPU-6050 SCL → Pin 21
- [ ] Motor 1: Pins 5,6
- [ ] Motor 2: Pins 4,3
- [ ] Motor 3: Pins 2,1

---

## 💻 STEP 4: Upload Firmware

### Connect ESP32 to Computer
- USB-C cable from ESP32 to computer
- Wait for driver installation (auto)

### Upload Code
1. In VS Code, look at bottom toolbar
2. Find PlatformIO icon (alien head)
3. Click **"Upload"** (arrow pointing right)
4. Wait 1-2 minutes for compile & upload

### Expected Output
```
✓ Compiling firmware...
✓ Uploading /firmware.bin
✓ Hash of data verified
✓ Leaving... hard resetting via RTS pin
```

---

## 📊 STEP 5: Check Serial Monitor

1. Click Serial Monitor in PlatformIO
2. Baud rate: **115200**
3. Should show:
   ```
   ✓ MINI DRONE FLIGHT CONTROLLER v1.0
   ✓ Motor pins configured
   ✓ MPU-6050 initialized  
   ✓ WiFi AP Started: 192.168.4.1
   ✓ All systems initialized
   ```

**If you see errors:**
```
✗ MPU-6050 NOT FOUND
  → Check pins 20 & 21 connections
  
✗ Motor error
  → Check TB6612FNG power (5V)
```

---

## 🌐 STEP 6: Connect WiFi

### From Your Phone/Computer

1. Look for WiFi networks
2. Find **"MiniDrone"**
3. Connect (password: **12345678**)
4. Wait 3 seconds
5. Open browser: **http://192.168.4.1**
6. You should see the dashboard!

---

## 🧪 STEP 7: Run Tests

### In Dashboard
1. Click **DIAGNOSTICS** tab (🔧)
2. Click **RUN TESTS** button
3. All should show ✅ PASS

**If any show ❌ FAIL:**
```
FAIL: IMU
→ Check SDA (pin 20) & SCL (pin 21)
→ Restart ESP32 (press EN button)

FAIL: Motor 1/2/3
→ Check TB6612FNG is powered (5V)
→ Check IN pins are connected

FAIL: WiFi
→ Restart ESP32 (press EN button)
→ Wait 5 seconds for WiFi
```

---

## 🚁 STEP 8: READY TO FLY!

### Preflight Checklist
- [ ] All tests pass ✅
- [ ] System Health: OK ✅
- [ ] Throttle at 0%
- [ ] Propellers clear
- [ ] Hands behind motors

### First Flight
1. Click **ARM DRONE** (green button)
2. Slowly move throttle to 20%
3. Watch PITCH/ROLL values
4. Adjust throttle as needed
5. **EMERGENCY STOP** anytime (red button)

---

## 🆘 TROUBLESHOOTING

### Dashboard won't load
```
Fix 1: Check WiFi "MiniDrone" connected
Fix 2: Try IP: http://192.168.4.1
Fix 3: Restart ESP32 (press EN button)
Fix 4: Open Serial Monitor to check errors
```

### IMU not detected
```
Fix 1: Verify pins 20 & 21 wires
Fix 2: Check 3.3V and GND connections
Fix 3: Try different I2C address (0x69)
Fix 4: Use I2C Scanner to find device
```

### Motors don't spin
```
Fix 1: Check TB6612FNG power (5V, NOT 3.3V)
Fix 2: Verify motor control pins connected
Fix 3: Try spinning motor by hand (check if stuck)
Fix 4: Check battery voltage (minimum 5V)
```

### WiFi doesn't start
```
Fix 1: Check battery connected to 5V
Fix 2: Restart ESP32 (press EN button)
Fix 3: Wait 5 seconds for WiFi to initialize
Fix 4: Check Serial Monitor for errors
```

---

## 📚 ALL FILES EXPLAINED

```
mini-drone-flight-controller/
├── platformio.ini               ← PlatformIO settings
├── src/
│   └── main.cpp               ← Complete firmware code
├── lib/
│   └── MPU6050/               ← Sensor library
│       ├── MPU6050.h
│       └── MPU6050.cpp
├── WIRING_COMPLETE.md         ← How to connect parts
├── DASHBOARD_GUIDE.md         ← How to use web interface
├── BUILD_INSTRUCTIONS.md      ← This file
├── README.md                  ← Project overview
└── .gitignore                 ← Git settings
```

---

## 🎓 NEXT STEPS

### Customize the Code
```cpp
// In src/main.cpp, change these:
const char* ssid = "MiniDrone";        // WiFi name
const char* password = "12345678";     // WiFi password
#define MOTOR1_IN1 5                    // Pin numbers
```

### Add More Features
- Altitude hold
- Manual flight modes
- GPS integration
- Flight logs
- Phone app control

### Share Your Project
- Fork this repo on GitHub
- Add your improvements
- Help others learn!

---

## 🎉 YOU'RE DONE!

Your Mini Drone Flight Controller is now **COMPLETE** and **WORKING**!

**Next: Go to DASHBOARD_GUIDE.md to learn how to fly**

