#include "header.h"

void spi_cb(int event) {
    flags.set(SPI_FLAG);
}