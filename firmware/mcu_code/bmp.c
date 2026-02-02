#include "bmp.h"


/**
 * @brief Chip information
 */
#define CHIP_NAME                 "Bosch BMP390"        /**< chip name */
#define MANUFACTURER_NAME         "Bosch"               /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.65f                 /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                  /**< chip max supply voltage */
#define MAX_CURRENT               0.73f                 /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                 /**< chip max operating temperature */
#define DRIVER_VERSION            1000                  /**< driver version */

/**
 * @brief Chip register definitions
 */
#define CMD                 0x7E        /**< command register */
#define NVM_PAR_T1_L        0x31        /**< NVM PAR T1 low register */
#define NVM_PAR_T1_H        0x32        /**< NVM PAR T1 high register */
#define NVM_PAR_T2_L        0x33        /**< NVM PAR T2 low register */
#define NVM_PAR_T2_H        0x34        /**< NVM PAR T2 high register */
#define NVM_PAR_T3          0x35        /**< NVM PAR T3 register */
#define NVM_PAR_P1_L        0x36        /**< NVM PAR P1 low register */
#define NVM_PAR_P1_H        0x37        /**< NVM PAR P1 high register */
#define NVM_PAR_P2_L        0x38        /**< NVM PAR P2 low register */
#define NVM_PAR_P2_H        0x39        /**< NVM PAR P2 high register */
#define NVM_PAR_P3          0x3A        /**< NVM PAR P3 register */
#define NVM_PAR_P4          0x3B        /**< NVM PAR P4 register */
#define NVM_PAR_P5_L        0x3C        /**< NVM PAR P5 low register */
#define NVM_PAR_P5_H        0x3D        /**< NVM PAR P5 high register */
#define NVM_PAR_P6_L        0x3E        /**< NVM PAR P6 low register */
#define NVM_PAR_P6_H        0x3F        /**< NVM PAR P6 high register */
#define NVM_PAR_P7          0x40        /**< NVM PAR P7 register */
#define NVM_PAR_P8          0x41        /**< NVM PAR P8 register */
#define NVM_PAR_P9_L        0x42        /**< NVM PAR P9 low register */
#define NVM_PAR_P9_H        0x43        /**< NVM PAR P9 high register */
#define NVM_PAR_P10         0x44        /**< NVM PAR P10 register */
#define NVM_PAR_P11         0x45        /**< NVM PAR P11 register */
#define CONFIG              0x1F        /**< configure register */
#define ODR                 0x1D        /**< odr register */
#define OSR                 0x1C        /**< osr register */
#define PWR_CTRL            0x1B        /**< power control register */
#define IF_CONF             0x1A        /**< if configure register */
#define INT_CTRL            0x19        /**< interrupt control register */
#define FIFO_CONFIG_2       0x18        /**< fifo configure 2 register */
#define FIFO_CONFIG_1       0x17        /**< fifo configure 1 register */
#define FIFO_WTM_1          0x16        /**< fifo watermark 1 register */
#define FIFO_WTM_0          0x15        /**< fifo watermark 2 register */
#define FIFO_DATA           0x14        /**< fifo data register */
#define FIFO_LENGTH_1       0x13        /**< fifo length 1 register */
#define FIFO_LENGTH_0       0x12        /**< fifo length 0 register */
#define INT_STATUS          0x11        /**< interrupt status register */
#define EVENT               0x10        /**< event register */
#define SENSORTIME_2        0x0E        /**< sensor time 2 register */
#define SENSORTIME_1        0x0D        /**< sensor time 1 register */
#define SENSORTIME_0        0x0C        /**< sensor time 0 register */
#define DATA_5              0x09        /**< data 5 register */
#define DATA_4              0x08        /**< data 4 register */
#define DATA_3              0x07        /**< data 3 register */
#define DATA_2              0x06        /**< data 2 register */
#define DATA_1              0x05        /**< data 1 register */
#define DATA_0              0x04        /**< data 0 register */
#define STATUS              0x03        /**< status register */
#define ERR_REG             0x02        /**< error register */
#define REV_ID              0x01        /**< rev id register */
#define CHIP_ID             0x00        /**< chip id register */


/**
 * @brief bmp390 error enumeration definition
 */
typedef enum
{
    ERROR_FATAL = (1 << 0),        /**< fatal error */
    ERROR_CMD   = (1 << 1),        /**< command error */
    ERROR_CONF  = (1 << 2),        /**< conf error */
} bmp390_error;

/**
 * @brief bmp390 status enumeration definition
 */
