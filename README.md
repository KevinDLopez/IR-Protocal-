# 📡 IR Protocol with Bluetooth-Controlled Car 🚗

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

## 📝 Description

This project implements a custom Infrared (IR) protocol integrated with a Bluetooth module on a TM4C launchpad microcontroller. The IR protocol operates at a 40kHz frequency with a 1.5ms start bit, two address bits, followed by command bits to control a remote car.

The car is controlled via Bluetooth using PWM signals and features an IR transmitter for communication with peripheral devices.

## ✨ Features

- 🎮 Custom IR protocol implementation
- 📱 Bluetooth connectivity
- 🚘 PWM-based motor control
- 🌈 Colored LCD screen interface
- 🔄 Remote control capabilities

## 🎬 Demo

<div align="center">
  <a href="https://www.youtube.com/watch?v=kkXHs1aFpvE">
    <img src="https://img.youtube.com/vi/kkXHs1aFpvE/0.jpg" alt="IR Protocol Demo Video" width="600" style="border-radius: 10px;">
  </a>
  <p><em>Click the image above to watch the demo on YouTube</em></p>
</div>

## 🛠️ Technologies Used

- TM4C123 LaunchPad microcontroller
- IR transmitter and receiver modules
- Bluetooth module
- PWM-based motor drivers
- Colored LCD screen

## 📁 Repository Structure

```
.
├── Project Report.pdf           # Detailed project documentation
├── Project2.0.zip               # Complete project archive
└── Source Code                  # Implementation files
    ├── INIT2.c                  # Initialization code variant 2
    ├── Init.c                   # Base initialization code
    ├── Init3.c                  # Initialization code variant 3
    ├── InputOutput.c            # I/O functionality
    ├── PLL.c                    # Phase-Locked Loop implementation
    ├── PLL.h                    # PLL header file
    ├── PWM.c                    # PWM implementation
    ├── PWM.h                    # PWM header file
    └── PWMtest.c                # PWM testing code
```

## ⚙️ Setup and Installation

1. Clone the repository:
   ```
   git clone https://github.com/KevinDLopez/IR-Protocal-
   ```
2. Open the project in your preferred microcontroller IDE (Keil, Code Composer Studio, etc.)
3. Connect the TM4C LaunchPad to your computer
4. Build and flash the code to your device

## 📖 Usage

1. Power on the remote-controlled car
2. Connect to the car's Bluetooth module using a compatible device
3. Use the control interface to send commands to the car
4. The car will respond to IR commands according to the implemented protocol

## 👥 Contributors

<div align="center">
  <div style="display: flex; justify-content: space-around;">
    <div style="text-align: center; margin: 10px;">
      <img src="https://github.com/KevinDLopez.png" alt="Kevin Lopez" width="100" height="100" style="border-radius: 50%;">
      <br />
      <strong>Kevin Lopez</strong>
      <br />
      <a href="https://github.com/KevinDLopez">@KevinDLopez</a>
    </div>
    <div style="text-align: center; margin: 10px;">
      <img src="https://github.com/Cristianlopez0718.png" alt="Cristian Lopez" width="100" height="100" style="border-radius: 50%;">
      <br />
      <strong>Cristian Lopez</strong>
      <br />
      <a href="https://github.com/Cristianlopez0718">@Cristianlopez0718</a>
    </div>
  </div>
</div>

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.
