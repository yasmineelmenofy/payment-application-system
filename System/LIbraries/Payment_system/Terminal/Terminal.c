#include <stdio.h>
#include <stdlib.h>

#include "Terminal.h"

/*
 * Prompts the user to enter the transaction date in the format DD/MM/YYYY.
 * Validates the date format and checks for logical correctness (e.g., day and month ranges).
 * Returns TERMINAL_OK if the date is valid; otherwise, returns WRONG_DATE.
 */
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    printf("Enter the Transaction Date (DD/MM/YYYY) please:\n");
    fgets(termData->transactionDate, sizeof(termData->transactionDate), stdin);

    int TdateLength = strlen((char*)termData->transactionDate);

    if (TdateLength > 0 && termData->transactionDate[TdateLength - 1] == '\n') {
        termData->transactionDate[TdateLength - 1] = '\0';
        TdateLength--;
    }

    if (TdateLength != TRANSACATION_DATE_LENGTH || TdateLength == 0) {
        return WRONG_DATE;
    }

    if (termData->transactionDate[2] != '/' || termData->transactionDate[5] != '/' ||
        !isdigit(termData->transactionDate[0]) || !isdigit(termData->transactionDate[1]) ||
        !isdigit(termData->transactionDate[3]) || !isdigit(termData->transactionDate[4]) ||
        !isdigit(termData->transactionDate[6]) || !isdigit(termData->transactionDate[7]) ||
        !isdigit(termData->transactionDate[8]) || !isdigit(termData->transactionDate[9])) {
        return WRONG_DATE;
    }


    int day = (termData->transactionDate[0] - '0') * 10 + (termData->transactionDate[1] - '0');
    int month = (termData->transactionDate[3] - '0') * 10 + (termData->transactionDate[4] - '0');
    int year = (termData->transactionDate[6] - '0') * 1000 +
               (termData->transactionDate[7] - '0') * 100 +
               (termData->transactionDate[8] - '0') * 10 +
               (termData->transactionDate[9] - '0');


    if (month < 1 || month > 12) {
        return WRONG_DATE;
    }

    if (day < 1 || day > 31 ||
        (month == 2 && day > 29) ||
        ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)) {
        return WRONG_DATE;
    }



    return TERMINAL_OK;
}

/*
 * Checks if the card is expired based on the transaction date and the card's expiration date.
 * Returns TERMINAL_OK if the card is not expired; otherwise, returns EXPIRED_CARD.
 */

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData) {
    int day1 = (termData->transactionDate[0] - '0') * 10 + (termData->transactionDate[1] - '0');
    int month1 = (termData->transactionDate[3] - '0') * 10 + (termData->transactionDate[4] - '0');
    int day2 = (cardData->cardExpirationDate[0] - '0') * 10 + (cardData->cardExpirationDate[1] - '0');
    int month2 = (cardData->cardExpirationDate[3] - '0') * 10 + (cardData->cardExpirationDate[4] - '0');
    if (month1 < month2) {
        return TERMINAL_OK;
    } else if (month1 > month2) {
        return EXPIRED_CARD;
    } else {
        if (day1 > day2) {
            return EXPIRED_CARD;
        } else {
            return TERMINAL_OK;
        }
    }
}

/*
 * Prompts the user to enter the transaction amount and validates the input.
 * Returns TERMINAL_OK if the amount is valid; otherwise, returns INVALID_AMOUNT.
 */
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData) {
    printf("Enter the Amount of Transaction Please:\n");


    if (scanf("%f", &termData->transAmount) != 1 || termData->transAmount <= 0) {
        return INVALID_AMOUNT;
    }

    return TERMINAL_OK;
}

/*
 * Checks if the transaction amount exceeds the maximum allowed amount.
 * Returns TERMINAL_OK if the amount is within the limit; otherwise, returns EXCEED_MAX_AMOUNT.
 */
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData) {
    if (termData->transAmount > termData->maxTransAmount) {
        return EXCEED_MAX_AMOUNT;
    }
    return TERMINAL_OK;
}
/*
 * Sets the maximum transaction amount for the terminal.
 * Returns TERMINAL_OK if the maximum amount is set successfully; otherwise, returns INVALID_MAX_AMOUNT.
 */
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount) {
    if (maxAmount <= 0) {
        return INVALID_MAX_AMOUNT;
    }

    termData->maxTransAmount = maxAmount;
    return TERMINAL_OK;
}


/*
 * Validates the card's Primary Account Number (PAN) using the Luhn algorithm.
 * Returns TERMINAL_OK if the PAN is valid; otherwise, returns INVALID_CARD.
 */
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData)
{
    int len = strlen(cardData->primaryAccountNumber);
    int sum = 0;
    int doubleDigit = 0;

    for (int i = len - 1; i >= 0; i--) {
        int digit = cardData->primaryAccountNumber[i] - '0';

        if (doubleDigit) {
            digit *= 2;
            if (digit > 9) {
                digit -= 9;
            }
        }

        sum += digit;
        doubleDigit = !doubleDigit;
    }

    if (sum % 10 == 0) {
        return TERMINAL_OK;
    }
    else {
        return INVALID_CARD;
    }
}
