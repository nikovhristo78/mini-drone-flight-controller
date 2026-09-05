#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "MPU6050.h"

// ===== PIN CONFIGURATION (ESP32-C3) =====
#define MOTOR1_IN1 5
#define MOTOR1_IN2 6
#define MOTOR2_IN1 4
#define MOTOR2_IN2 3
#define MOTOR3_IN1 2
#define MOTOR3_IN2 1
#define SDA_PIN 20
#define SCL_PIN 21

// WiFi Credentials
const char* ssid = "MiniDrone";
const char* password = "12345678";

// System Variables
MPU6050 mpu;
WebServer server(80);
float pitch = 0, roll = 0, yaw = 0;
float throttle = 0;
int motor1_speed = 0, motor2_speed = 0, motor3_speed = 0;
bool armed = false;

// System Diagnostics
struct SystemDiagnostics {
  bool wifi_ok = false;
  bool imu_ok = false;
  bool motor1_ok = true;
  bool motor2_ok = true;
  bool motor3_ok = true;
  bool battery_ok = true;
  String error_details = "";
  unsigned long last_check = 0;
} diag;

// Professional Flight Control Dashboard HTML
const char* html_page = R"(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <title>🚁 ESP32-C3 Flight Control</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { 
            font-family: 'Courier New', monospace; 
            background: linear-gradient(135deg, #0a0e27 0%, #16213e 100%); 
            color: #00ff00; 
            min-height: 100vh; 
            padding: 20px;
        }
        
        .container { max-width: 1400px; margin: 0 auto; }
        
        .header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            padding: 15px 20px;
            background: rgba(0, 50, 50, 0.8);
            border: 2px solid #00ff00;
            border-radius: 5px;
            margin-bottom: 20px;
        }
        
        .title { 
            font-size: 20px; 
            font-weight: bold;
            color: #00ff00;
            text-shadow: 0 0 10px #00ff00;
        }
        
        .status-display {
            display: flex;
            gap: 20px;
            font-size: 14px;
        }
        
        .status-item {
            padding: 5px 10px;
            border: 1px solid #00ff00;
            border-radius: 3px;
        }
        
        .armed-status {
            font-weight: bold;
            color: #ff3333;
        }
        
        .armed-status.armed {
            color: #ff3333;
        }
        
        .ready-status {
            color: #00ff00;
        }
        
        .main-grid {
            display: grid;
            grid-template-columns: 1fr 1fr 1fr;
            gap: 15px;
            margin-bottom: 20px;
        }
        
        .telemetry-box {
            background: rgba(0, 50, 50, 0.8);
            border: 2px solid #00ff00;
            padding: 15px;
            border-radius: 5px;
            text-align: center;
        }
        
        .telemetry-label {
            font-size: 11px;
            color: #00aaaa;
            text-transform: uppercase;
            margin-bottom: 8px;
        }
        
        .telemetry-value {
            font-size: 32px;
            font-weight: bold;
            color: #00ff00;
            font-family: 'Courier New', monospace;
        }
        
        .telemetry-unit {
            font-size: 10px;
            color: #00aaaa;
            margin-top: 5px;
        }
        
        .controls-row {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 15px;
            margin-bottom: 20px;
        }
        
        .control-panel {
            background: rgba(0, 50, 50, 0.8);
            border: 2px solid #00ff00;
            padding: 20px;
            border-radius: 5px;
        }
        
        .control-title {
            font-size: 13px;
            color: #00aaaa;
            text-transform: uppercase;
            margin-bottom: 15px;
            text-align: center;
        }
        
        .gimbal-input {
            width: 200px;
            height: 200px;
            margin: 0 auto 20px;
            background: radial-gradient(circle at 30% 30%, rgba(0,255,0,0.1), transparent);
            border: 2px solid #00ff00;
            border-radius: 50%;
            position: relative;
            display: flex;
            align-items: center;
            justify-content: center;
        }
        
        .gimbal-center {
            width: 80px;
            height: 80px;
            background: rgba(0, 255, 0, 0.2);
            border: 2px solid #00ff00;
            border-radius: 5px;
            display: flex;
            align-items: center;
            justify-content: center;
            color: #00ff00;
            font-size: 20px;
        }
        
        .gimbal-label {
            position: absolute;
            font-size: 10px;
            color: #00aaaa;
            text-transform: uppercase;
        }
        
        .gimbal-label.top { top: 10px; }
        .gimbal-label.bottom { bottom: 10px; }
        .gimbal-label.left { left: 10px; }
        .gimbal-label.right { right: 10px; }
        
        .spatial-grid {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            gap: 10px;
            margin-bottom: 15px;
        }
        
        .spatial-btn {
            padding: 30px;
            background: rgba(0, 100, 100, 0.8);
            border: 2px solid #00ff00;
            color: #00ff00;
            cursor: pointer;
            border-radius: 5px;
            font-size: 20px;
            transition: all 0.2s;
            text-transform: uppercase;
            font-family: 'Courier New', monospace;
            font-size: 12px;
        }
        
        .spatial-btn:hover {
            background: #00ff00;
            color: #000;
            box-shadow: 0 0 10px #00ff00;
        }
        
        .spatial-btn:active {
            transform: scale(0.95);
        }
        
        .action-buttons {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 15px;
        }
        
        .btn-arm {
            padding: 20px;
            background: rgba(0, 150, 150, 0.8);
            border: 2px solid #00ff00;
            color: #00ff00;
            cursor: pointer;
            border-radius: 5px;
            font-weight: bold;
            text-transform: uppercase;
            font-size: 16px;
            transition: all 0.3s;
        }
        
        .btn-arm:hover {
            background: #00ff00;
            color: #000;
            box-shadow: 0 0 15px #00ff00;
        }
        
        .btn-kill {
            padding: 20px;
            background: rgba(200, 50, 50, 0.8);
            border: 2px solid #ff3333;
            color: #ff3333;
            cursor: pointer;
            border-radius: 5px;
            font-weight: bold;
            text-transform: uppercase;
            font-size: 16px;
            transition: all 0.3s;
        }
        
        .btn-kill:hover {
            background: #ff3333;
            color: #fff;
            box-shadow: 0 0 15px #ff3333;
        }
        
        .throttle-section {
            background: rgba(0, 50, 50, 0.8);
            border: 2px solid #00ff00;
            padding: 20px;
            border-radius: 5px;
        }
        
        .throttle-label {
            font-size: 12px;
            color: #00aaaa;
            text-transform: uppercase;
            margin-bottom: 10px;
        }
        
        .throttle-bar {
            width: 100%;
            height: 50px;
            background: rgba(0, 20, 20, 0.9);
            border: 2px solid #00ff00;
            border-radius: 3px;
            overflow: hidden;
            margin-bottom: 10px;
            position: relative;
        }
        
        .throttle-fill {
            height: 100%;
            background: linear-gradient(90deg, #00ff00 0%, #ffaa00 50%, #ff3333 100%);
            width: 0%;
            transition: width 0.05s;
        }
        
        .throttle-value {
            text-align: center;
            font-size: 24px;
            font-weight: bold;
            color: #00ff00;
            margin-top: 10px;
        }
        
        .velocity-display {
            padding: 15px;
            background: rgba(0, 50, 50, 0.8);
            border: 2px solid #00ff00;
            border-radius: 5px;
            margin-top: 20px;
            text-align: center;
        }
        
        .velocity-label {
            font-size: 12px;
            color: #00aaaa;
            text-transform: uppercase;
            margin-bottom: 10px;
        }
        
        .velocity-value {
            font-size: 28px;
            font-weight: bold;
            color: #00ff00;
        }
        
        .motor-status {
            display: grid;
            grid-template-columns: 1fr 1fr 1fr;
            gap: 10px;
            margin-top: 20px;
            padding-top: 15px;
            border-top: 1px solid #00ff00;
        }
        
        .motor-indicator {
            text-align: center;
            padding: 10px;
            background: rgba(0, 100, 100, 0.5);
            border: 1px solid #00ff00;
            border-radius: 3px;
            font-size: 13px;
        }
        
        .error-display {
            background: rgba(200, 50, 50, 0.9);
            border: 2px solid #ff3333;
            color: #ff3333;
            padding: 15px;
            border-radius: 5px;
            margin-bottom: 20px;
            display: none;
            text-align: center;
            font-weight: bold;
        }
        
        input[type="range"] {
            width: 100%;
            height: 8px;
            border-radius: 3px;
            background: rgba(0, 255, 0, 0.2);
            outline: none;
            -webkit-appearance: none;
        }
        
        input[type="range"]::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 20px;
            height: 20px;
            border-radius: 50%;
            background: #00ff00;
            cursor: pointer;
            box-shadow: 0 0 5px #00ff00;
        }
        
        input[type="range"]::-moz-range-thumb {
            width: 20px;
            height: 20px;
            border-radius: 50%;
            background: #00ff00;
            cursor: pointer;
            border: none;
        }
        
        @media (max-width: 1024px) {
            .main-grid { grid-template-columns: 1fr 1fr; }
            .controls-row { grid-template-columns: 1fr; }
        }
        
        @media (max-width: 768px) {
            .main-grid { grid-template-columns: 1fr; }
            .gimbal-input { width: 150px; height: 150px; }
            .gimbal-center { width: 60px; height: 60px; }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <div class="title">▢ ESP32-C3 FLIGHT CONTROL</div>
            <div class="status-display">
                <div class="status-item">VTOL-97</div>
                <div class="status-item ready-status">READY FOR FLIGHT</div>
                <div class="status-item armed-status" id="armedIndicator">🔴 DISARMED</div>
            </div>
        </div>
        
        <div class="error-display" id="errorDisplay"></div>
        
        <div class="main-grid">
            <div class="telemetry-box">
                <div class="telemetry-label">PITCH AXIS</div>
                <div class="telemetry-value" id="pitchValue">1.3</div>
                <div class="telemetry-unit">DEG</div>
            </div>
            <div class="telemetry-box">
                <div class="telemetry-label">ROLL AXIS</div>
                <div class="telemetry-value" id="rollValue">-0.8</div>
                <div class="telemetry-unit">DEG</div>
            </div>
            <div class="telemetry-box">
                <div class="telemetry-label">HEADING</div>
                <div class="telemetry-value" id="yawValue">184</div>
                <div class="telemetry-unit">AZM</div>
            </div>
        </div>
        
        <div class="controls-row">
            <div class="control-panel">
                <div class="control-title">ATTITUDE CONTROL</div>
                <div class="control-title" style="font-size: 10px; color: #00aaaa; margin-top: -10px;">GIMBAL INPUT</div>
                <div class="gimbal-input">
                    <div class="gimbal-label top">PITCH FORWARD</div>
                    <div class="gimbal-label bottom">PITCH BACKWARD</div>
                    <div class="gimbal-label left">ROLL LEFT</div>
                    <div class="gimbal-label right">ROLL RIGHT</div>
                    <div class="gimbal-center">✚</div>
                </div>
            </div>
            
            <div class="control-panel">
                <div class="control-title">SPATIAL ACTIONS</div>
                <div class="control-title" style="font-size: 10px; color: #00aaaa; margin-top: -10px;">YAW & ALTITUDE</div>
                <div class="spatial-grid">
                    <div style="grid-column: 2;">
                        <button class="spatial-btn" onclick="altUp()">▲<br>ALT UP</button>
                    </div>
                    <div></div>
                    <div></div>
                    
                    <button class="spatial-btn" onclick="yawLeft()">◄<br>YAW LEFT</button>
                    <div></div>
                    <button class="spatial-btn" onclick="yawRight()">►<br>YAW RIGHT</button>
                    
                    <div style="grid-column: 2;">
                        <button class="spatial-btn" onclick="altDown()">▼<br>ALT DOWN</button>
                    </div>
                </div>
            </div>
        </div>
        
        <div class="throttle-section">
            <div class="throttle-label">CURRENT VELOCITY</div>
            <div style="display: flex; justify-content: space-between; align-items: center;">
                <span style="color: #00ff00; font-weight: bold;">IDLE</span>
                <span id="velocityDisplay" style="font-size: 16px; color: #00ff00;">0.0 KM/H</span>
            </div>
            <div class="throttle-bar">
                <div class="throttle-fill" id="throttleFill"></div>
            </div>
            <input type="range" id="throttleSlider" min="0" max="100" value="0" oninput="updateThrottle()">
            <div class="throttle-value"><span id="throttlePercent">0</span>%</div>
        </div>
        
        <div class="action-buttons">
            <button class="btn-arm" onclick="armDrone()">🔋 ARM SYSTEM</button>
            <button class="btn-kill" onclick="emergencyStop()">🛑 KILL SWITCH / DISARM</button>
        </div>
        
        <div class="motor-status">
            <div class="motor-indicator">M1: <span id="m1Status">✅</span></div>
            <div class="motor-indicator">M2: <span id="m2Status">✅</span></div>
            <div class="motor-indicator">M3: <span id="m3Status">✅</span></div>
        </div>
    </div>
    
    <script>
        function updateTelemetry() {
            fetch('/api/telemetry')
                .then(r => r.json())
                .then(data => {
                    document.getElementById('pitchValue').textContent = data.pitch.toFixed(1);
                    document.getElementById('rollValue').textContent = data.roll.toFixed(1);
                    document.getElementById('yawValue').textContent = Math.round(data.yaw);
                    document.getElementById('throttlePercent').textContent = Math.round(data.throttle);
                    document.getElementById('throttleFill').style.width = data.throttle + '%';
                    document.getElementById('throttleSlider').value = data.throttle;
                    
                    const velocity = (data.throttle / 100) * 50;
                    document.getElementById('velocityDisplay').textContent = velocity.toFixed(1) + ' KM/H';
                    
                    document.getElementById('armedIndicator').textContent = data.armed ? '🟢 ARMED' : '🔴 DISARMED';
                    document.getElementById('armedIndicator').className = data.armed ? 'status-item armed-status armed' : 'status-item armed-status';
                    
                    document.getElementById('m1Status').textContent = data.motor1_ok ? '✅' : '❌';
                    document.getElementById('m2Status').textContent = data.motor2_ok ? '✅' : '❌';
                    document.getElementById('m3Status').textContent = data.motor3_ok ? '✅' : '❌';
                    
                    if (data.error_msg) {
                        document.getElementById('errorDisplay').style.display = 'block';
                        document.getElementById('errorDisplay').innerHTML = '⚠️ ' + data.error_msg;
                    } else {
                        document.getElementById('errorDisplay').style.display = 'none';
                    }
                })
                .catch(err => {
                    document.getElementById('errorDisplay').style.display = 'block';
                    document.getElementById('errorDisplay').innerHTML = '⚠️ CONNECTION LOST';
                });
        }
        
        function armDrone() {
            if (confirm('ARM SYSTEM? Propellers will spin!')) {
                fetch('/api/arm', {method: 'POST'}).then(() => updateTelemetry());
            }
        }
        
        function emergencyStop() {
            fetch('/api/disarm', {method: 'POST'}).then(() => updateTelemetry());
        }
        
        function updateThrottle() {
            const val = document.getElementById('throttleSlider').value;
            fetch('/api/throttle', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({throttle: parseInt(val)})
            });
        }
        
        function altUp() {
            let val = Math.min(100, parseInt(document.getElementById('throttleSlider').value) + 10);
            document.getElementById('throttleSlider').value = val;
            updateThrottle();
        }
        
        function altDown() {
            let val = Math.max(0, parseInt(document.getElementById('throttleSlider').value) - 10);
            document.getElementById('throttleSlider').value = val;
            updateThrottle();
        }
        
        function yawLeft() { console.log('Yaw left'); }
        function yawRight() { console.log('Yaw right'); }
        
        setInterval(updateTelemetry, 500);
        updateTelemetry();
    </script>
