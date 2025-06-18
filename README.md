<h1 align="center"> Dashboard Firmware </h2>

# 🤖 About

This repository contains the firmware that is run on the PIC18F47Q84 to display text to the driver via an LCD and 7-Segment displays.

![screenshot](https://github.com/user-attachments/assets/12355e24-0bd7-4968-bfbe-95ceb300ec47)
![screenshot](https://github.com/user-attachments/assets/59248495-54ed-4f21-b953-4582fcb83004)


<<<<<<< Updated upstream
LCD Shows startup screen, then prompts the driver to press TSON and R2D buttons. The LCD also shows the current VCU state and errors, and when the car is in the driving state, it will display critical system information such as temperature and power.
=======
LCD Shows startup screen, then prompts the driver to press TSON and R2D buttons, before then showing critical system information including:
  - BMS_Average_Temperature (msg ID 0x202)
>>>>>>> Stashed changes

The dual 7-segment displays display the BMS State of Charge.

Some features of the dashboard including buttons and LEDs (warning and informational) are controlled via the VCU/TSAL not the PIC microcontroller on the dash.

This firmware was written in MPLAB X IDE, with MCC code generation to handle implementation of I2C and CAN.

# 📝 How to Build and Program

![image](https://github.com/user-attachments/assets/4f20bd78-ee8d-4ce8-a729-4a18cc97245f)


In MPLAB X IDE:

```sh
# Open the project folder
# Navigate to Production and click Build (shortcut F11)
# Open MPLAB X IPE and connect the PICKIT4 programmer
# Connect the ICSP Pins
# Load the hex file
# Cick Program
# Unplug the MCLR pin (Hold on Reset mode default)
# Power cycle the board to ensure correct operation from boot
```

# ⚙️ Versions

The DESIGNSHOW firmware has limited functionality and only shows a splashscreen text on the LCD

The COMP firmware has all functionality and is the firmware that should be loaded onto the PIC before competition.



# 🧑‍🎓Resources

PIC18F47Q84 Datasheet: 
https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/PIC18F27-47-57Q84-Microcontroller-Data-Sheet-DS40002213.pdf

PCF8574 Datasheet:
https://www.ti.com/lit/ds/symlink/pcf8574.pdf

HT16K33 Datasheet:
https://cdn-shop.adafruit.com/datasheets/ht16K33v110.pdf

Using a 16x2 LCD:
https://www.electronicsforu.com/technology-trends/learn-electronics/16x2-lcd-pinout-diagram

# 🤝 Related Projects

  - [Dashboard PCB](https://github.com/sufst/pcb/tree/new-dash/dash)
  - [VCU](https://github.com/sufst/vcu)
  - [CAN Bus Definitions](https://github.com/sufst/can-defs)
  - [TSAL-Latching](https://github.com/sufst/pcb/tsal-latching-integrated)


