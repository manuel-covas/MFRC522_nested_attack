#ifndef RC552_TCP_H
#define RC552_TCP_H

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    SPI_TRANSCEIVE,
    RST_GPIO_GET,
    RST_GPIO_SET
}
rc522_tcp_operation_t;

typedef enum {
    OPERATION_OK,
    OPERATION_FAIL
}
rc522_tcp_operation_result_t;

typedef enum {
    GPIO_LOW,
    GPIO_HIGH
}
rc522_tcp_gpio_state_t;


void rc522_tcp_listen(int port);
void rc522_tcp_spi_transcieve(uint8_t* data, uint8_t length);
rc522_tcp_gpio_state_t rc522_tcp_rst_gpio_get();
rc522_tcp_operation_result_t rc522_tcp_rst_gpio_set(rc522_tcp_gpio_state_t state);


#ifdef __cplusplus
}
#endif

#endif