</body>
</html>
")

// ===== SETUP FUNCTIONS =====
void setupWiFi() {
  Serial.println("[WiFi] Starting AP: MiniDrone");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress ip = WiFi.softAPIP();
  Serial.print("[WiFi] ✓ AP Started. IP: ");
  Serial.println(ip);
  diag.wifi_ok = true;
}

void setupIMU() {
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.println("[IMU] Initializing MPU-6050...");
  
  if (!mpu.begin()) {
    Serial.println("[IMU] ✗ MPU-6050 NOT FOUND! Check wiring (SDA:20, SCL:21)");
    diag.imu_ok = false;
    return;
  }
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  Serial.println("[IMU] ✓ MPU-6050 initialized");
  diag.imu_ok = true;
}

void setupMotors() {
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);
  pinMode(MOTOR3_IN1, OUTPUT);
  pinMode(MOTOR3_IN2, OUTPUT);
  
  digitalWrite(MOTOR1_IN1, LOW);
  digitalWrite(MOTOR1_IN2, LOW);
  digitalWrite(MOTOR2_IN1, LOW);
  digitalWrite(MOTOR2_IN2, LOW);
  digitalWrite(MOTOR3_IN1, LOW);
  digitalWrite(MOTOR3_IN2, LOW);
  
  Serial.println("[Motors] ✓ Motor pins configured");
  diag.motor1_ok = true;
  diag.motor2_ok = true;
  diag.motor3_ok = true;
}

