#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED
#include <stdint.h>
#include "Card.h"
#define TRANSACATION_DATE_LENGTH 10

/*
 * FILE: terminal.h
 * AUTHOR: Yasmine Elmenofy
 * This is the header file of terminal module
 *contains the structures that carry terminal data
 *contains also the prototypes of the used function in the module
 *Date :25/08/2024
 */



typedef struct ST_terminalData_t
{
float transAmount;
float maxTransAmount;
uint8_t transactionDate[11];
}ST_terminalData_t;

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData);
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t*
termData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float
maxAmount);
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData);


#endif // TERMINAL_H_INCLUDED
