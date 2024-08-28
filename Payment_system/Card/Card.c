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


EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    printf("Enter The Name of Cardholder please : \n");
    fgets((char*)cardData->cardHolderName, sizeof(cardData->cardHolderName), stdin);///////////problem here!!!!!!!!!!!! the name more than 24 is allowed to be entered by the user
//instead of scanf fgets to allow the spaces between name
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

    // Test Case 1: Valid name
    strcpy((char*)cardData.cardHolderName, "John Doe");
    result = getCardHolderName(&cardData);
    printf("Tester Name: Your Name\n");
    printf("Function Name: getCardHolderName\n");
    printf("Test Case 1:\nInput Data: John Doe\nExpected Result: CARD_OK\nActual Result: %d\n\n", result);

    // Test Case 2: Name too short
    strcpy((char*)cardData.cardHolderName, "J");
    result = getCardHolderName(&cardData);
    printf("Test Case 2:\nInput Data: J\nExpected Result: WRONG_NAME\nActual Result: %d\n\n", result);

    // Test Case 3: Name too long
    strcpy((char*)cardData.cardHolderName, "This name is definitely too long");
    result = getCardHolderName(&cardData);
    printf("Test Case 3:\nInput Data: This name is definitely too long\nExpected Result: WRONG_NAME\nActual Result: %d\n\n", result);

    // Test Case 4: Empty name
    strcpy((char*)cardData.cardHolderName, "");
    result = getCardHolderName(&cardData);
    printf("Test Case 4:\nInput Data: (empty string)\nExpected Result: WRONG_NAME\nActual Result: %d\n\n", result);

    // Test Case 5: Valid name with spaces
    strcpy((char*)cardData.cardHolderName, "Jane Smith");
    result = getCardHolderName(&cardData);
    printf("Test Case 5:\nInput Data: Jane Smith\nExpected Result: CARD_OK\nActual Result: %d\n\n", result);
}




EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData) {
    printf("Enter the Card Expiry Date (MM/YY) Please: \n");
    fgets((char*)cardData->cardExpirationDate, sizeof(cardData->cardExpirationDate), stdin);

    int DateLength = strlen((char*)cardData->cardExpirationDate);

    // Remove newline character
    if (DateLength > 0 && cardData->cardExpirationDate[DateLength - 1] == '\n') {
        cardData->cardExpirationDate[DateLength - 1] = '\0';
        DateLength--;
    }

    // Check if the length is exactly 5 characters
    if (DateLength != EXPIRY_DATE_LENGTH) {
        return WRONG_EXP_DATE;
    }

    // Check for the correct format "MM/YY"
    if (cardData->cardExpirationDate[2] != '/' ||
        !isdigit(cardData->cardExpirationDate[0]) ||
        !isdigit(cardData->cardExpirationDate[1]) ||
        !isdigit(cardData->cardExpirationDate[3]) ||
        !isdigit(cardData->cardExpirationDate[4])) {
        return WRONG_EXP_DATE;
    }

    return CARD_OK;
}


void getCardExpiryDateTest(void) {
    ST_cardData_t cardData;

    // Test Case 1: Valid date
    strcpy((char*)cardData.cardExpirationDate, "05/25");
    EN_cardError_t result = getCardExpiryDate(&cardData);
    printf("Tester Name: Your Name\nFunction Name: getCardExpiryDate\nTest Case 1:\nInput Data: %s\nExpected Result: CARD_OK\nActual Result: %s\n\n",
           cardData.cardExpirationDate, result == CARD_OK ? "CARD_OK" : "WRONG_EXP_DATE");

    // Test Case 2: Invalid date (too short)
    strcpy((char*)cardData.cardExpirationDate, "5/25");
    result = getCardExpiryDate(&cardData);
    printf("Test Case 2:\nInput Data: %s\nExpected Result: WRONG_EXP_DATE\nActual Result: %s\n\n",
           cardData.cardExpirationDate, result == CARD_OK ? "CARD_OK" : "WRONG_EXP_DATE");

    // Test Case 3: Invalid date (no slash)
    strcpy((char*)cardData.cardExpirationDate, "0525");
    result = getCardExpiryDate(&cardData);
    printf("Test Case 3:\nInput Data: %s\nExpected Result: WRONG_EXP_DATE\nActual Result: %s\n\n",
           cardData.cardExpirationDate, result == CARD_OK ? "CARD_OK" : "WRONG_EXP_DATE");

    // Test Case 4: Invalid date (too long)
    strcpy((char*)cardData.cardExpirationDate, "05/2025");
    result = getCardExpiryDate(&cardData);
    printf("Test Case 4:\nInput Data: %s\nExpected Result: WRONG_EXP_DATE\nActual Result: %s\n\n",
           cardData.cardExpirationDate, result == CARD_OK ? "CARD_OK" : "WRONG_EXP_DATE");


}

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

    // Test Case 1: Valid PAN
    strcpy((char*)cardData.primaryAccountNumber, "1234567890123456"); // 16 characters
    result = getCardPAN(&cardData);
    printf("Test Case 1:\nInput Data: 1234567890123456\nExpected Result: CARD_OK\n");
    printf("Actual Result: %s\n", result == CARD_OK ? "CARD_OK" : "WRONG_PAN");
    printf("\n");

    // Test Case 2: PAN less than 16 characters
    strcpy((char*)cardData.primaryAccountNumber, "12345678901234"); // 14 characters
    result = getCardPAN(&cardData);
    printf("Test Case 2:\nInput Data: 12345678901234\nExpected Result: WRONG_PAN\n");
    printf("Actual Result: %s\n", result == WRONG_PAN ? "WRONG_PAN" : "CARD_OK");
    printf("\n");

    // Test Case 3: PAN more than 19 characters
    strcpy((char*)cardData.primaryAccountNumber, "12345678901234567890"); // 20 characters
    result = getCardPAN(&cardData);
    printf("Test Case 3:\nInput Data: 12345678901234567890\nExpected Result: WRONG_PAN\n");
    printf("Actual Result: %s\n", result == WRONG_PAN ? "WRONG_PAN" : "CARD_OK");
    printf("\n");

    // Test Case 4: PAN with exactly 19 characters
    strcpy((char*)cardData.primaryAccountNumber, "1234567890123456789"); // 19 characters
    result = getCardPAN(&cardData);
    printf("Test Case 4:\nInput Data: 1234567890123456789\nExpected Result: CARD_OK\n");
    printf("Actual Result: %s\n", result == CARD_OK ? "CARD_OK" : "WRONG_PAN");
    printf("\n");

    // Test Case 5: Empty PAN
    strcpy((char*)cardData.primaryAccountNumber, "");
    result = getCardPAN(&cardData);
    printf("Test Case 5:\nInput Data: [Empty String]\nExpected Result: WRONG_PAN\n");
    printf("Actual Result: %s\n", result == WRONG_PAN ? "WRONG_PAN" : "CARD_OK");
    printf("\n");
}