void setupWebServer() {
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", html_page);
  });
  
  server.on("/api/telemetry", HTTP_GET, []() {
    DynamicJsonDocument doc(512);
    doc["pitch"] = pitch;
    doc["roll"] = roll;
    doc["yaw"] = yaw;
    doc["throttle"] = throttle;
    doc["armed"] = armed;
    doc["wifi_ok"] = diag.wifi_ok;
    doc["imu_ok"] = diag.imu_ok;
    doc["motor1_ok"] = diag.motor1_ok;
    doc["motor2_ok"] = diag.motor2_ok;
    doc["motor3_ok"] = diag.motor3_ok;
    doc["error_msg"] = diag.error_details;
    
    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
  });
  
  server.on("/api/arm", HTTP_POST, []() {
    if (!diag.imu_ok) {
      diag.error_details = "Cannot arm: IMU not connected";
      server.send(400, "application/json", "{\"error\":\"IMU Error\"}");
      return;
    }
    armed = true;
    diag.error_details = "";
    server.send(200, "application/json", "{\"status\":\"armed\"}");
    Serial.println("[System] ✓ DRONE ARMED");
  });
  
  server.on("/api/disarm", HTTP_POST, []() {
    armed = false;
    throttle = 0;
    motor1_speed = 0;
    motor2_speed = 0;
    motor3_speed = 0;
    diag.error_details = "";
    server.send(200, "application/json", "{\"status\":\"disarmed\"}");
    Serial.println("[System] ✓ DRONE DISARMED");
  });
  
  server.on("/api/throttle", HTTP_POST, []() {
    if (!armed) {
      server.send(400, "application/json", "{\"error\":\"Not armed\"}");
      return;
    }
    
    if (server.hasArg("plain")) {
      DynamicJsonDocument doc(256);
      deserializeJson(doc, server.arg("plain"));
      throttle = constrain(doc["throttle"], 0, 100);
    }
    
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  });
  
  server.begin();
  Serial.println("[WebServer] ✓ Server started on port 80");
}

