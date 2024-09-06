#include <stdio.h>
#include <stdlib.h>
#include "Server.h"

/*
 * FILE: server.c
 * AUTHOR: Yasmine Elmenofy
 * BRIEF: This section of the project deal with
 *  the server-side operations of the payment application.
 * It manages account validation, transaction processing, and data persistence.
 * Functions in this module check account status, verify available funds, and
 * save transaction records to a file, ensuring secure and accurate payment handling.
 *account or edit account or view transaction history
 * DATA: 27/08/2024
 */


/*
 * Validates the provided transaction data and checks account status before processing the transaction.
 * Updates account balance and saves the transaction if all checks are passed.
 */
EN_transState_t receiveTransactionData(ST_transaction *transData) {
    ST_accountsDB_t accountReference;
    printf("Validating account for PAN: %s\n", transData->cardHolderData.primaryAccountNumber);
    if (isValidAccount(&transData->cardHolderData, &accountReference) != SERVER_OK) {
        printf("Account not found!\n");
        return FRAUD_CARD;
    }
    printf("Account found: %s, Balance: %.2f, State: %d\n",
           accountReference.primaryAccountNumber, accountReference.balance, accountReference.state);
    if (isBlockedAccount(&accountReference) != SERVER_OK) {
        printf("Account is blocked!\n");
        return DECLINED_STOLEN_CARD;
    }else
       printf("Account is not blocked\n");

    if (isAmountAvailable(&transData->terminalData, &accountReference) != SERVER_OK) {
        printf("Insufficient funds! Available: %.2f, Requested: %.2f\n",
               accountReference.balance, transData->terminalData.transAmount);
        return DECLINED_INSUFFECIENT_FUND;
    }else
    {
        printf("The fund is sufficient \n");
    }

    transData->transState = APPROVED;
    if (saveTransaction(transData) != SERVER_OK) {
        printf("Error saving transaction!\n");
        return INTERNAL_SERVER_ERROR;
    }
    else
    {
        printf("transaction saved successfully \n");
    }

    listnode *current = accountList.head;
    while (current != NULL) {
        if (strcmp(current->entry.accountRecord.primaryAccountNumber, transData->cardHolderData.primaryAccountNumber) == 0) {
            current->entry.accountRecord.balance -= transData->terminalData.transAmount;
            printf("Updated balance for PAN: %s, New Balance: %.2f\n",
                   current->entry.accountRecord.primaryAccountNumber, current->entry.accountRecord.balance);
             updateAccountBalance("accounts.txt", &current->entry.accountRecord);
            break;
        }
        current = current->next;
    }

    return APPROVED;
}


/*
 * Checks if the account associated with the provided PAN exists in the account list.
 * If found, it retrieves the account details into the provided reference structure.
 */
EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t* accountReference) {
    listnode *current = accountList.head;

    while (current != NULL) {
        printf("Checking account: %s against card PAN: %s\n",
               current->entry.accountRecord.primaryAccountNumber,
               cardData->primaryAccountNumber);
        if (strcmp(current->entry.accountRecord.primaryAccountNumber, cardData->primaryAccountNumber) == 0) {
            accountReference->balance = current->entry.accountRecord.balance;
            accountReference->state = current->entry.accountRecord.state;
            strcpy(accountReference->primaryAccountNumber, current->entry.accountRecord.primaryAccountNumber);

            return SERVER_OK;
        }
        current = current->next;
    }

    printf("Account not found for PAN: %s\n", cardData->primaryAccountNumber);
    return ACCOUNT_NOT_FOUND;
}


/*
 * Verifies if the account balance is sufficient to cover the requested transaction amount.
 * Returns a status indicating whether sufficient funds are available.
 */
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence) {
    if (accountRefrence->balance >= termData->transAmount) {
        return SERVER_OK;
    } else {
        return LOW_BALANCE;
    }
}


/*
 * Checks the account's state to determine if it is currently blocked or active.
 * Returns a status indicating whether the account is running or blocked.
 */
  EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence) {
    if (accountRefrence->state == RUNNING) {
        return SERVER_OK;
    } else {
        return BLOCKED_ACCOUNT;
    }
}

/*
 * Saves the transaction details to a file, appending the information in a predefined format.
 * Returns a status indicating the success or failure of the file operation.
 */
EN_serverError_t saveTransaction(ST_transaction *transData) {
    FILE *file = fopen("transactions.txt", "a");
    if (file == NULL) {
        return SAVING_FAILED;
    } else {
        printf("File is opened \n");
    }

    fprintf(file, "%u,%s,%.2f,%d,%.2f,%s,%s,%s\n",
            transData->transactionSequenceNumber,
            transData->terminalData.transactionDate,
            transData->terminalData.transAmount,
            transData->transState,
            transData->terminalData.maxTransAmount,
            transData->cardHolderData.cardHolderName,
            transData->cardHolderData.primaryAccountNumber,
            transData->cardHolderData.cardExpirationDate);

    fclose(file);

    printf("Transaction saved successfully.\n");

    return SERVER_OK;
}


/*
 * Displays the transaction history from the saved transactions list.
 * If no transactions are found, it notifies the user accordingly.
 */

void listSavedTransactions(void)
{
    if (isEmptyList(&transactionList)) {
        printf("No transaction history available.\n");
        return;
    }

    listnode *current = transactionList.head;
    while (current != NULL)
    {
        printf("Transaction Sequence Number: %u\n", current->entry.transactionRecord.transactionSequenceNumber);
        printf("Transaction Date: %s\n", current->entry.transactionRecord.terminalData.transactionDate);
        printf("Transaction Amount: %.2f\n", current->entry.transactionRecord.terminalData.transAmount);
        printf("Transaction State: %d\n", current->entry.transactionRecord.transactionState);
        printf("Terminal Max Amount: %.2f\n", current->entry.transactionRecord.terminalData.maxTransAmount);
       printf("Cardholder Name: %s\n", current->entry.transactionRecord.cardHolderData.cardHolderName);
    printf("PAN: %s\n", current->entry.transactionRecord.cardHolderData.primaryAccountNumber);
      printf("Card Expiration Date: %s\n", current->entry.transactionRecord.cardHolderData.cardExpirationDate);


        current = current->next;
    }
}




