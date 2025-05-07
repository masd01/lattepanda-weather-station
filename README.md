# lattepanda-weather-station
Lattepanda weather station with oled ssd1306. Python script receives data from web and sends to onboard arduino leonardo to display in oled


According to Lattepanda pinout and ssd1306, 

![lattepanda](https://github.com/user-attachments/assets/91e5a617-e49f-4772-abcb-a7fdf219d838)

![ssd1306](https://github.com/user-attachments/assets/8cdd4056-b65e-4361-a52f-26677666b938)

SDA-SDA, 
SCL-SCL, 
5V-VCC, 
GND-GND.

1. very simple connection of oled ssd1306 to onboard arduino.
2. be sure to have installed all necessary libraries as they are mentioned(included-imported) in the first lines of both .ino and .py files. don't forget to change to your city in line 21 of python file 
4. upload weatherStation.ino to onboard arduino.
5. run weatherStation.py. 

![Screenshot_2025-04-20-02-50-27-733_com miui gallery-edit](https://github.com/user-attachments/assets/85e6082d-ff59-42d9-a4d0-a13314a1a3db)
