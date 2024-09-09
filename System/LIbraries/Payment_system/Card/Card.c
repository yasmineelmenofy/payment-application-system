#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "Card.h"

/*
 * FILE: Card.c
 * AUTHOR: Yasmine Elmenofy
 * BRIEF: This section of the project deals with card and taking the card information from the user
 * with checking if the information meet the requirements or not
 * DATA: 22/8/2024
 */

/*
 * This function prompts the user to enter the cardholder's name
 * and validates it against the required length.
 */

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    printf("Enter The Name of Cardholder please : \n");
    fgets((char*)cardData->cardHolderName, sizeof(cardData->cardHolderName), stdin);
    int nameLength = strlen((char*)cardData->cardHolderName);
    if (nameLength > 0 && cardData->cardHolderName[nameLength - 1] == '\n') {
    cardData->cardHolderName[nameLength - 1] = '\0';
    nameLength--;
}
    if (nameLength == 0 || nameLength > MAXNAME || nameLength < MINNAME) {
        return WRONG_NAME;
    }
    else
    {
        return CARD_OK;
    }
}

void getCardHolderNameTest(void) {
    ST_cardData_t cardData;
    EN_cardError_t result;

    strcpy((char*)cardData.cardHolderName, "John Doe");
    result = getCardHolderName(&cardData);
    printf("Tester Name: Your Name\n");
    printf("Function Name: getCardHolderName\n");
    printf("Test Case 1:\nInput Data: John Doe\nExpected Result: CARD_OK\nActual Result: %d\n\n", result);
    strcpy((char*)cardData.cardHolderName, "J");
    result = getCardHolderName(&cardData);
    printf("Test Case 2:\nInput Data: J\nExpected Result: WRONG_NAME\nActual Result: %d\n\n", result);
    strcpy((char*)cardData.cardHolderName, "This name is definitely too long");
    result = getCardHolderName(&cardData);
    printf("Test Case 3:\nInput Data: This name is definitely too long\nExpected Result: WRONG_NAME\nActual Result: %d\n\n", result);
    strcpy((char*)cardData.cardHolderName, "");
    result = getCardHolderName(&cardData);
    printf("Test Case 4:\nInput Data: (empty string)\nExpected Result: WRONG_NAME\nActual Result: %d\n\n", result);
    strcpy((char*)cardData.cardHolderName, "Jane Smith");
    result = getCardHolderName(&cardData);
    printf("Test Case 5:\nInput Data: Jane Smith\nExpected Result: CARD_OK\nActual Result: %d\n\n", result);
}



/*
 * This function prompts the user to enter the card expiry date
 * and validates it in the MM/YY format.
 */
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData) {
    printf("Enter the Card Expiry Date (MM/YY) Please: \n");
    fgets((char*)cardData->cardExpirationDate, sizeof(cardData->cardExpirationDate), stdin);

    int DateLength = strlen((char*)cardData->cardExpirationDate);

    if (DateLength > 0 && cardData->cardExpirationDate[DateLength - 1] == '\n') {
        cardData->cardExpirationDate[DateLength - 1] = '\0';
        DateLength--;
    }

    if (DateLength != EXPIRY_DATE_LENGTH) {
        return WRONG_EXP_DATE;
    }

    if (cardData->cardExpirationDate[2] != '/' ||
        !isdigit(cardData->cardExpirationDate[0]) ||
        !isdigit(cardData->cardExpirationDate[1]) ||
        !isdigit(cardData->cardExpirationDate[3]) ||
        !isdigit(cardData->cardExpirationDate[4])) {
        return WRONG_EXP_DATE;
    }
    int month = (cardData->cardExpirationDate[0] - '0') * 10 + (cardData->cardExpirationDate[1] - '0');
    int year = (cardData->cardExpirationDate[3] - '0') * 10 + (cardData->cardExpirationDate[4] - '0');

    if (month < 1 || month > 12) {
        return WRONG_EXP_DATE;
    }

    return CARD_OK;
}

void getCardExpiryDateTest(void) {
    ST_cardData_t cardData;
    strcpy((char*)cardData.cardExpirationDate, "05/25");
    EN_cardError_t result = getCardExpiryDate(&cardData);
    printf("Tester Name: Your Name\nFunction Name: getCardExpiryDate\nTest Case 1:\nInput Data: %s\nExpected Result: CARD_OK\nActual Result: %s\n\n",
           cardData.cardExpirationDate, result == CARD_OK ? "CARD_OK" : "WRONG_EXP_DATE");
    strcpy((char*)cardData.cardExpirationDate, "5/25");
    result = getCardExpiryDate(&cardData);
    printf("Test Case 2:\nInput Data: %s\nExpected Result: WRONG_EXP_DATE\nActual Result: %s\n\n",
           cardData.cardExpirationDate, result == CARD_OK ? "CARD_OK" : "WRONG_EXP_DATE");
    strcpy((char*)cardData.cardExpirationDate, "0525");
    result = getCardExpiryDate(&cardData);
    printf("Test Case 3:\nInput Data: %s\nExpected Result: WRONG_EXP_DATE\nActual Result: %s\n\n",
           cardData.cardExpirationDate, result == CARD_OK ? "CARD_OK" : "WRONG_EXP_DATE");
    strcpy((char*)cardData.cardExpirationDate, "05/2025");
    result = getCardExpiryDate(&cardData);
    printf("Test Case 4:\nInput Data: %s\nExpected Result: WRONG_EXP_DATE\nActual Result: %s\n\n",
           cardData.cardExpirationDate, result == CARD_OK ? "CARD_OK" : "WRONG_EXP_DATE");


}

