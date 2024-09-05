#include <stdio.h>
#include <stdlib.h>
#include "Server.h"

EN_transState_t receiveTransactionData(ST_transaction *transData) {
    ST_accountsDB_t accountReference;

    // Validate the account
    printf("Validating account for PAN: %s\n", transData->cardHolderData.primaryAccountNumber);
    if (isValidAccount(&transData->cardHolderData, &accountReference) != SERVER_OK) {
        printf("Account not found!\n");
        return FRAUD_CARD; // Account not found
    }
    printf("Account found: %s, Balance: %.2f, State: %d\n",
           accountReference.primaryAccountNumber, accountReference.balance, accountReference.state);

    // Check if the account is blocked
    if (isBlockedAccount(&accountReference) != SERVER_OK) {
        printf("Account is blocked!\n");
        return DECLINED_STOLEN_CARD; // Account is blocked
    }

    // Check if the amount is available
    if (isAmountAvailable(&transData->terminalData, &accountReference) != SERVER_OK) {
        printf("Insufficient funds! Available: %.2f, Requested: %.2f\n",
               accountReference.balance, transData->terminalData.transAmount);
        return DECLINED_INSUFFECIENT_FUND; // Insufficient funds
    }

    // Save the transaction
    transData->transState = APPROVED;
    if (saveTransaction(transData) != SERVER_OK) {
        printf("Error saving transaction!\n");
        return INTERNAL_SERVER_ERROR; // Error saving transaction
    }

    // Update the account balance
    listnode *current = accountList.head;
    while (current != NULL) {
        if (strcmp(current->entry.accountRecord.primaryAccountNumber, transData->cardHolderData.primaryAccountNumber) == 0) {
            current->entry.accountRecord.balance -= transData->terminalData.transAmount;
            printf("Updated balance for PAN: %s, New Balance: %.2f\n",
                   current->entry.accountRecord.primaryAccountNumber, current->entry.accountRecord.balance);
            break;
        }
        current = current->next;
    }

    return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t* accountReference) {
    listnode *current = accountList.head;  // Start at the head of the account list

    // Traverse the account list
    while (current != NULL) {
        printf("Checking account: %s against card PAN: %s\n",
               current->entry.accountRecord.primaryAccountNumber,
               cardData->primaryAccountNumber);
        if (strcmp(current->entry.accountRecord.primaryAccountNumber, cardData->primaryAccountNumber) == 0) {
            accountReference->balance = current->entry.accountRecord.balance;
            accountReference->state = current->entry.accountRecord.state;
            strcpy(accountReference->primaryAccountNumber, current->entry.accountRecord.primaryAccountNumber);

            return SERVER_OK;  // Account found
        }
        current = current->next;  // Move to the next node in the list
    }

    printf("Account not found for PAN: %s\n", cardData->primaryAccountNumber);
    return ACCOUNT_NOT_FOUND;  // Account not found
}



EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence) {
    if (accountRefrence->balance >= termData->transAmount) {
        return SERVER_OK; // Sufficient funds available
    } else {
        return LOW_BALANCE; // Insufficient funds
    }
}



  EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence) {
    // Check the state of the account
    if (accountRefrence->state == RUNNING) {
        return SERVER_OK; // Account is running
    } else {
        return BLOCKED_ACCOUNT; // Account is blocked
    }
}


EN_serverError_t saveTransaction(ST_transaction *transData) {
    // Save transaction logic (assumed to be implemented)
    // For example, saving to a file or updating the list of transactions
    FILE *file = fopen("transactions.txt", "a");
    if (file == NULL) {
        return SAVING_FAILED;  // Failed to open file
    }

    // Prepare the transaction data for saving
    fprintf(file, "%u,%s,%s,%f,%s,%d\n",
            transData->transactionSequenceNumber,
            transData->cardHolderData.primaryAccountNumber,
            transData->terminalData.transactionDate,
            transData->terminalData.transAmount,
            transData->terminalData.maxTransAmount,
            transData->transState);

    fclose(file);

    return SERVER_OK;  // Successfully saved transaction
}



void listSavedTransactions(void)
{
    listnode *current = transactionList.head;
     while(current != NULL)
     {
        printf("Transaction Sequence Number: %u\n", current->entry.transactionRecord.transactionSequenceNumber);
        printf("Transaction ID: %u\n", current->entry.transactionRecord.transactionID);
        printf("Transaction Date: %s\n", current->entry.transactionRecord.terminalData.transactionDate);
        printf("Transaction Amount: %.2f\n", current->entry.transactionRecord.terminalData.transAmount);
        printf("Transaction State: %d\n", current->entry.transactionRecord.transactionState); // Assuming transactionState is an enum
        printf("Terminal Max Amount: %.2f\n", current->entry.transactionRecord.terminalData.maxTransAmount);
        printf("Cardholder Name: %s\n", current->entry.transactionRecord.cardHolderData.cardHolderName);
        printf("PAN: %s\n", current->entry.transactionRecord.cardHolderData.primaryAccountNumber);
        printf("Card Expiration Date: %s\n", current->entry.transactionRecord.cardHolderData.cardExpirationDate);
     }
}


