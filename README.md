<h1 align="center"> Dashboard Firmware </h2>

# 🤖 About

This repository contains the firmware that is run on the PIC18F47Q84 to display text to the driver via an LCD and 7-Segment displays.

![screenshot](https://github.com/user-attachments/assets/12355e24-0bd7-4968-bfbe-95ceb300ec47)
![screenshot](https://github.com/user-attachments/assets/59248495-54ed-4f21-b953-4582fcb83004)


LCD Shows startup screen, then prompts the driver to press TSON and R2D buttons, before then showing critical system information.

The dual 7-segment displays display the BMS State of Charge.

Some features of the dashboard including buttons and LEDs (warning and informational) are controlled via the VCU not the PIC microcontroller on the dash.

This firmware has been written in VSCode rather than using the MPLAB X IDE.

# 📝 How to Build

This project uses a Makefile in order to streamline the compilation process.

```sh
# Open a terminal (Command Prompt or PowerShell for Windows, Terminal for macOS or Linux)

# Ensure Git is installed
# Visit https://git-scm.com to download and install console Git if not already installed

# Clone this repository
git clone https://github.com/sufst/dash-firmware.git

# Navigate to the project directory
cd dash-firmware

# use Make to link and compile all the source files at once
Make all
```


## Source Files

### main.c 
The main file is where the various top-level functions defined in the other source files are called.
First, the system is initialised, a boot screen shows and then the main while(1) loop is entered.
  - CAN messages get recieved
  - Messages are shown whilst the VCU waits for TSON and R2D button presses
  - Information is shown on the LCD and 7-Segment displays

### i2c.c
The i2c file contains low-level functions to enable and utilise i2c communication between the PIC and peripheral devices used to control the displays (PCF8574 and HT16K33).
It includes functions to:
  - send a start bit
  - send a stop bit
  - send bytes of data
  - wait until the bus is free

### CAN.c
The CAN file contains functions to recieve messages from the can tranciever and parse the messages that are important.
  - The can_init() function sets the Tx and Rx pins, sets configuration bits, chooses the baud rate prescaler and sets up message ID masks and filters.
  - The can_process_messages() function checks if there is a message address available in the FIFO register. If so, it gets the data stored at that address in the PIC's ram, and separates the msg ID, dlc, and data. Then, if the msg ID corresponds with the ID of a wanted message, the data saved.
  - The can_get_dashboard_data() passes a pointer to the data so that the main program can access it when called on.

### LCD.c
The LCD file contains high-level functions that can be called on in main.c, for example lcd_print or lcd_set_cursor. These either send commands or data over i2c to the PCF8574 GPIO expander.
Since the LCD connects to the PCF8574 with 4 wires, it must recieve data in 4 bit nibbles. Bits are sent to either the command or data registers to control what is displayed on the screen. This is handled with low-level functions lcd_send_nibble() and lcd_send_byte().

### Seg.c
The Seg file contains functions to display a two digit number to the dual 7-Segment displays. High-level function seg_write_display() takes a 2 digit number, separates the tens and units digits and buffers them to be sent to the HT16K33 over i2c. It is also possible to control the decimal point.

## Header Files
The header files only differ slightly and contain function prototypes for functions in the above source files.
The config.h file however has includes for common libraries and PIC specific libraries.
It also defines constants that may be subject to change such as the clock frequency and i2c addresses.
  - (note: if the clock frequency is changed, baud rates for i2c and CAN must be reconfigured}


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


