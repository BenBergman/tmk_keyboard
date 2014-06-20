/*
Copyright 2014 Benjamin Bergman <ben@benbergman.ca>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



/*
 * THE PLAN
 *
 * PORTB is all pin change interrupts. Use for the 8 rows (needed for power
 * saving mode).
 * PORTD3:0 and PORTE6 are interrupts. Save for joystick and potential IO
 * expander (needed for power saving mode).
 * PORTD3:2 are UART Rx/Tx. Use for bluetooth module.
 * PORTD1:0 are TWI(I2C). Use for joystick and potential IO expander data.
 * PORTD7:6,4 and PORT7:4,1:0 are ADCs. Save one for battery meter.
 *
 * Total Pins
 * PORTB0 - row 0
 * PORTB1 - row 1
 * PORTB2 - row 2
 * PORTB3 - row 3
 * PORTB4 - row 4
 * PORTB5 - row 5
 * PORTB6 - row 6
 * PORTB7 - row 7
 *
 * PORTC6 - col 6
 * PORTC7 - col 7
 *
 * PORTD0 - I2C SCL (EasyPoint/IO expander/etc)
 * PORTD1 - I2C SDA(EasyPoint/IO expander/etc)
 * PORTD2 - UART RXD (WT12)
 * PORTD3 - UART TXD (WT12)
 * PORTD4 - ADC? EasyPoint button?
 * PORTD5 - col 8
 * PORTD6 - col 9
 * PORTD7 - col 10
 *
 * old plan, can't do with adafruit board -> PORTE2 - col 11
 * old plan, can't do with adafruit board -> PORTE6 - INT6 (EasyPoint)
 *
 * PORTE2 - HWB
 * PORTE6 - col 11
 *
 * PORTF0 - col 0
 * PORTF1 - col 1
 * PORTF4 - col 2
 * PORTF5 - col 3
 * PORTF6 - col 4
 * PORTF7 - col 5
 *
 * Future changes:
 * - Move WT12 to an I2C to UART adapter to free two interrupt pins.
 * - Move EasyPoint button to interrupt pin
 * - Add battery monitor to ADC input (formerly EasyPoint button)
 * - Add I2C IO expander either for LEDs or for second half of matrix
 *
 * Alternate plan:
 * - Switch to AT90USB1286 for additional pins
 */



/*
 * scan matrix
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"


#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);


inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    // initialize row and col
    unselect_rows();
    init_cols();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}

uint8_t matrix_scan(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        _delay_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        unselect_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

bool matrix_is_modified(void)
{
    if (debouncing) return false;
    return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 01234567\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row); print(": ");
        pbin_reverse16(matrix_get_row(row));
        print("\n");
    }
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop16(matrix[i]);
    }
    return count;
}

/* Column pin configuration
 * col: 0   1   2   3   4   5   6   7
 * pin: B0  B1  B2  B3  B4  B5  D6  B7
 */
static void  init_cols(void)
{
    // Input with pull-up(DDR:0, PORT:1)
    DDRB  = 0x00;
    PORTB = 0xFF;
}

static matrix_row_t read_cols(void)
{
    return ~PINB;
}

/* Row pin configuration
 * row: 0  1  2  3  4  5  6  7  8  9  10 11
 * pin: F0 F1 F4 F5 F6 F7 C6 C7 D5 D6 D7 E6
 */
static void unselect_rows(void)
{
    // Hi-Z(DDR:0, PORT:0) to unselect
    DDRF  &= ~0b11110011; // PF: 7,6,5,4,1,0
    PORTF &= ~0b11110011;
    DDRC  &= ~0b11000000; // PC: 7,6
    PORTC &= ~0b11000000;
    DDRD  &= ~0b11100000; // PD: 7,6,5
    PORTD &= ~0b11100000;
    DDRE  &= ~0b01000000; // PE: 6
    PORTE &= ~0b01000000;
}

static void select_row(uint8_t row)
{
    // Output low(DDR:1, PORT:0) to select
    switch (row) {
        case 0:
            DDRF  |= (1<<0);
            PORTF &= ~(1<<0);
            break;
        case 1:
            DDRF  |= (1<<1);
            PORTF &= ~(1<<1);
            break;
        case 2:
            DDRF  |= (1<<4);
            PORTF &= ~(1<<4);
            break;
        case 3:
            DDRF  |= (1<<5);
            PORTF &= ~(1<<5);
            break;
        case 4:
            DDRF  |= (1<<6);
            PORTF &= ~(1<<6);
            break;
        case 5:
            DDRF  |= (1<<7);
            PORTF &= ~(1<<7);
            break;
        case 6:
            DDRC  |= (1<<6);
            PORTC &= ~(1<<6);
            break;
        case 7:
            DDRC  |= (1<<7);
            PORTC &= ~(1<<7);
            break;
        case 8:
            DDRD  |= (1<<5);
            PORTD &= ~(1<<5);
            break;
        case 9:
            DDRD  |= (1<<6);
            PORTD &= ~(1<<6);
            break;
        case 10:
            DDRD  |= (1<<7);
            PORTD &= ~(1<<7);
            break;
        case 11:
            DDRE  |= (1<<6);
            PORTE &= ~(1<<6);
            break;
    }
}
