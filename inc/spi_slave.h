#ifndef _SPI_SLAVE_H
#define _SPI_SLAVE_H

/**
 * @brief SPI Slave Init
 * @param void
 * @return void
 */
void spi_slave_init(void);

/**
 * @brief SPI Slave Register TX Buffer
 * @param txbuffer
 * @param txbuflen
 * @return void
 */
void spi_slave_register_txbuffer(void *txbuffer, uint8_t txbuflen);

/**
 * @brief SPI Slave Register TX Done Handler
 * @param txdone_handler
 * @return void
 */
void spi_slave_register_txdone_handler(void (*txdone_handler)());

#endif // _SPI_SLAVE_H