typedef enum
{
    STATUS_COMMAND_READY = (1 << 4),        /**< command ready status */
    STATUS_PRESS_READY   = (1 << 5),        /**< press ready status */
    STATUS_TEMP_READY    = (1 << 6),        /**< temp ready status */
} bmp390_status;

/**
 * @brief bmp390 event enumeration definition
 */
typedef enum
{
    EVENT_NONE                  = (0 << 0),         /**< event none */
    EVENT_POWER_UP_OR_SOFTRESET = (1 << 0),         /**< event power up or soft reset */
    EVENT_ITF_ACT_PT            = (2 << 0),         /**< a serial interface transaction occurs during a conversion */
    EVENT_BOTH                  = (3 << 0),         /**< event power up or soft reset &&
                                                         a serial interface transaction occurs during a conversion */
} bmp390_event;

/**
 * @brief bmp390 fifo data source enumeration definition
 */
typedef enum
{
    FIFO_DATA_SOURCE_UNFILTERED  = (0 << 3),        /**< fifo data source unfiltered */
    FIFO_DATA_SOURCE_FILTERED    = (1 << 3),        /**< fifo data source filtered */
} bmp390_fifo_data_source;

/**
 * @brief bmp390 interrupt status enumeration definition
 */
typedef enum
{
    INTERRUPT_STATUS_FIFO_WATERMARK = (1 << 0),     /**< fifo watermark interrupt status */
    INTERRUPT_STATUS_FIFO_FULL      = (1 << 1),     /**< fifo full interrupt status */
    INTERRUPT_STATUS_DATA_READY     = (1 << 3),     /**< data ready interrupt status */
} bmp390_interrupt_status;

/**
 * @brief bmp390 interrupt active level enumeration definition
 */
typedef enum
{
    INTERRUPT_ACTIVE_LEVEL_LOWER  = (0 << 1),       /**< active level lower */
    INTERRUPT_ACTIVE_LEVEL_HIGHER = (1 << 1),       /**< active level higher */
} bmp390_interrupt_active_level;

/**
 * @brief bmp390 interrupt pin type enumeration definition
 */
typedef enum
{
    INTERRUPT_PIN_TYPE_PUSH_PULL  = (0 << 0),       /**< push pull pin type */
    INTERRUPT_PIN_TYPE_OPEN_DRAIN = (1 << 0),       /**< open drain pin type */
} bmp390_interrupt_pin_type;

/**
 * @brief bmp390 spi wire enumeration definition
 */
typedef enum
{
    SPI_WIRE_4 = (0 << 0),      /**< 4 wire */
    SPI_WIRE_3 = (1 << 0),      /**< 3 wire */
} bmp390_spi_wire;

/**
 * @brief bmp390 iic watchdog period enumeration definition
 */
typedef enum
{
    IIC_WATCHDOG_PERIOD_1P25_MS = (0 << 2),     /**< iic watchdog period 1.25 ms */
    IIC_WATCHDOG_PERIOD_40_MS   = (1 << 2),     /**< iic watchdog period 40 ms */
} bmp390_iic_watchdog_period;

/**
 * @brief bmp390 mode enumeration definition
 */
typedef enum {
    MODE_SLEEP_MODE  = (0 << 4),        /**< sleep mode */
    MODE_FORCED_MODE = (1 << 4),        /**< forced mode */
    MODE_NORMAL_MODE = (3 << 4),        /**< normal mode */
} bmp390_mode;

/**
 * @brief bmp390 oversampling enumeration definition
 */
typedef enum {
    OVERSAMPLING_x1  = 0x00,        /**< oversampling x1 */
    OVERSAMPLING_x2  = 0x01,        /**< oversampling x2 */
    OVERSAMPLING_x4  = 0x02,        /**< oversampling x4 */
    OVERSAMPLING_x8  = 0x03,        /**< oversampling x8 */
    OVERSAMPLING_x16 = 0x04,        /**< oversampling x16 */
    OVERSAMPLING_x32 = 0x05,        /**< oversampling x32 */
} bmp390_oversampling;

/**
 * @brief bmp390 output data rate enumeration definition
 */
