#STM32F4 output to VGA monitor (1 bit color, green)
This code is for stm32f4vg, and it does VGA output via SPI & DMA with timers. It's great that output made on peripherals only, so it has only little affect on the main program flow.
Original code developed by Artekit team for stm32f103 & ported on f4 series by Karl Lunt

Now I try to improve/optimize/prepare it to be useful in graphics output/etc
Output:

|VGA pin | STM32F4 pin  |  Channel                 | Comment
|--------|--------------|--------------------------|----
|   2    |   PB5        |  Pixel Data GREEN        | Can be changed into VGA Pin 1 (RED) or 3 (BLUE). MUST be connected with resistor divider (270 Ohm to GND, 120 Ohm to PB5). Plan to introduce R2R with regulated colors (Probably with FPGA)
| 5,10   |   GND (any)  |  GND for digital signals | Must be connected with MCU or board GND pin
|  13    |   PA8        |  Hsync                   |
|  14    |   PA1        |  Vsync                   |
   


