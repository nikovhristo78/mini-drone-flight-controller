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
const char* default_ip = "192.168.4.1";

// System Variables
MPU6050 mpu;
WebServer server(80);
float pitch = 0, roll = 0, yaw = 0;
float throttle = 0;
int motor1_speed = 0, motor2_speed = 0, motor3_speed = 0;
bool armed = false;
unsigned long last_telemetry = 0;
unsigned long system_start_time = 0;

// Error codes & diagnostics
struct SystemDiagnostics {
  bool wifi_ok = false;
  bool imu_ok = false;
  bool motor1_ok = true;
  bool motor2_ok = true;
  bool motor3_ok = true;
  bool battery_ok = true;
  String connection_status = "INITIALIZING";
  String error_details = "";
  unsigned long last_check = 0;
  int connection_attempts = 0;
  String imu_address = "0x68";
  String last_error = "";
} diag;

struct ConnectionTest {
  bool power_ok = true;
  bool imu_detected = false;
  bool motor1_ok = true;
  bool motor2_ok = true;
  bool motor3_ok = true;
  bool wifi_broadcasting = false;
  String test_log = "";
} test_results;

// HTML Dashboard
const char* html_page = R"(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>🚁 Mini Drone Controller</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { font-family: 'Arial', sans-serif; background: linear-gradient(135deg, #0f0f1e 0%, #1a1a2e 100%); color: #fff; min-height: 100vh; }
        .container { max-width: 900px; margin: 0 auto; padding: 15px; }
        .header { text-align: center; padding: 20px 0; border-bottom: 3px solid #00ff00; margin-bottom: 20px; }
        .title { font-size: 28px; font-weight: bold; color: #00ff00; text-shadow: 0 0 10px #00ff00; }
        .tabs { display: flex; gap: 10px; margin-bottom: 20px; flex-wrap: wrap; }
        .tab-btn { padding: 12px 20px; background: #333; border: 2px solid #555; color: #aaa; cursor: pointer; border-radius: 5px; font-weight: bold; }
        .tab-btn.active { background: #00ff00; color: #000; border-color: #00ff00; }
        .tab-content { display: none; }
        .tab-content.active { display: block; }
        .status-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 15px; margin-bottom: 20px; }
        .status-card { background: #2a2a3e; padding: 15px; border-radius: 8px; border-left: 4px solid #00ff00; }
        .status-label { font-size: 12px; color: #888; text-transform: uppercase; margin-bottom: 5px; }
        .status-value { font-size: 18px; font-weight: bold; color: #00ff00; }
        .telemetry-grid { display: grid; grid-template-columns: 1fr 1fr 1fr; gap: 15px; margin-bottom: 20px; }
        .telemetry-box { background: #333; padding: 15px; border-radius: 8px; text-align: center; border: 2px solid #444; }
        .telemetry-value { font-size: 24px; font-weight: bold; color: #00ff00; }
        .control-section { background: #2a2a3e; padding: 20px; border-radius: 8px; margin-bottom: 20px; }
        .control-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 15px; margin-bottom: 15px; }
        .btn { padding: 15px; font-size: 14px; font-weight: bold; border: none; border-radius: 8px; cursor: pointer; text-transform: uppercase; }
        .btn-primary { background: #00ff00; color: #000; }
        .btn-danger { background: #ff4444; color: #fff; }
        .btn-info { background: #0088ff; color: #fff; }
        .throttle-bar { width: 100%; height: 30px; background: #1a1a2e; border-radius: 5px; overflow: hidden; margin: 10px 0; border: 2px solid #444; }
        .throttle-fill { height: 100%; background: linear-gradient(90deg, #00ff00 0%, #ffaa00 50%, #ff4444 100%); width: 0%; }
        .slider { width: 100%; }
        .error-box { background: #dd0000; padding: 15px; border-radius: 8px; margin-bottom: 20px; display: none; }
        .motor-status { display: grid; grid-template-columns: 1fr 1fr 1fr; gap: 10px; margin-top: 15px; }
        .motor-led { padding: 10px; border-radius: 5px; text-align: center; font-weight: bold; }
        .motor-led.ok { background: #00ff00; color: #000; }
        .motor-led.error { background: #ff4444; color: #fff; }
        .connection-section { background: #2a2a3e; padding: 15px; border-radius: 8px; margin-bottom: 20px; border: 2px solid #00ff00; }
        .connection-section h3 { color: #00ff00; margin-bottom: 10px; }
        .ip-input-group { display: flex; gap: 10px; margin-bottom: 10px; }
        .ip-input-group input { flex: 1; padding: 10px; background: #1a1a2e; border: 2px solid #444; color: #fff; border-radius: 5px; }
        .ip-input-group button { padding: 10px 20px; background: #00ff00; color: #000; border: none; border-radius: 5px; cursor: pointer; font-weight: bold; }
        .test-item { padding: 10px; margin-bottom: 8px; background: #1a1a2e; border-left: 3px solid #666; }
        .test-item.pass { border-left-color: #00ff00; background: rgba(0, 255, 0, 0.1); }
        .test-item.fail { border-left-color: #ff4444; background: rgba(255, 68, 68, 0.1); }
        @media (max-width: 600px) {
            .telemetry-grid { grid-template-columns: 1fr; }
            .status-grid { grid-template-columns: 1fr; }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <div class="title">🚁 MINI DRONE FLIGHT CONTROLLER</div>
        </div>
        
        <div class="tabs">
            <button class="tab-btn active" onclick="showTab('flight')">✈️ FLIGHT</button>
            <button class="tab-btn" onclick="showTab('diagnostics')">🔧 DIAGNOSTICS</button>
            <button class="tab-btn" onclick="showTab('connection')">🌐 CONNECTION</button>
        </div>
        
        <div id="flight" class="tab-content active">
            <div class="error-box" id="errorBox"></div>
            
            <div class="status-grid">
                <div class="status-card">
                    <div class="status-label">Flight Status</div>
                    <div class="status-value" id="flightStatus">DISARMED</div>
                </div>
                <div class="status-card">
                    <div class="status-label">System Health</div>
                    <div class="status-value" id="systemHealth">CHECKING...</div>
                </div>
            </div>
            
            <div class="telemetry-grid">
                <div class="telemetry-box">
                    <div>PITCH</div>
                    <div class="telemetry-value" id="pitch">0.0°</div>
                </div>
                <div class="telemetry-box">
                    <div>ROLL</div>
                    <div class="telemetry-value" id="roll">0.0°</div>
                </div>
                <div class="telemetry-box">
                    <div>YAW RATE</div>
                    <div class="telemetry-value" id="yaw">0.0°/s</div>
                </div>
            </div>
            
            <div class="control-section">
                <div>Throttle: <span id="throttlePercent">0</span>%</div>
                <div class="throttle-bar">
                    <div class="throttle-fill" id="throttleFill"></div>
                </div>
                <input type="range" id="throttle" class="slider" min="0" max="100" value="0" oninput="updateThrottle()">
                <div class="control-grid" style="margin-top: 15px;">
                    <button class="btn btn-primary" onclick="armDrone()">ARM</button>
                    <button class="btn btn-danger" onclick="disarmDrone()">STOP</button>
                    <button class="btn btn-info" onclick="increaseThrottle()">+</button>
                    <button class="btn btn-info" onclick="decreaseThrottle()">-</button>
                </div>
                <div class="motor-status" id="motorStatus">
                    <div class="motor-led ok">M1: ✅</div>
                    <div class="motor-led ok">M2: ✅</div>
                    <div class="motor-led ok">M3: ✅</div>
                </div>
            </div>
        </div>
        
        <div id="diagnostics" class="tab-content">
            <div id="testResults" style="margin-bottom: 20px;"></div>
            <button class="btn btn-info" style="width: 100%;" onclick="runTests()">RUN TESTS</button>
        </div>
        
        <div id="connection" class="tab-content">
            <div class="connection-section">
                <h3>📍 Your IP Address</h3>
                <p style="font-size: 18px; color: #00ff00; font-family: monospace;" id="currentIP">Loading...</p>
            </div>
            
            <div class="connection-section">
                <h3>🔌 Connect to Another IP</h3>
                <div class="ip-input-group">
                    <input type="text" id="customIP" placeholder="Enter IP (e.g., 192.168.1.100)">
                    <button onclick="connectToIP()">CONNECT</button>
                </div>
            </div>
            
            <div class="connection-section">
                <h3>📡 Status</h3>
                <p>WiFi SSID: <strong>MiniDrone</strong></p>
                <p>Password: <strong>12345678</strong></p>
                <p>IMU: <span id="imuStatus">Checking...</span></p>
                <p>Motor 1: <span id="motor1Status">OK</span></p>
                <p>Motor 2: <span id="motor2Status">OK</span></p>
                <p>Motor 3: <span id="motor3Status">OK</span></p>
            </div>
        </div>
    </div>
    
    <script>
        const local_ip = window.location.hostname;
        document.getElementById('currentIP').textContent = local_ip;
        
        function showTab(tabName) {
            document.querySelectorAll('.tab-content').forEach(t => t.classList.remove('active'));
            document.querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));
            document.getElementById(tabName).classList.add('active');
            event.target.classList.add('active');
        }
        
        function updateTelemetry() {
            fetch('/api/telemetry')
                .then(r => r.json())
                .then(data => {
                    document.getElementById('pitch').textContent = data.pitch.toFixed(1) + '°';
                    document.getElementById('roll').textContent = data.roll.toFixed(1) + '°';
                    document.getElementById('yaw').textContent = data.yaw.toFixed(1) + '°/s';
                    document.getElementById('throttlePercent').textContent = data.throttle.toFixed(0);
                    document.getElementById('throttleFill').style.width = data.throttle + '%';
                    
                    document.getElementById('flightStatus').textContent = data.armed ? '🟢 ARMED' : '🔴 DISARMED';
                    const health = data.wifi_ok && data.imu_ok ? '✅ OK' : '⚠️ CHECK';
                    document.getElementById('systemHealth').textContent = health;
                    
                    document.getElementById('motorStatus').innerHTML = `
                        <div class="motor-led ${data.motor1_ok ? 'ok' : 'error'}">M1: ${data.motor1_ok ? '✅' : '❌'}</div>
                        <div class="motor-led ${data.motor2_ok ? 'ok' : 'error'}">M2: ${data.motor2_ok ? '✅' : '❌'}</div>
                        <div class="motor-led ${data.motor3_ok ? 'ok' : 'error'}">M3: ${data.motor3_ok ? '✅' : '❌'}</div>
                    `;
                    
                    document.getElementById('imuStatus').textContent = data.imu_ok ? '✅ Connected' : '❌ Not Found';
                    document.getElementById('motor1Status').textContent = data.motor1_ok ? '✅ OK' : '❌ Error';
                    document.getElementById('motor2Status').textContent = data.motor2_ok ? '✅ OK' : '❌ Error';
                    document.getElementById('motor3Status').textContent = data.motor3_ok ? '✅ OK' : '❌ Error';
                    
                    if (data.error_msg) {
                        document.getElementById('errorBox').style.display = 'block';
                        document.getElementById('errorBox').innerHTML = '⚠️ ' + data.error_msg;
                    } else {
                        document.getElementById('errorBox').style.display = 'none';
                    }
                })
                .catch(() => {
                    document.getElementById('errorBox').style.display = 'block';
                    document.getElementById('errorBox').innerHTML = '❌ Connection Lost';
                });
        }
        
        function armDrone() {
            fetch('/api/arm', {method: 'POST'}).then(() => updateTelemetry());
        }
        
        function disarmDrone() {
            fetch('/api/disarm', {method: 'POST'}).then(() => updateTelemetry());
        }
        
        function increaseThrottle() {
            let val = Math.min(100, parseInt(document.getElementById('throttle').value) + 10);
            document.getElementById('throttle').value = val;
            updateThrottle();
        }
        
        function decreaseThrottle() {
            let val = Math.max(0, parseInt(document.getElementById('throttle').value) - 10);
            document.getElementById('throttle').value = val;
            updateThrottle();
        }
        
        function updateThrottle() {
            let val = document.getElementById('throttle').value;
            fetch('/api/throttle', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({throttle: parseInt(val)})
            });
        }
        
        function runTests() {
            fetch('/api/test').then(r => r.json()).then(data => {
                let html = '<h3>Test Results:</h3>';
                html += '<div class="test-item ' + (data.power_ok ? 'pass' : 'fail') + '">Power: ' + (data.power_ok ? '✅ OK' : '❌ ERROR') + '</div>';
                html += '<div class="test-item ' + (data.imu_detected ? 'pass' : 'fail') + '">IMU (Pins 20,21): ' + (data.imu_detected ? '✅ FOUND' : '❌ NOT FOUND') + '</div>';
                html += '<div class="test-item ' + (data.motor1_ok ? 'pass' : 'fail') + '">Motor 1 (Pins 5,6): ' + (data.motor1_ok ? '✅ OK' : '❌ ERROR') + '</div>';
                html += '<div class="test-item ' + (data.motor2_ok ? 'pass' : 'fail') + '">Motor 2 (Pins 4,3): ' + (data.motor2_ok ? '✅ OK' : '❌ ERROR') + '</div>';
                html += '<div class="test-item ' + (data.motor3_ok ? 'pass' : 'fail') + '">Motor 3 (Pins 2,1): ' + (data.motor3_ok ? '✅ OK' : '❌ ERROR') + '</div>';
                html += '<div class="test-item ' + (data.wifi_broadcasting ? 'pass' : 'fail') + '">WiFi: ' + (data.wifi_broadcasting ? '✅ BROADCASTING' : '❌ ERROR') + '</div>';
                document.getElementById('testResults').innerHTML = html;
            });
        }
        
        function connectToIP() {
            const ip = document.getElementById('customIP').value;
            if (!ip) return;
            window.location.href = 'http://' + ip;
        }
        
        setInterval(updateTelemetry, 500);
        updateTelemetry();
        runTests();
    </script>
</body>
</html>
")