typedef enum {
    ODR_200    = 0x00,       /**< 200Hz */
    ODR_100    = 0x01,       /**< 100Hz */
    ODR_50     = 0x02,       /**< 50Hz */
    ODR_25     = 0x03,       /**< 25Hz */
    ODR_12P5   = 0x04,       /**< 12.5Hz */
    ODR_6P25   = 0x05,       /**< 6.25Hz */
    ODR_3P1    = 0x06,       /**< 3.1Hz */
    ODR_1P5    = 0x07,       /**< 1.5Hz */
    ODR_0P78   = 0x08,       /**< 0.78Hz */
    ODR_0P39   = 0x09,       /**< 0.39Hz */ 
    ODR_0P2    = 0x0A,       /**< 0.2Hz */ 
    ODR_0P1    = 0x0B,       /**< 0.1Hz */ 
    ODR_0P05   = 0x0C,       /**< 0.05Hz */
    ODR_0P02   = 0x0D,       /**< 0.02Hz */
    ODR_0P01   = 0x0E,       /**< 0.01Hz */
    ODR_0P006  = 0x0F,       /**< 0.006Hz */
    ODR_0P003  = 0x10,       /**< 0.003Hz */
    ODR_0P0015 = 0x11,       /**< 0.0015Hz */ 
} bmp390_odr;

/**
 * @brief bmp390 filter coefficient enumeration definition
 */
typedef enum {
    FILTER_COEFFICIENT_0   = (0x00 << 1),       /**< coefficient 0 */
    FILTER_COEFFICIENT_1   = (0x01 << 1),       /**< coefficient 1 */
    FILTER_COEFFICIENT_3   = (0x02 << 1),       /**< coefficient 3 */
    FILTER_COEFFICIENT_7   = (0x03 << 1),       /**< coefficient 7 */
    FILTER_COEFFICIENT_15  = (0x04 << 1),       /**< coefficient 15 */
    FILTER_COEFFICIENT_31  = (0x05 << 1),       /**< coefficient 31 */
    FILTER_COEFFICIENT_63  = (0x06 << 1),       /**< coefficient 63 */
    FILTER_COEFFICIENT_127 = (0x07 << 1),       /**< coefficient 127 */
} bmp390_filter_coefficient;

/**
 * @brief bmp390 frame type enumeration definition
 */
typedef enum {
    FRAME_HEADER_TEMPERATURE = 0x90,        /**< temperature frame type */
    FRAME_HEADER_PRESSURE    = 0x84,        /**< pressure frame type */
    FRAME_HEADER_SENSORTIME  = 0xA0,        /**< sensor time frame type */
} bmp390_frame_type;



/* BMP390 Base I2C Operations */

I2C_Mode BMP_readReg(uint8_t reg_addr, uint8_t *reg_data) {
    return I2C_readReg(bmp.addr, reg_addr, reg_data, 1);
}

I2C_Mode BMP_burstReadReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count) {
    return I2C_readReg(bmp.addr, reg_addr, reg_data, count);
}

I2C_Mode BMP_writeReg(uint8_t reg_addr, uint8_t *reg_data) {
    return I2C_writeReg(bmp.addr, reg_addr, reg_data, 1);
}

I2C_Mode BMP_burstWriteReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count) {
    return I2C_writeReg(bmp.addr, reg_addr, reg_data, count);
}


/* BMP390 Configuration */

uint8_t BMPdisable() {
    uint8_t new = 0x00;  // Disable Pressure and Temperature Sensors
    
    if (BMP_writeReg(PWR_CTRL, (uint8_t*) new) == NACK_MODE) {
        return 1;   // Register write failed
    }
    return 0;
}

uint8_t BMP_enableForcedMode() {
    uint8_t new = MODE_FORCED_MODE | (0b01);  // Enable Pressure Sensors in Forced Mode
    
    if (BMP_writeReg(PWR_CTRL, (uint8_t*) new) == NACK_MODE) {
        return 1;   // Register write failed
    }
    return 0;
}


/* BMP390 CMD Register Controls */

uint8_t BMP_flushFIFO() {
    uint8_t cmd = 0xB0;  // Flush contents of FIFO
    
    if (BMP_writeReg(PWR_CTRL, (uint8_t*) cmd) == NACK_MODE) {
        return 1;   // Register write failed
    }
    return 0;
}

uint8_t BMP_softReset() {
    uint8_t cmd = 0xB6;  // Reset User Configs to Default
    
    if (BMP_writeReg(PWR_CTRL, (uint8_t*) cmd) == NACK_MODE) {
        return 1;   // Register write failed
    }
    return 0;
}


/* BMP390 Sampling */

uint8_t BMP_getPressure(uint32_t *pressure) {
    uint8_t *buf = (uint8_t*) pressure;

    if (BMP_burstReadReg(DATA_0, buf, 3) == NACK_MODE) {
        return 1;   // Register read failed
    }
    return 0;
}
