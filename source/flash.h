#ifndef __FLASH_H_
#define __FLASH_H_

unsigned char Flash_Read(unsigned int addr);
unsigned int Flash_Read_Int(unsigned int addr);
void Flash_Erase(unsigned int addr);
void Flash_Write(unsigned int addr,unsigned char dat);
 
#endif  
                                                                                                                                                                                                                                       