/*
 * This function prompts the user to enter the card's Primary Account Number (PAN)
 * and validates its length and format (numeric characters only).
 */
EN_cardError_t getCardPAN(ST_cardData_t *cardData) {
    printf("Enter the Primary Account Number (PAN) please: \n");
    if (fgets((char*)cardData->primaryAccountNumber, sizeof(cardData->primaryAccountNumber), stdin) == NULL) {
        return WRONG_PAN;
    }

    int NumberLength = strlen((char*)cardData->primaryAccountNumber);
    if (NumberLength > 0 && cardData->primaryAccountNumber[NumberLength - 1] == '\n') {
        cardData->primaryAccountNumber[NumberLength - 1] = '\0';
        NumberLength--;
    }

    if (NumberLength == 0 || NumberLength > MAXNUMBEr || NumberLength < MINNUMBEr) {
        return WRONG_PAN;
    }

    for (int i = 0; i < NumberLength; i++) {
        if (cardData->primaryAccountNumber[i] < '0' || cardData->primaryAccountNumber[i] > '9') {
            return WRONG_PAN;
        }
    }

    return CARD_OK;
}
void getCardPANTest(void) {
    ST_cardData_t cardData;
    EN_cardError_t result;
    printf("Tester Name: Your Name\n");
    printf("Function Name: getCardPAN\n");
    strcpy((char*)cardData.primaryAccountNumber, "1234567890123456"); // 16 characters
    result = getCardPAN(&cardData);
    printf("Test Case 1:\nInput Data: 1234567890123456\nExpected Result: CARD_OK\n");
    printf("Actual Result: %s\n", result == CARD_OK ? "CARD_OK" : "WRONG_PAN");
    printf("\n");
    strcpy((char*)cardData.primaryAccountNumber, "12345678901234"); // 14 ch
    result = getCardPAN(&cardData);
    printf("Test Case 2:\nInput Data: 12345678901234\nExpected Result: WRONG_PAN\n");
    printf("Actual Result: %s\n", result == WRONG_PAN ? "WRONG_PAN" : "CARD_OK");
    printf("\n");
    strcpy((char*)cardData.primaryAccountNumber, "12345678901234567890");
    result = getCardPAN(&cardData);
    printf("Test Case 3:\nInput Data: 12345678901234567890\nExpected Result: WRONG_PAN\n");
    printf("Actual Result: %s\n", result == WRONG_PAN ? "WRONG_PAN" : "CARD_OK");
    printf("\n");
    strcpy((char*)cardData.primaryAccountNumber, "1234567890123456789");
    result = getCardPAN(&cardData);
    printf("Test Case 4:\nInput Data: 1234567890123456789\nExpected Result: CARD_OK\n");
    printf("Actual Result: %s\n", result == CARD_OK ? "CARD_OK" : "WRONG_PAN");
    printf("\n");
    strcpy((char*)cardData.primaryAccountNumber, "");
    result = getCardPAN(&cardData);
    printf("Test Case 5:\nInput Data: [Empty String]\nExpected Result: WRONG_PAN\n");
    printf("Actual Result: %s\n", result == WRONG_PAN ? "WRONG_PAN" : "CARD_OK");
    printf("\n");
}

EN_cardError_t processCardDetails(ST_cardData_t *cardData) {
    EN_cardError_t cardStatus;

    clearInputBuffer();
    cardStatus = getCardHolderName(cardData);
    if (cardStatus != CARD_OK) {
        printf("Invalid cardholder name.\n");
        return cardStatus;
    }

    clearInputBuffer();
    cardStatus = getCardExpiryDate(cardData);
    if (cardStatus != CARD_OK) {
        printf("Invalid expiry date.\n");
        return cardStatus;
    }

    clearInputBuffer();
    cardStatus = getCardPAN(cardData);
    if (cardStatus != CARD_OK) {
        printf("Invalid PAN.\n");
        return cardStatus;
    }

    clearInputBuffer();
    if (isValidCardPAN(cardData) != TERMINAL_OK) {
        printf("Invalid card PAN. Please check the card details.\n");
        return TERMINAL_ERROR ;
    }

    return CARD_OK;
}