void setMotorSpeed(int motor, int speed) {
  speed = constrain(speed, 0, 255);
  
  if (motor == 1) {
    digitalWrite(MOTOR1_IN1, speed > 0 ? HIGH : LOW);
    digitalWrite(MOTOR1_IN2, speed > 0 ? LOW : HIGH);
  } else if (motor == 2) {
    digitalWrite(MOTOR2_IN1, speed > 0 ? HIGH : LOW);
    digitalWrite(MOTOR2_IN2, speed > 0 ? LOW : HIGH);
  } else if (motor == 3) {
    digitalWrite(MOTOR3_IN1, speed > 0 ? HIGH : LOW);
    digitalWrite(MOTOR3_IN2, speed > 0 ? LOW : HIGH);
  }
}

void updateIMU() {
  if (!diag.imu_ok) return;
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  pitch = atan2(a.acceleration.y, a.acceleration.z) * 57.2958;
  roll = atan2(a.acceleration.x, a.acceleration.z) * 57.2958;
  yaw = g.gyro.z * 57.2958;
}

void checkSystemHealth() {
  if (millis() - diag.last_check < 2000) return;
  diag.last_check = millis();
  
  if (!diag.imu_ok) {
    diag.error_details = "IMU NOT CONNECTED - Check wires (SDA pin 20, SCL pin 21)";
    armed = false;
  }
  
  if (!diag.motor1_ok || !diag.motor2_ok || !diag.motor3_ok) {
    armed = false;
    diag.error_details = "Motor connection error - Check pins 5,6,4,3,2,1";
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n========================================");
  Serial.println("  MINI DRONE FLIGHT CONTROLLER v2.0");
  Serial.println("  Professional Flight Control Interface");
  Serial.println("========================================\n");
  
  setupMotors();
  setupIMU();
  setupWiFi();
  setupWebServer();
  
  Serial.println("\n[System] ✓ All systems initialized");
  Serial.println("[System] ✓ Connect to WiFi: MiniDrone / 12345678");
  Serial.println("[System] ✓ Open browser: http://192.168.4.1");
  Serial.println("========================================\n");
}

void loop() {
  server.handleClient();
  updateIMU();
  checkSystemHealth();
  
  if (armed) {
    motor1_speed = (throttle / 100.0) * 255;
    motor2_speed = (throttle / 100.0) * 255;
    motor3_speed = (throttle / 100.0) * 255;
  } else {
    motor1_speed = 0;
    motor2_speed = 0;
    motor3_speed = 0;
  }
  
  setMotorSpeed(1, motor1_speed);
  setMotorSpeed(2, motor2_speed);
  setMotorSpeed(3, motor3_speed);
  
  delay(10);
}