#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "rc522_tcp.h"


int socket_fd, connection_fd;

void rc522_tcp_listen(int port) {
    
    struct sockaddr_in servaddr, cli;
    
    // socket create and verification
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (socket_fd == -1) {
        printf("Socket creation failed.\n");
        exit(0);
    }else{
        printf("Socket successfully created.\n");
    }
    memset(&servaddr, 0, sizeof(servaddr));
    
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    
    int one = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    
    // Binding newly created socket to given IP and verification
    if ((bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed.\n");
        exit(0);
    }else{
        printf("Socket bound.\n");
    }

    // Now server is ready to listen and verification
    if ((listen(socket_fd, 5)) != 0) {
        printf("Listen failed.\n");
        exit(0);
    }else{
        printf("Socket listening on port %i...\n", port);
    }
    
    // Wait for a connection.
    int len = sizeof(cli);
    connection_fd = accept(socket_fd, (struct sockaddr*)&cli, &len);

    if (connection_fd < 0) { 
        printf("Socket acccept failed.\n"); 
        exit(0); 
    }else{
        printf("Connected.\n"); 
    }
}


void rc522_tcp_spi_transcieve(uint8_t* data, uint8_t length) {

    uint8_t* operation = malloc(length + 2);
    rc522_tcp_operation_result_t result;

    operation[0] = SPI_TRANSCEIVE;       // Operation code
    operation[1] = length;               // SPI Transceive length
    memcpy(operation + 2, data, length); // Data to tranceive on SPI

    write(connection_fd, operation, length + 2);
    read(connection_fd, &result, 1);     // Read operation result
    read(connection_fd, data, length);   // Read SPI response

    free(operation);

    if (result != OPERATION_OK) {
        printf("SPI_TRANSCEIVE failed. Operation result: %02X\n", result);
        exit(result);
    }
}


rc522_tcp_gpio_state_t rc522_tcp_rst_gpio_get() {

    uint8_t* operation = malloc(2);

    operation[0] = RST_GPIO_GET;         // Operation code
    operation[1] = 0;                    // Ignored argument

    rc522_tcp_operation_result_t result; // Operation result
    rc522_tcp_gpio_state_t gpio_state;   // GPIO state

    write(connection_fd, operation, 2);
    read(connection_fd, &result, 1);     // Read operation result
    read(connection_fd, &gpio_state, 1); // Read gpio state

    if (result != OPERATION_OK) {
        printf("RST_GPIO_GET failed. Operation result: %02X\n", result);
        exit(result);
    }

    return gpio_state;
}

rc522_tcp_operation_result_t rc522_tcp_rst_gpio_set(rc522_tcp_gpio_state_t state) {

    uint8_t* operation = malloc(2);
    rc522_tcp_operation_result_t result;

    operation[0] = RST_GPIO_SET;     // Operation code
    operation[1] = state;            // GPIO state to set

    write(connection_fd, operation, 2);
    read(connection_fd, &result, 1); // Read operation result
    
    free(operation);
    return result;
}