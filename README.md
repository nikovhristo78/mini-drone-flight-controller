# 📖 README - MINI DRONE FLIGHT CONTROLLER

## 🚁 What You Have

A **complete, working Mini Drone Flight Controller** with:

✅ **Full Source Code** - PlatformIO ready  
✅ **Web Dashboard** - Real-time control & telemetry  
✅ **3x Motor Control** - Independent speed adjustment  
✅ **IMU Sensor** - Pitch, Roll, Yaw tracking  
✅ **Error Detection** - Know what's connected  
✅ **WiFi AP Mode** - Connect from any device  
✅ **Password Protected** - MiniDrone / 12345678  
✅ **Easy Wiring Diagram** - No guessing  
✅ **Full Error Handling** - Tells you what's wrong  
✅ **Connection Testing** - Diagnose problems  

---

## 🔧 HARDWARE (Only These - No Extras)

- 1x **ESP32-C3 SuperMini** (microcontroller)
- 1x **MPU-6050** (gyro + accelerometer)
- 2x **TB6612FNG** (motor drivers)
- 3x **Coreless DC Motors** (propulsion)
- 1x **5V Battery** (power source)

**Total Cost:** ~$25-30 USD

---

## 📌 PINS USED

```
5V, GND, 3.3V, 0, 1, 2, 3, 4, 5, 6, 20, 21
```

**That's it!** No other pins needed.

---

## 🚀 QUICK START (3 Steps)

### 1️⃣ Wire It
Follow **WIRING_COMPLETE.md** for detailed connections

### 2️⃣ Build It  
Follow **BUILD_INSTRUCTIONS.md** to upload firmware

### 3️⃣ Fly It
Open **http://192.168.4.1** on your phone/computer

---

## 📊 DASHBOARD FEATURES

### 🛫 FLIGHT TAB
- Real-time telemetry (PITCH, ROLL, YAW)
- Throttle slider (0-100%)
- ARM/DISARM buttons
- Motor status (connected or error)
- Emergency stop button

### 🔧 DIAGNOSTICS TAB  
- System health check
- Connection testing
- Error detection
- All components verified

### 🌐 CONNECTION TAB
- Your IP address
- Connect to another device
- WiFi settings
- Sensor status

---

## 🛠️ BUILDING (Step-by-Step)

### What You Need
- PC with VS Code installed
- USB cable (USB-C for ESP32-C3)
- 5V battery (AAA recommended)

### Installation

1. **Download VS Code**  
   https://code.visualstudio.com/

2. **Install PlatformIO**  
   Extensions → Search "PlatformIO" → Install → Restart

3. **Download This Project**  
   https://github.com/nikovhristo78/mini-drone-flight-controller  
   Click Code → Download ZIP

4. **Open in VS Code**  
   File → Open Folder → Select project folder

5. **Connect ESP32**  
   Plug in USB cable

6. **Upload**  
   Bottom toolbar → Upload button (arrow right)

7. **Wait 2 Minutes**  
   Compilation happens automatically

8. **Check Serial Monitor**  
   Should show: "✓ All systems initialized"

---

## 📱 CONNECTING TO DASHBOARD

### On Phone/Computer

1. Look for WiFi networks
2. Connect to **"MiniDrone"** (password: **12345678**)
3. Wait 3 seconds
4. Open browser: **http://192.168.4.1**
5. Dashboard loads!

### First Connection
- Allow pop-ups if prompted
- Refresh page if blank (F5)
- Check WiFi signal (need to be close)

---

## ✈️ FLYING

### Safety First!
- [ ] Propellers clear of objects
- [ ] Hands behind propellers  
- [ ] Run diagnostics tests
- [ ] All tests show ✅ PASS
- [ ] System Health: ✅ OK

### Flight Steps

1. **Arm Drone**  
   Click green ARM button

2. **Increase Throttle**  
   Move slider to 20-30%

3. **Monitor Telemetry**  
   Watch PITCH/ROLL values

4. **Control Height**  
   Adjust throttle up/down

5. **Emergency Stop**  
   Red button stops everything

---

## 🧪 TESTING

### Auto-Tests (In Dashboard)

1. Open DIAGNOSTICS tab (🔧)
2. Click RUN TESTS
3. Check results:
   - ✅ PASS (Green) = Good
   - ❌ FAIL (Red) = Fix it

### Serial Monitor Logs

Open Serial Monitor in PlatformIO (115200 baud):

```
✓ MINI DRONE FLIGHT CONTROLLER v1.0
✓ Motor pins configured
✓ MPU-6050 initialized
✓ WiFi AP Started: 192.168.4.1
✓ All systems initialized
```

---

## ⚠️ ERROR MESSAGES

### "IMU NOT CONNECTED"
```
Fix: Check pins 20 & 21 connections
     Verify 3.3V and GND
     Restart ESP32 (press EN)
```

### "MOTOR ERROR"
```
Fix: Check TB6612FNG power (5V, NOT 3.3V)
     Verify pins connected
     Check motor spins freely
```

### "WiFi Not Found"
```
Fix: Check battery connected to 5V
     Restart ESP32
     Wait 5 seconds for WiFi
```

### "Cannot ARM"
```
Fix: IMU must be connected first
     Check diagnostics tests
     All should show PASS
```

---

## 🔐 WiFi Security

**Default Credentials:**
- SSID: `MiniDrone`
- Password: `12345678`

**To Change:**

Edit `src/main.cpp` line ~20:

```cpp
const char* ssid = "MyDrone";        // Change name
const char* password = "mypassword";  // Change password
```

Then upload again.

---

## 📚 ALL FILES

| File | Purpose |
|------|----------|
| `platformio.ini` | PlatformIO configuration |
| `src/main.cpp` | Complete firmware code |
| `lib/MPU6050/` | Sensor driver |
| `WIRING_COMPLETE.md` | Detailed wiring diagram |
| `DASHBOARD_GUIDE.md` | How to use dashboard |
| `BUILD_INSTRUCTIONS.md` | Step-by-step build |
| `README.md` | This file |
| `.gitignore` | Git ignore rules |

---

## 🎓 LEARNING RESOURCES

### ESP32-C3
- Official: https://www.espressif.com/
- Documentation: https://docs.espressif.com/

### PlatformIO
- Docs: https://docs.platformio.org/
- Community: https://community.platformio.org/

### Arduino
- Reference: https://www.arduino.cc/reference/
- Tutorials: https://www.arduino.cc/en/Tutorial/

### Drones
- Theory: https://en.wikipedia.org/wiki/Quadcopter
- Control: https://en.wikipedia.org/wiki/Flight_control_system

---

## 🐛 BUG REPORTING

Found an issue? 

1. Check **BUILD_INSTRUCTIONS.md** troubleshooting
2. Check **DASHBOARD_GUIDE.md** error messages  
3. Open Serial Monitor and read error logs
4. Create GitHub issue with:
   - Error message
   - What you did
   - What you expected
   - Serial monitor output

---

## 📋 PROJECT STATUS

✅ **Complete & Working**
- [x] Firmware compiles without errors
- [x] Dashboard fully responsive
- [x] All sensors tested
- [x] Motor control verified
- [x] Error handling implemented
- [x] Connection diagnostics working
- [x] Documentation complete
- [x] Ready to fly!

---

## 📄 LICENSE

Free to use, modify, and share.
Give credit if you share it!

---

## 🎉 YOU'RE READY!

**Next Steps:**
1. Read WIRING_COMPLETE.md
2. Follow BUILD_INSTRUCTIONS.md
3. Open http://192.168.4.1
4. **FLY!** 🚁

---

**Made with ❤️ for makers**
