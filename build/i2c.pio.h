// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// --- //
// i2c //
// --- //

#define i2c_wrap_target 12
#define i2c_wrap 17
#define i2c_pio_version 0

#define i2c_offset_entry_point 12u

static const uint16_t i2c_program_instructions[] = {
    0x008c, //  0: jmp    y--, 12
    0xc030, //  1: irq    wait 0 rel
    0xe027, //  2: set    x, 7
    0x6781, //  3: out    pindirs, 1             [7]
    0xba42, //  4: nop                    side 1 [2]
    0x24a1, //  5: wait   1 pin, 1               [4]
    0x4701, //  6: in     pins, 1                [7]
    0x1743, //  7: jmp    x--, 3          side 0 [7]
    0x6781, //  8: out    pindirs, 1             [7]
    0xbf42, //  9: nop                    side 1 [7]
    0x27a1, // 10: wait   1 pin, 1               [7]
    0x12c0, // 11: jmp    pin, 0          side 0 [2]
            //     .wrap_target
    0x6026, // 12: out    x, 6
    0x6041, // 13: out    y, 1
    0x0022, // 14: jmp    !x, 2
    0x6060, // 15: out    null, 32
    0x60f0, // 16: out    exec, 16
    0x0050, // 17: jmp    x--, 16
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program i2c_program = {
    .instructions = i2c_program_instructions,
    .length = 18,
    .origin = -1,
    .pio_version = i2c_pio_version,
#if PICO_PIO_VERSION > 0
    .used_gpio_ranges = 0x0
#endif
};

static inline pio_sm_config i2c_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + i2c_wrap_target, offset + i2c_wrap);
    sm_config_set_sideset(&c, 2, true, true);
    return c;
}

#include "hardware/clocks.h"
#include "hardware/gpio.h"
static inline void i2c_program_init(PIO pio, uint sm, uint offset, uint pin_scl, uint pin_sda) {
    assert(pin_sda == pin_scl + 1);  // SCL first, then SDA
    pio_sm_config c = i2c_program_get_default_config(offset);
    // Redirect OUT/SET/IN to SDA (which is now pin_scl + 1)
    sm_config_set_out_pins(&c, pin_sda, 1);    // OUT to SDA
    sm_config_set_set_pins(&c, pin_sda, 1);    // SET to SDA
    sm_config_set_in_pins(&c, pin_sda);        // IN from SDA
    sm_config_set_sideset_pins(&c, pin_scl);   // SIDESET is SCL
    sm_config_set_jmp_pin(&c, pin_sda);        // Jump uses SDA
    sm_config_set_out_shift(&c, false, true, 16);
    sm_config_set_in_shift(&c, false, true, 8);
    float div = (float)clock_get_hz(clk_sys) / (32 * 100000);
    sm_config_set_clkdiv(&c, div);
    // Configure SDA and SCL pins with pull-ups
    gpio_pull_up(pin_sda);
    gpio_pull_up(pin_scl);
    uint32_t both_pins = (1u << pin_sda) | (1u << pin_scl);
    pio_sm_set_pins_with_mask(pio, sm, both_pins, both_pins);
    pio_sm_set_pindirs_with_mask(pio, sm, both_pins, both_pins);
    pio_gpio_init(pio, pin_sda);
    gpio_set_oeover(pin_sda, GPIO_OVERRIDE_INVERT);
    pio_gpio_init(pio, pin_scl);
    gpio_set_oeover(pin_scl, GPIO_OVERRIDE_INVERT);
    pio_sm_set_pins_with_mask(pio, sm, 0, both_pins);
    pio_set_irq0_source_enabled(pio, (enum pio_interrupt_source)((uint)pis_interrupt0 + sm), false);
    pio_set_irq1_source_enabled(pio, (enum pio_interrupt_source)((uint)pis_interrupt0 + sm), false);
    pio_interrupt_clear(pio, sm);
    pio_sm_init(pio, sm, offset + i2c_offset_entry_point, &c);
    pio_sm_set_enabled(pio, sm, true);
}

#endif

// ----------- //
// set_scl_sda //
// ----------- //

#define set_scl_sda_wrap_target 0
#define set_scl_sda_wrap 3
#define set_scl_sda_pio_version 0

static const uint16_t set_scl_sda_program_instructions[] = {
            //     .wrap_target
    0xf780, //  0: set    pindirs, 0      side 0 [7]
    0xf781, //  1: set    pindirs, 1      side 0 [7]
    0xff80, //  2: set    pindirs, 0      side 1 [7]
    0xff81, //  3: set    pindirs, 1      side 1 [7]
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program set_scl_sda_program = {
    .instructions = set_scl_sda_program_instructions,
    .length = 4,
    .origin = -1,
    .pio_version = set_scl_sda_pio_version,
#if PICO_PIO_VERSION > 0
    .used_gpio_ranges = 0x0
#endif
};

static inline pio_sm_config set_scl_sda_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + set_scl_sda_wrap_target, offset + set_scl_sda_wrap);
    sm_config_set_sideset(&c, 2, true, false);
    return c;
}

// Define order of our instruction table
enum {
    I2C_SC0_SD0 = 0,
    I2C_SC0_SD1,
    I2C_SC1_SD0,
    I2C_SC1_SD1
};

#endif

