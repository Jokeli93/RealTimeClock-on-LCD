\# 🕒 STM32 Real-Time Clock with LCD (Bare-Metal)



\## 📌 Overview



This project implements a \*\*Real-Time Clock (RTC) system\*\* using an STM32 microcontroller and a 16x2 LCD display (HD44780, 4-bit mode). The system reads time and date from an external \*\*DS1307 RTC module (I2C)\*\* and displays it on the LCD, updating every second using the \*\*SysTick timer interrupt\*\*.



The entire project is developed using \*\*bare-metal programming\*\*, without relying on HAL libraries, to demonstrate low-level hardware control and strong understanding of embedded systems fundamentals.



\---



\## 🚀 Features



\* ⏰ Real-time clock display (HH:MM:SS)

\* 📅 Date display (DD/MM/YY + Day of Week)

\* 🔄 Automatic update every second using SysTick

\* 🧩 Modular architecture (Driver + Application layers)

\* ⚡ Bare-metal implementation (no HAL)

\* 🧪 Custom drivers for GPIO, I2C, LCD, and RTC



\---



\## 🏗️ Project Architecture



The project is structured in layers to ensure clarity, reusability, and scalability:



\### 🔹 Application Layer



\* `017rtc\_lcd.c`

\* Handles system logic, formatting, and display updates



\### 🔹 Device Drivers



\* `lcd.c / lcd.h` → HD44780 LCD driver (4-bit mode)

\* `ds1307.c / ds1307.h` → RTC driver using I2C



\### 🔹 MCU Drivers (Bare-Metal)



\* `stm32f407xx\_gpio\_driver.c`

\* `stm32f407xx\_i2c\_driver.c`

\* Direct register-level programming for STM32F407



\---



\## 🛠️ Hardware Setup



\### 📍 Microcontroller



\* STM32F407 Discovery Board



\### 📍 Peripherals



\* DS1307 RTC module (I2C)

\* 16x2 LCD (HD44780, 4-bit mode)



\### 🔌 LCD Connections



| LCD Pin | STM32 Pin         |

| ------- | ----------------- |

| RS      | PD0               |

| RW      | GND               |

| EN      | PD2               |

| D4      | PD3               |

| D5      | PD4               |

| D6      | PD5               |

| D7      | PD6               |

| VSS     | GND               |

| VCC     | 5V                |

| VO      | 10k Potentiometer |

| LED+    | 5V                |

| LED-    | GND               |



\---



\## ⚙️ Key Concepts Demonstrated



\* 🧠 Bare-metal register programming (no HAL)

\* 🔄 Interrupt-driven design using SysTick

\* 📡 I2C communication protocol (master mode)

\* 🖥️ LCD interfacing in 4-bit mode

\* ⏱️ Timing constraints and delay handling

\* 🧩 Modular driver architecture

\* ⚠️ Interrupt-safe design (no blocking operations in ISR)



\---



\## 🔍 Implementation Details



\### ⏱️ SysTick Timer



\* Configured to generate 1 Hz interrupt

\* Used to trigger periodic RTC updates



\### 🖥️ LCD Driver



\* 4-bit communication mode

\* Manual control of GPIO pins

\* Timing handled via software delays



\### 📡 I2C Driver



\* Custom implementation using STM32 registers

\* Supports communication with DS1307 RTC



\### ⏳ RTC Handling



\* Time and date read from DS1307

\* Data formatted into human-readable strings



\---



\## 📸 Demo



Example display:



```

06:05:00 PM

10/04/26 Fri

```



\---



\## 🧪 Challenges \& Solutions



\### ⚠️ LCD Garbage Output



\* Cause: Timing issues and incorrect RS control

\* Solution: Proper delays and correct command/data separation



\### ⚠️ Interrupt Instability



\* Cause: LCD operations inside ISR

\* Solution: Use flag-based communication between ISR and main loop



\### ⚠️ LCD Initialization Issues



\* Cause: Incorrect power-up sequence

\* Solution: Follow HD44780 initialization timing strictly



\---



\## 📈 Future Improvements



\* 🔘 Add user input (buttons) to set time/date

\* ⏰ Alarm functionality with buzzer

\* 🧭 Menu system for configuration

\* ⏱️ Replace delay loops with hardware timers

\* ⚡ Use LCD busy flag instead of fixed delays

\* 🎨 Improve UI (no flickering updates)



\---



\## 🧑‍💻 Author



\*\*Joelikane\*\*

Junior Embedded Software Developer (aspiring)



\---



\## 📎 Notes



This project is intentionally implemented using \*\*bare-metal programming\*\* to demonstrate a deep understanding of:



\* Microcontroller architecture

\* Peripheral control

\* Embedded system design principles



\---



\## ⭐ If you like this project



Feel free to ⭐ the repository and connect!



