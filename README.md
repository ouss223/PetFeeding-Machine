# Pet Feeding Machine

An automated pet feeding system built with ESP32 microcontroller and React web interface. This IoT solution allows pet owners to control and monitor their pet's food and water dispensing either manually or through scheduled feedings.

##  Table of Contents

- [Features](#features)
- [Architecture](#architecture)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Project Structure](#project-structure)
- [Installation](#installation)
  - [Backend Setup (ESP32)](#backend-setup-esp32)
  - [Frontend Setup (React)](#frontend-setup-react)
- [Usage](#usage)
- [API Endpoints](#api-endpoints)
- [Integration](#integration)
- [Simulation](#simulation)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

##  Features

- **Dual Operating Modes**:
  - **Manual Mode**: Dispense food and water on-demand through the web interface
  - **Scheduled Mode**: Automatic feeding at pre-configured times (default: 8:00 AM, 4:00 PM, 10:00 PM)

- **Real-time Monitoring**:
  - Food and water stock levels tracking
  - Food consumption statistics
  - Served quantities monitoring
  - Weight sensors for accurate measurements

- **Smart Features**:
  - LCD display (20x4) showing current status
  - RTC (Real-Time Clock) for scheduled operations
  - Servo motor for precise food dispensing
  - Ultrasonic sensors for level detection
  - HX711 load cells for weight measurements

- **Remote Access**:
  - Web-based control interface
  - MQTT integration for IoT connectivity
  - Alexa skill integration (ask "Alexa, ask Pet Feeder how full is the food container")

##  Architecture

The system consists of three main components:

```
┌─────────────────┐
│  React Frontend │ ◄──── HTTP ────┐
│  (Port 3000)    │                │
└─────────────────┘                │
                                   │
┌─────────────────┐                │
│  ESP32 Backend  │ ◄──────────────┘
│  (Port 80/8180) │
└────────┬────────┘
         │
         ├── Sensors (Ultrasonic, Weight)
         ├── Servo Motor
         ├── LCD Display
         ├── RTC Module
         └── MQTT Broker
```

### Backend (ESP32)
- **Framework**: Arduino/PlatformIO
- **Web Server**: ESP32 HTTP Server
- **Communication**: WiFi, HTTP REST API, MQTT
- **Sensors**: HC-SR04 ultrasonic sensors, HX711 load cells
- **Actuators**: Servo motor for food dispensing
- **Display**: I2C LCD 20x4
- **Timekeeping**: DS1307 RTC module

### Frontend (React)
- **Framework**: React 18.3.1
- **Routing**: React Router DOM
- **UI Components**: React Icons, Recharts for data visualization
- **Styling**: Tailwind CSS
- **State Management**: React Hooks

##  Hardware Requirements

### Electronics Components

| Component | Quantity | Purpose |
|-----------|----------|---------|
| ESP32 DevKit | 1 | Main microcontroller |
| HC-SR04 Ultrasonic Sensor | 2-4 | Distance/level detection |
| HX711 Load Cell Amplifier | 3 | Weight measurement |
| Load Cell (5kg) | 3 | Weight sensing |
| Servo Motor | 1 | Food dispensing mechanism |
| DS1307 RTC Module | 1 | Real-time clock |
| LCD 20x4 with I2C | 1 | Status display |
| Jumper Wires | Multiple | Connections |

### Pin Configuration

See `Back/src/Pins.h` for detailed pin mapping:
- **Servo**: GPIO 15
- **Ultrasonic Sensors**: GPIOs 2, 4, 16, 17
- **I2C (LCD)**: SDA=GPIO 33, SCL=GPIO 32
- **I2C (RTC)**: SDA=GPIO 21, SCL=GPIO 22
- **HX711 Load Cells**: GPIOs 12, 14, 18, 19, 25, 26

##  Software Requirements

### Backend
- [PlatformIO](https://platformio.org/) (or Arduino IDE)
- [Wokwi VS Code Extension](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode) (for simulation)

### Frontend
- Node.js (v14 or higher)
- npm or yarn

##  Project Structure

```
PetFeeding-Machine/
├── Back/                   # ESP32 Backend
│   ├── src/
│   │   ├── esp32-http-server.ino  # Main Arduino sketch
│   │   ├── Machine.h/cpp          # Machine state management
│   │   ├── ServerHandlers.h/cpp   # HTTP request handlers
│   │   └── Pins.h                 # Pin definitions
│   ├── platformio.ini             # PlatformIO configuration
│   ├── wokwi.toml                 # Wokwi simulator config
│   ├── diagram.json               # Hardware wiring diagram
│   └── README.md                  # Backend documentation
│
├── Front/                  # React Frontend
│   ├── src/
│   │   ├── components/
│   │   │   ├── Landing.js         # Mode selection page
│   │   │   ├── First.js           # Manual mode interface
│   │   │   ├── Second.js          # Scheduled mode interface
│   │   │   └── FoodStats.js       # Statistics component
│   │   └── App.js                 # Main React app
│   ├── package.json               # Dependencies
│   ├── tailwind.config.js         # Tailwind configuration
│   └── README.md                  # Frontend documentation
│
└── README.md               # This file
```

##  Installation

### Backend Setup (ESP32)

1. **Install PlatformIO**:
   ```bash
   # Using pip
   pip install platformio
   
   # Or install VS Code extension
   # Search for "PlatformIO IDE" in VS Code extensions
   ```

2. **Clone the repository**:
   ```bash
   git clone https://github.com/ouss223/PetFeeding-Machine.git
   cd PetFeeding-Machine/Back
   ```

3. **Install dependencies**:
   ```bash
   pio lib install
   ```

4. **Build the project**:
   ```bash
   pio run
   ```

5. **Upload to ESP32**:
   ```bash
   pio run --target upload
   ```

   Or for simulation (see [Simulation](#simulation) section below).

6. **Configure WiFi**:
   - Update `WIFI_SSID` and `WIFI_PASSWORD` in `esp32-http-server.ino`
   - Default simulation uses "Wokwi-GUEST" with no password

### Frontend Setup (React)

1. **Navigate to frontend directory**:
   ```bash
   cd Front
   ```

2. **Install dependencies**:
   ```bash
   npm install
   ```

3. **Configure API endpoint**:
   - Update `ESP32_API` in `src/components/Landing.js` if needed
   - Default: `http://localhost:8180` (for simulation)
   - Production: Update to your ESP32's IP address

4. **Start development server**:
   ```bash
   npm start
   ```

   The app will open at [http://localhost:3000](http://localhost:3000)

5. **Build for production**:
   ```bash
   npm run build
   ```

## 📖 Usage

### Getting Started

1. **Power up the system**: Connect ESP32 to power supply
2. **Connect to WiFi**: ESP32 will connect to configured network
3. **Open web interface**: Navigate to ESP32's IP address (or localhost:8180 for simulation)
4. **Select operating mode**:
   - Click "Manual Mode" for on-demand feeding
   - Click "Scheduled Mode" for automatic feeding

### Manual Mode

In manual mode, you can:
- View current stock levels (food and water)
- View served amounts
- Manually trigger food dispensing
- Manually trigger water dispensing
- Monitor food consumption statistics

### Scheduled Mode

In scheduled mode:
- Food is automatically dispensed at configured times
- Default schedule: 8:00 AM, 4:00 PM, 10:00 PM
- View and modify feeding schedule through the interface
- Monitor feeding history and statistics
- Track consumption patterns over time

### Alexa Integration

Ask Alexa: "Alexa, ask Pet Feeder how full is the food container"
- Requires Alexa skill setup (not included in this repository)
- Returns current food stock percentage

### MQTT Monitoring

Connect to MQTT broker to monitor real-time data:
- **Broker**: `broker.mqtt.cool`
- **Port**: 1883
- **Topic**: `petfeeder/foodstock`
- **Web Client**: [https://testclient-cloud.mqtt.cool](https://testclient-cloud.mqtt.cool)

## � API Endpoints

The ESP32 exposes the following HTTP endpoints:

| Method | Endpoint | Parameters | Description |
|--------|----------|------------|-------------|
| GET | `/getMode` | - | Get current operating mode (1 or 2) |
| POST | `/setMode` | `mode=1\|2` | Set operating mode (1=manual, 2=scheduled) |
| GET | `/getStockFood` | - | Get food stock level |
| GET | `/getStockWater` | - | Get water stock level |
| GET | `/getServedFood` | - | Get amount of food served |
| GET | `/getServedWater` | - | Get amount of water served |
| POST | `/setStockFood` | `stock=<value>` | Update food stock level |
| POST | `/setStockWater` | `stock=<value>` | Update water stock level |
| POST | `/dispenseFood` | - | Trigger food dispensing |
| POST | `/dispenseWater` | - | Trigger water dispensing |
| GET | `/getSchedule` | - | Get feeding schedule |
| POST | `/setSchedule` | JSON array | Update feeding schedule |
| GET | `/getFoodConsumption` | - | Get total food consumed |

##  Integration

### MQTT Setup

The device publishes data to MQTT broker for IoT integration:

```
Broker: broker.mqtt.cool
Port: 1883
Topic: petfeeder/foodstock
```

To monitor messages, use any MQTT client or the web interface at [testclient-cloud.mqtt.cool](https://testclient-cloud.mqtt.cool).

### Alexa Skill Integration

The system is designed to work with a custom Alexa skill. To integrate:

1. Create an Alexa skill in the Amazon Developer Console
2. Configure the skill to query the ESP32's HTTP API
3. Use the food stock endpoint to provide voice responses
4. Example interaction: "Alexa, ask Pet Feeder how full is the food container"

##  Simulation

The project includes full Wokwi simulation support for testing without hardware.

### Using Wokwi Simulator

1. **Install Wokwi VS Code Extension**:
   - Open VS Code
   - Search for "Wokwi" in extensions
   - Install "Wokwi Simulator"

2. **Open project**:
   ```bash
   cd Back
   code .
   ```

3. **Start simulation**:
   - Press `F1` in VS Code
   - Type "Wokwi: Start Simulator"
   - Or click "Start Simulation" button in status bar

4. **Access simulated server**:
   - ESP32 server: [http://localhost:8180](http://localhost:8180)
   - React frontend: [http://localhost:3000](http://localhost:3000)

5. **View wiring diagram**:
   - Open `diagram.json` to see component connections
   - Visualize in Wokwi interface when simulation is running

### Simulation Features

- Full hardware simulation (sensors, servo, LCD, RTC)
- Network forwarding (localhost:8180 → ESP32:80)
- Real-time sensor interaction
- LCD display visualization
- No physical hardware required

