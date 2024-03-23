/*
* RPC example to pass complete data structures over RCP link from M7 to M4
*  
*
* Build by : Jay Fox
* Date : 2024-03-2024
* Platform: Portenta H7
*
* License: open source MIT
*
*/

#include <Arduino.h>

#define MAXMESSAGE 64

// Defined Structures for RPC Passing 

struct M7Data { 
bool error;   
uint32_t timestamp;
uint16_t pcounter;
bool alive;
bool init;
uint16_t points;
uint16_t kills;
uint8_t identity;
char message[MAXMESSAGE];
};

struct M4Data { 
bool error;        
uint32_t timestamp;
uint16_t pcounter;
bool alive;
bool init;
uint16_t posx;
uint16_t posy;
uint16_t bul1x;
uint16_t bul1y;
uint16_t bul2x;
uint16_t bul2y;
bool shoot1;
bool shoot2;
uint8_t identity;
uint8_t key;
char message[MAXMESSAGE];
};