NodeMCU based clocking system
-----------------------------

Created by Andras Dobozi
Eszterhazy Karoly University
2018

-----------------------------------------------------------------------------------
-----------------------------------------------------------------------------------

This is a NodeMCU microcontroller based clocking system.
When the card is touched, the mcu sends the card id to the sql database
and asks that is the user clocked in or not.

if user is not clocked in:
--------------------------
Creates a new row in the 'events' table with the user id,
is_clocked_in boolean, and clock in datetime.
After that welcomes the user on the OLED screen with the user's full name.

if user is clocked in already:
------------------------------
Updates the is_clocked_in boolean to '0' and SQL fills in the clocked out
datetime field due to updating.
After that says 'bye bye' to the user on the OLED screen with the user's full name.

-----------------------------------------------------------------------------------
-----------------------------------------------------------------------------------

Required parts:
---------------

NodeMcu Lua WiFi IOT development board based on ESP8266, CP2102 module
RFID-RC522 Radiofrequency IC Card Inducing Sensor Reader
Blue 0.96" 128X64 OLED I2C IIC Serial Display Module