🕹️ Arduino OLED Mini Games (Flappy Bird + Pong)
🎯 Overview

This project brings two fun mini-games — Flappy Bird and One-Button Pong — to your Arduino Nano and 128×64 OLED display.
It uses just one push button to control both games and navigate the main menu.

🧩 Features

🏠 Clean Home Menu — Choose between games with a short/long press.

🕊️ Flappy Bird — Tap to flap and dodge pipes; faster after score 12.

🏓 One-Button Pong — Control your paddle with one button, compete against a computer opponent.

🔁 Auto Return — Both games return to menu after “Game Over.”

🧠 Optimized for Arduino Nano + 128×64 OLED — Smooth gameplay and low memory usage.

⚙️ Hardware Requirements
Component	Quantity	Description
Arduino Nano	1	     Main microcontroller
OLED Display	1	     0.96" or 1.3" I2C 128×64 SSD1306
Push Button	  1	     For all input (jump, move, select)
Breadboard + Jumper Wires	—	For wiring connections
🧠 Libraries Used

Install these libraries from Arduino Library Manager:

Adafruit SSD1306

📦 Library Manager: Adafruit SSD1306

💻 GitHub: https://github.com/adafruit/Adafruit_SSD1306

Adafruit GFX Library

📦 Library Manager: Adafruit GFX Library

💻 GitHub: https://github.com/adafruit/Adafruit-GFX-Library

These handle all drawing, text, and display communication.

🔌 Wiring Diagram (I²C OLED)
OLED Pin	Arduino Nano
VCC	5V
GND	GND
SCL	A5
SDA	A4
Button Pin	Arduino Nano
One side	D2
Other side	GND

💡 Internal pull-up is used, so no resistor is required between D2 and 5V.

🕹️ Controls
Action	Menu	Flappy Bird	Pong
Short Press	Scroll between games	Flap	Move paddle up
Long Press	Select game	—	Move paddle down (release)
Game Over	Returns to main menu	Returns to main menu	Returns to main menu
🧩 How It Works
🏠 Menu

Displays “GAMES” title and two options: Flappy Bird and One-Button Pong.

Use short press to move selection and long press to select.

🕊️ Flappy Bird

The bird jumps when the button is pressed.

Avoid top and bottom pipes.

The speed increases after score = 12.

Touching a pipe or screen edge = game over.

🏓 One-Button Pong

The button moves your paddle up when pressed, down when released.

Competes against a computer-controlled paddle.

Game ends when a player scores 10 points.

AI is intentionally imperfect so you can win!

🧠 Code Structure
|-- OLED_Games.ino
    |-- Menu System
    |-- Flappy Bird Game
    |-- Pong Game
    |-- Button Handling (short/long press)


Each game runs inside its own loop (flappyLoop() and pongLoop()), controlled by the global variable currentGame.

⚡ Performance Tips

Use Arduino Nano or UNO for best results.

Ensure no serial output — OLED updates are timing-sensitive.

Keep OLED I²C address set to 0x3C (default for SSD1306).

If your display shows only white pixels or a line, double-check wiring or try swapping SDA/SCL lines.

🧱 Possible Upgrades

🔊 Add buzzer for sound effects (flap, bounce, game over).

🎨 Add splash animation or icons in the menu.

🧮 Add score saving using EEPROM.

⚙️ Support for rotary encoder instead of button.

💡 Add difficulty selection on startup.

👨‍💻 Author & Credits

Created by: Swastik (SW Robotics)
