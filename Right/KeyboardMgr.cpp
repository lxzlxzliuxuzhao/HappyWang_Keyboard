#include <Keyboard.h>
#include <Wire.h>
#include "KeyboardMgr.h"

byte row_codes[] = { 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE };

int8_t row_pins[] = { 2, 3, 4, 5, 6, 7 };
int8_t col_pins[] = { A3, A2, A1, A0, 15, 14 };

int flag = 0;

KeyboardMgr::KeyboardMgr(int Rx_pin, int Tx_pin, byte _mcp23018_addr) {
    left_ble = new SoftwareSerial(Rx_pin, Tx_pin);
    mcp23018_addr = _mcp23018_addr;
    key_def[0] = KEY_TAB;
    key_def[1] = 113;
    key_def[2] = 119;
    key_def[3] = 101;
    key_def[4] = 114;
    key_def[5] = 116;

    key_def[12] = KEY_CAPS_LOCK;
    key_def[13] = 97;
    key_def[14] = 115;
    key_def[15] = 100;
    key_def[16] = 102;
    key_def[17] = 103;
    
    key_def[24] = KEY_LEFT_SHIFT;
    key_def[25] = 122;
    key_def[26] = 120;
    key_def[27] = 99;
    key_def[28] = 118;
    key_def[29] = 98;

    key_def[36] = 0;
    key_def[37] = 0;
    key_def[38] = KEY_LEFT_CTRL;
    key_def[39] = KEY_LEFT_GUI;
    key_def[40] = KEY_LEFT_ALT;
    key_def[41] = 32;

    key_def[48] = 0;
    key_def[49] = 0;
    key_def[50] = 0;
    key_def[51] = 0;
    key_def[52] = 51;
    key_def[53] = 49;

    key_def[60] = 0;
    key_def[61] = 0;
    key_def[62] = 0;
    key_def[63] = 0;
    key_def[64] = 52;
    key_def[65] = 50;

    key_def[6] = 121;
    key_def[7] = 117;
    key_def[8] = 105;
    key_def[9] = 111;
    key_def[10] = 112;
    key_def[11] = 92;
    
    key_def[18] = 104;
    key_def[19] = 106;
    key_def[20] = 107;
    key_def[21] = 188;
    key_def[22] = 59;
    key_def[23] = 44;

    key_def[30] = 110;
    key_def[31] = 109;
    key_def[32] = 44;
    key_def[33] = 46;
    key_def[34] = 91;
    key_def[35] = 93;

    key_def[42] = KEY_RETURN;
    key_def[43] = KEY_BACKSPACE;
    key_def[44] = 45;
    key_def[45] = 61;
    key_def[46] = 0;
    key_def[47] = 0;

    key_def[54] = 54;
    key_def[55] = 56;
    key_def[56] = 0;
    key_def[57] = 0;
    key_def[58] = 0;
    key_def[59] = 0;

    key_def[66] = 53;
    key_def[67] = 55;
    key_def[68] = 0;
    key_def[69] = 0;
    key_def[70] = 0;
    key_def[71] = 0;

    key_fn_def[0] = KEY_F1;
    key_fn_def[1] = KEY_F2;
    key_fn_def[2] = KEY_F3;
    key_fn_def[3] = KEY_F4;
    key_fn_def[4] = KEY_F5;
    key_fn_def[5] = KEY_F6;

    key_fn_def[6] = KEY_F7;
    key_fn_def[7] = KEY_F8;
    key_fn_def[8] = KEY_F9;
    key_fn_def[9] = KEY_F10;
    key_fn_def[10] = KEY_F11;
    key_fn_def[11] = KEY_F12;

    key_fn_def[12] = '1';
    key_fn_def[13] = '2';
    key_fn_def[14] = '3';
    key_fn_def[15] = '4';
    key_fn_def[16] = '6';
    
    key_fn_def[17] = '7';
    key_fn_def[18] = '8';
    key_fn_def[19] = '9';
    key_fn_def[20] = '0';
    key_fn_def[21] = '-';
    key_fn_def[22] = '+';

    key_fn_def[23] = 0;
    key_fn_def[24] = 0;
    key_fn_def[25] = 
}

void KeyboardMgr::begin() {
    for (int8_t pin : row_pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    for (int8_t pin : col_pins) {
        pinMode(pin, INPUT_PULLUP);
    }
    left_ble->begin(BAUD_RATE);
    Keyboard.begin();
/*     Wire.begin();
    Wire.setClock(400000UL);

    Wire.beginTransmission(mcp23018_addr);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.beginTransmission(mcp23018_addr);
    Wire.write(0x01);
    Wire.write(0xFF);
    Wire.endTransmission();

    Wire.beginTransmission(mcp23018_addr);
    Wire.write(0x0D);
    Wire.write(0xFF);
    Wire.endTransmission(); */
}

void KeyboardMgr::exec() {
    for (uint8_t row = 0; row < 6; ++row) {
        
        digitalWrite(row_pins[row], LOW);
        
        for (uint8_t col = 0; col < 6; ++col) {
            bool key_state = digitalRead(col_pins[col]);
            if (keys[row][col].state_changed(key_state)) {
                right_data.pressed = key_state;
                right_data.matrix_idx = row + col * 12 + 6;
                if(key_state == HIGH)
                {
                    if (col == 5 && row == 0)
                    {
                        flag = 0;
                    }
                    else
                    {
                        if (flag == 0)
                        {
                            processKeyEvent(right_data);
                        }
                        else
                        {
                            right_data.matrix_idx |= 0b10000000;//组合键
                            processKeyEvent(right_data);
                        }
                    }
                }
                else
                {
                    if (col == 5 && row == 0)
                    {
                        flag = 1;
                    }
                    else
                    {
                        if (flag == 0) {

                            processKeyEvent(right_data);
                        }
                        else
                        {
                            right_data.matrix_idx |= 0b10000000;
                            processKeyEvent(right_data);
                        }
                    }
                }
            }
        }

        while (read_from_left()) {
            
            processKeyEvent(left_data);
        }
        digitalWrite(row_pins[row], HIGH);
    }
}

void KeyboardMgr::processKeyEvent(ParsedData data) {
    char key_char = key_def[data.matrix_idx];
    if (key_char != 0) {
        if (!data.pressed) {
            Keyboard.press(key_char);
        } else {
            Keyboard.release(key_char);
        }
    }
}

bool KeyboardMgr::read_from_left() {
    if (left_ble->available()) {
        int recv = char(left_ble->read());
        if (recv & 0b10000000) {
            left_data.pressed = true;
            left_data.matrix_idx = (recv & 0b01111111);
        } else {
            left_data.pressed = false;
            left_data.matrix_idx = recv;
        }
        return true;
    } else {
        return false;
    }
}

bool KeyboardMgr::col_decoder(byte val, uint8_t col) {
    return !(0x01 & (val >> col));
}

void KeyboardMgr::control_row(byte val) {
    Wire.beginTransmission(mcp23018_addr);
    Wire.write(0x12);
    Wire.write(val);
    Wire.endTransmission();
}