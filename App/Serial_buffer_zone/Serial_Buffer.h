#ifndef __SERIAL_BUFFER_H_
#define __SERIAL_BUFFER_H_

//缓冲区大小
#define MAXSIZE (uint16_t)30

void Enpty_Buffer(void);
uint8_t EnterQueue(uint8_t Data);
uint8_t OutQueue(uint8_t *Data);

#endif
