### ES32 BarcodeReader

This is Esp32 driver with example .ino for a barcodereader I found on ebay called SE-660L

After much work to figure this thing out I packaged up my learnings for using this board.

##### HARDWARE NOTE:

At the time of this writing, on the PCB, the labels for USB and TTL are reverserd.

The "TTL" has pins for DP and DM which are USB pins. Do not use those pins.

The "USB" has pins for RX and TX that are partially hidden under the speaker. Use these 4 pins. This is actually the TTL.

##### DRIVER NOTE:

This driver is currently very opinionated and requires:

1. You connect the RX/TX to their TX/RX counterparts on the Serial2, which are pins TX2 and RX2 on the ESP32 board.

2. Serial communications are configured for 9600 baud

3. The scan will allow up to 10 seconds to read a barcode.

4. The board is set to "Trigger Mode" which allows you to control when the scan is activated.

5. If you use the beginDeepSleep() method, you have to run the wakeUp() method before more barcodes will read.

##### USAGE

For example usage see barcode/barcode.ino sketch.

For your own code, copy the barcode.h into your arduino library to use.

##### USAGE

Public methods of BarcodeReader:

###### void initializeBarcodeReader()

Sends mimimal commands to get the barcode reader to respond to the other commands.

###### void sendCommand(byte[5] command)

Receives a byte array of the hex characters to make the command work. Takes care of all the crc calculations to make the command work. You just nee dto include the 0x07, 0x01, {addr1}, {addr2}, {data} parts of the command. This is not intuitive and took a while to figure out, but basically construct your hex binary code for the data from the documentation and feed it into a binary-to-hex website. For instance in the BarcodeReader.h file a command SET_TRIGGER_MODE has the addresses 0x00, 0x00 and data of 0xD5. The 0xD5 is a binary 11010101 translated to hexidecimal and specifies all of the options to:

Enable LED,
Disable silence,
Set Normal aiming,
Set Normal lighting
Command triggered mode

##### About Me:

I'm a javascript / C# programmer hacking my way through this C++ / G++ code. There are doubtless better ways to do this. If you have contributions to improve on this, feel free to fork and contribute.

##### License

MIT License

Copyright (c) 2021 hoekma

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE
