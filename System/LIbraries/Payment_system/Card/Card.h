#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

#include <stdint.h>

#define MAXNAME 24
#define MINNAME 20
#define EXPIRY_DATE_LENGTH 5
#define MAXNUMBEr 19
#define MINNUMBEr 16

typedef struct ST_cardData_t
{
uint8_t cardHolderName[25];
uint8_t primaryAccountNumber[20];
uint8_t cardExpirationDate[6];
}ST_cardData_t;

typedef enum EN_cardError_t
{
CARD_OK,
 WRONG_NAME,
  WRONG_EXP_DATE,
 WRONG_PAN,
}EN_cardError_t;

EN_cardError_t getCardHolderName(ST_cardData_t *cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData);
EN_cardError_t getCardPAN(ST_cardData_t *cardData);

#endif // CARD_H_INCLUDED
