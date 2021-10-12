/*
 * se660 barcode device driver
 * 
 * Decription: controls se-660 barcode reader
 * 
 */

#ifndef BARCODE_H
#define BARCODE_H

#define DEBUG false


class BarcodeReader {
    private: 
        const byte HEADER[2] = { 0x7E, 0x00 };
        const byte SET_TRIGGER_MODE[5] = {0x08, 0x01, 0x00, 0x00, 0xD5};
        const byte SET_READ_TIME[5] = {0x08, 0x01, 0x00, 0x06, 0x64}; // ~10seconds
        const byte SET_SERIAL_OUTPUT[5] = {0x08, 0x01, 0x00, 0x0D, 0x04};
        const byte SET_DIABLE_SETUP_CODE[5] = {0x08, 0x01, 0x00, 0x03, 0x02};

        const byte SCAN_BARCODE[5] = {0x08, 0x01, 0x00, 0x02, 0x41};
        const byte BEGIN_DEEP_SLEEP[5] = {0x08, 0x01, 0x00, 0xD9, 0xA5};
        const byte WAKE_UP[5] = {0x08, 0x01, 0x00, 0xD9, 0x00};

        const unsigned long MAX_SCAN_TIME = 10000;
        const unsigned long MAX_AWAIT_COMMAND_OUTPUT_TIME = 500;

        // --------------
        // Calculate CRC

        unsigned int crc_cal_by_bit(unsigned char* ptr, unsigned int len) {
            unsigned int crc = 0;
            while(len-- != 0) {
            for(unsigned char i = 0x80; i != 0; i /= 2){
                crc *= 2;
                if((crc&0x10000) !=0) {
                    //previous bit CRC x 2, if the first bit is 1, divided by 0x11021 
                    crc ^= 0x11021;
                }
                    
                if((*ptr&i) != 0) {
                    //if base bit is 1, CRC = previous bit CRC + base bit/CRC_CCITT
                    crc ^= 0x1021;
                }
                }
                ptr++;
            }
            return crc;
        }

        // ----------------------------------------
        // -- setCRC()

        void setCRC(byte *crc, const byte *command){
            unsigned int crcInt = {crc_cal_by_bit((unsigned char*)command, 5)};

            crc[0] = (crcInt >> 8) & 0xFF;
            crc[1] = crcInt & 0xFF;  
        }

    public: 
        // ----------------------------------------
        // Constructor

        BarcodeReader() {
            Serial2.begin(9600);
        };

        // ----------------------------------------
        // initializeBarcodeReader()

        void initializeBarcodeReader() {
            if (DEBUG) {
                Serial.print("Initialize Barcode Reader...");
            }

            sendCommand(SET_SERIAL_OUTPUT);
            sendCommand(SET_TRIGGER_MODE);
            sendCommand(SET_READ_TIME);
            sendCommand(SET_DIABLE_SETUP_CODE);
            
            
            if (DEBUG) {
                Serial.println("done.");  
                delay(10);
            }
        }

        // ----------------------------------------
        // -- sendCommand()


        void sendCommand(const byte *command){
            byte crc[2];
            setCRC(crc, command);

            Serial2.write(HEADER, 2);
            Serial2.write(command, 5);
            Serial2.write(crc,2);

            unsigned long startMillis = millis();
            boolean dataReceived = false;
            
            while(!dataReceived && millis() - startMillis < MAX_AWAIT_COMMAND_OUTPUT_TIME) {
                if (Serial2.available() > 0) {
                dataReceived = true;
                }
            }
            
            while (Serial2.available() > 0) {
                Serial2.read();
                delay(10);
            }        
        }


        // ----------------------------------------
        // SCAN BAR CODE

        String scanBarCode() {
        
        // Activate barcode scan function
        sendCommand(SCAN_BARCODE);
        
        unsigned long startMillis = millis();
        boolean dataReceived = false;
        boolean done = false; 
        int stringLen = 0;

        // Wait until the serial buffer is filled or 5 seconds passes.
        while(!done && millis() - startMillis < MAX_SCAN_TIME) {
            if (dataReceived && Serial2.available() == stringLen) {
            done = true;
            } else if (Serial2.available() > 0) {
            stringLen = Serial2.available();
            dataReceived = true;
            delay(10);
            }
        }

        // Read barcode from Serial2 buffer into string.
        String barcodeData = "";  
        barcodeData += Serial2.readStringUntil('\r');

        return barcodeData;
        }


        void beginDeepSleep() {
            // Enter deep sleep mode
            if (DEBUG){
                Serial.println("Entering Deep Sleep");
                sendCommand(BEGIN_DEEP_SLEEP);
            }
        }

        void wakeUp() {
            // Enter deep sleep mode
            if (DEBUG){
                Serial.println("Waking Up");
            }
            
            sendCommand(WAKE_UP);
            delay(1000);

            initializeBarcodeReader();

        }
};
#endif
