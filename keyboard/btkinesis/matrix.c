/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

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
 * PORTE2 - col 11
 * PORTE6 - INT6 (EasyPoint)
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


#if (MATRIX_COLS > 16)
#   error "MATRIX_COLS must not exceed 16"
#endif
#if (MATRIX_ROWS > 255)
#   error "MATRIX_ROWS must not exceed 255"
#endif

#define _DDRA (uint8_t *const)&DDRA
#define _DDRB (uint8_t *const)&DDRB
#define _DDRC (uint8_t *const)&DDRC
#define _DDRD (uint8_t *const)&DDRD
#define _DDRE (uint8_t *const)&DDRE
#define _DDRF (uint8_t *const)&DDRF

#define _PINA (uint8_t *const)&PINA
#define _PINB (uint8_t *const)&PINB
#define _PINC (uint8_t *const)&PINC
#define _PIND (uint8_t *const)&PIND
#define _PINE (uint8_t *const)&PINE
#define _PINF (uint8_t *const)&PINF

#define _PORTA (uint8_t *const)&PORTA
#define _PORTB (uint8_t *const)&PORTB
#define _PORTC (uint8_t *const)&PORTC
#define _PORTD (uint8_t *const)&PORTD
#define _PORTE (uint8_t *const)&PORTE
#define _PORTF (uint8_t *const)&PORTF

#define _BIT0 0x01
#define _BIT1 0x02
#define _BIT2 0x04
#define _BIT3 0x08
#define _BIT4 0x10
#define _BIT5 0x20
#define _BIT6 0x40
#define _BIT7 0x80

/* Specifies the ports and pin numbers for the rows */
static
uint8_t *const row_ddr[MATRIX_ROWS] = {
	 _DDRF,  _DDRF,                  _DDRF,  _DDRF,  _DDRF,  _DDRF,
	                                                 _DDRC,  _DDRC,
	                                         _DDRD,  _DDRD,  _DDRD,
	                _DDRE};

static
uint8_t *const row_port[MATRIX_ROWS] = {
	_PORTF, _PORTF,                 _PORTF, _PORTF, _PORTF, _PORTF,
	                                                _PORTC, _PORTC,
	                                         _DDRD, _PORTD, _PORTD,
	                _PORTE};

static
uint8_t *const row_pin[MATRIX_ROWS] = {
	 _PINF,  _PINF,                  _PINF,  _PINF,  _PINF,  _PINF,
	                                                 _PINC,  _PINC,
	                                         _PIND,  _PIND,  _PIND,
	                _PINE};

static
const uint8_t row_bit[MATRIX_ROWS] = {
	 _BIT0,  _BIT1,                  _BIT4,  _BIT5,  _BIT6,  _BIT7,
	                                                 _BIT6,  _BIT7,
	                                         _BIT5,  _BIT6,  _BIT7,
	                _BIT2};

#ifndef DEBOUNCE
#   define DEBOUNCE	0
#endif
static uint8_t debouncing = DEBOUNCE;

// matrix state buffer(1:on, 0:off)
static matrix_row_t *matrix;
static matrix_row_t *matrix_prev;
static matrix_row_t _matrix0[MATRIX_ROWS];
static matrix_row_t _matrix1[MATRIX_ROWS];

#ifdef MATRIX_HAS_GHOST
static bool matrix_has_ghost_in_row(uint8_t row);
#endif
static matrix_row_t read_row(void);
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
    unselect_rows(); /* SET ROWS TO HI-Z */
    // Input with pull-up(DDR:0, PORT:1)
    DDRB = 0x00;
    PORTB = 0xFF;

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
		_matrix0[i] = 0x00;
		_matrix1[i] = 0x00;
	}
    matrix = _matrix0;
    matrix_prev = _matrix1;
}

uint8_t matrix_scan(void)
{
    if (!debouncing) {
        matrix_row_t *tmp = matrix_prev;
        matrix_prev = matrix;
        matrix = tmp;
    }

    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        unselect_rows();
        select_row(i);
        _delay_us(30);  // without this wait read unstable value.
        if (matrix[i] != (matrix_row_t)~read_row()) {
            matrix[i] = (matrix_row_t)~read_row();
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); print("\n");
            }
            debouncing = DEBOUNCE;
        }
    }
    unselect_rows();

    if (debouncing) {
        debouncing--;
    }

    return 1;
}

bool matrix_is_modified(void)
{
    if (debouncing) return false;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        if (matrix[i] != matrix_prev[i]) {
            return true;
        }
    }
    return false;
}

inline
bool matrix_has_ghost(void)
{
#ifdef MATRIX_HAS_GHOST
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        if (matrix_has_ghost_in_row(i))
            return true;
    }
#endif
    return false;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & (1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 01234567\n");
    for (uint8_t row = 0; row < matrix_rows(); row++) {
        phex(row); print(": ");
#if (MATRIX_COLS <= 8)
        pbin_reverse(matrix_get_row(row));
#else
        pbin_reverse16(matrix_get_row(row));
#endif
#ifdef MATRIX_HAS_GHOST
        if (matrix_has_ghost_in_row(row)) {
            print(" <ghost");
        }
#endif
        print("\n");
    }
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
#if (MATRIX_COLS <= 8)
        count += bitpop(matrix[i]);
#else
        count += bitpop16(matrix[i]);
#endif
    }
    return count;
}

#ifdef MATRIX_HAS_GHOST
inline
static bool matrix_has_ghost_in_row(uint8_t row)
{
    // no ghost exists in case less than 2 keys on
    if (((matrix[row] - 1) & matrix[row]) == 0)
        return false;

    // ghost exists in case same state as other row
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        if (i != row && (matrix[i] & matrix[row]) == matrix[row])
            return true;
    }
    return false;
}
#endif

inline
static matrix_row_t read_row(void)
{
    return PINB;
}

inline
static void unselect_rows(void)
{
    // Hi-Z(DDR:0, PORT:0) to unselect
	for (int i = 0; i < MATRIX_ROWS; i++) {
		*row_ddr[i]  &= ~row_bit[i];
		*row_port[i] &= ~row_bit[i];
	}
}

inline
static void select_row(uint8_t row)
{
    // Output low(DDR:1, PORT:0) to select
	*row_ddr[row]  |= row_bit[row];
	*row_port[row] &= ~row_bit[row];
}
