# GPS-TM4C123GH6PM-Project

The goal of this project is to develop an embedded system using C programming that
gathers real-time positional coordinates while a microcontroller is in motion, and display
location information on an attached computer or an LCD. The positioning system should
use the TM4C123G LaunchPad. A map of the place with coordinates of various landmarks
should be stored on the device. When approaching a landmark, its name should be
displayed.
As a demo, it’s required to load the coordinates of at least 5 different halls of the faculty.

Functional Requirements:
  1. After power-on, the system should read GPS module data and wait until there is a GPS fix (Check GPS module datasheet to check how can this be done).
  2. After acquiring the coordinates, it should display the name of closest landmark from a list of preset landmark coordinates.
  3. Periodically update the displayed location information.

Project team : 
  - Hana walaa    2200653
  - Hoor Wael     2200298
  - Mariam Amr    2200704
  - Alaa Abdel    2200498
  - Neama Esam    2200371
  - Miran Mohamed 2200604
  - Nour Ali      2200292

For additional features we chose to add:
  1. LCD screen that displays name of nearest hall (writing our own LCD Drivers).
  2. SD card to store google maps image of the university satelite image (map) and store images of the halls.
  3. TFT touch screen that draws a map of the university.
  4. The TFT screen displays in real time the tiva c's coordinates on the drawn map.
  5. With the use of the touch screen we made it so that if you click on the hall it would display an image of it.
  6. Buzzer that beeps whenever the closest hall is changed.
  7. Power bank supply as to be portable and not require connection to a laptop.
  8. UART communication between Arduino and Tiva C as the Tiva C sends longitude and Latitude to Arduino.
