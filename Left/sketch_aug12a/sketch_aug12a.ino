#define BAUD_RATE 9600
#include "KeyBtn.h"
#include <SoftwareSerial.h>

#define RX_PIN A3
#define TX_PIN 9

int flag = 0;

int8_t row_pins[] = { 2, 3, 4, 5, 6, 7 };
int8_t col_pins[] = { A1, A0, 15, 14, 16, 10 };

SoftwareSerial BT(TX_PIN, RX_PIN);

KeyBtn keys[7][6];

void setup() {
    for (int8_t pin : row_pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    for (int8_t pin : col_pins) {
        pinMode(pin, INPUT_PULLUP);
    }
    BT.begin(BAUD_RATE);
}

void loop() {
    for (int8_t row = 0; row < 6; ++row) {
        // select row
        digitalWrite(row_pins[row], LOW);
        
        for (int8_t col = 0; col < 6; ++col) {
            bool key_state = digitalRead(col_pins[col]);
            if (keys[row][col].state_changed(key_state)) {
                int i = 5 - row;
                int j = col;
                

                uint8_t matrix_idx = j + i * 12;
                if (key_state == HIGH) {
                    if (col == 5 && row == 0)
                    {
                        flag = 0;
                    }
                    else
                    {
                        if (flag == 0)
                        {
                            matrix_idx |= 0b100000000;
                            BT.print((char)matrix_idx);
                        }
                        else
                        {
                            matrix_idx |= 0b110000000;//组合键
                            BT.print((char)matrix_idx);
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
                        if ( flag == 1)  
                        {
                            matrix_idx |= 0b010000000;//组合键
                            BT.print((char)matrix_idx);
                        }
                        else
                        {
                            BT.print((char)matrix_idx);
                        }
                    }
                }
            }
        }

        // release row
        digitalWrite(row_pins[row], HIGH);
    }
}
