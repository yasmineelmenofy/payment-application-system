#include <stdio.h>
#include <stddef.h>
#include "File handling.h"
#include "List.h"

extern List accountList;
extern List transactionList;



void DFF_vInsertNewAccount(const char *filename_account, RecordData *new_record) {
    printf("Debug: Starting DFF_vInsertNewAccount function.\n");

    // Insert the new record into the account list
    printf("Debug: Attempting to insert into list.\n");
    if (insertList(accountList.size, *new_record, &accountList) == -1) {
        printf("Error inserting record into the list.\n");
        return;
    }
    printf("Debug: Successfully inserted into list.\n");

    // Open the file to append the new record
    printf("Debug: Attempting to open file: %s\n", filename_account);
    FILE *file = fopen(filename_account, "a");
    if (file != NULL) {
        // Debugging: Print what will be written to the file
        printf("Debug: Writing to file: %.2f,%d,%s,%s,%s\n",
               new_record->accountRecord.balance,
               new_record->accountRecord.state,
               new_record->accountRecord.primaryAccountNumber,
               new_record->accountRecord.accountHolderName,
               new_record->accountRecord.expiryDate);

        fprintf(file, "%.2f,%d,%s,%s,%s\n",
                new_record->accountRecord.balance,
                new_record->accountRecord.state,
                new_record->accountRecord.primaryAccountNumber,
                new_record->accountRecord.accountHolderName,
                new_record->accountRecord.expiryDate);
        fclose(file);
        printf("Debug: Successfully wrote to file and closed file.\n");
    } else {
        printf("Error: Unable to open account information file %s\n", filename_account);
    }

    printf("Debug: Finished DFF_vInsertNewAccount function.\n");
}

int DFF_iReadAccountInformationIntoList(const char *filename_account) {
    FILE *file = fopen(filename_account, "r");
    if (file == NULL) {
        printf("Error: Unable to open account information file %s\n", filename_account);
        return -1;
    }

    initList(&accountList); // Initialize the account list
    AccountRecord_t temp_record;
    RecordData recordData;

    while (fscanf(file, "%f,%d,%19[^,],%24[^,],%19[^,],%5[^\n]\n",
                  &temp_record.balance,
                  &temp_record.state,
                  temp_record.primaryAccountNumber,
                  temp_record.accountHolderName,
                  temp_record.expiryDate) == 5) {

        recordData.accountRecord = temp_record; // Wrap the account record in the union
        insertList(accountList.size, recordData, &accountList); // Insert the union into the account list
    }

    fclose(file);
    return accountList.size;  // Return the number of records read
}

void DFF_insertnewtransactiontofile(const char *filename_transaction, RecordData *new_record) {
    // Insert the new record into the transaction list
    insertList(transactionList.size, *new_record, &transactionList);

    // Append the new record to the file
    FILE *file = fopen(filename_transaction, "a");
    if (file != NULL) {
        fprintf(file, "%u,%s,%s,%f,%s,%d\n",
                new_record->transactionRecord.transactionID,
                new_record->transactionRecord.cardHolderData.primaryAccountNumber,
                new_record->transactionRecord.terminalData.transactionDate,
                new_record->transactionRecord.terminalData.transAmount,
                new_record->transactionRecord.terminalData.maxTransAmount,
                new_record->transactionRecord.transactionSequenceNumber,
                new_record->transactionRecord.transactionState);
        fclose(file);
    } else {
        printf("Error: Unable to open transaction information file %s\n", filename_transaction);
    }
}

int DFF_iReadTransactionInformationIntoList(const char *filename_transaction) {
    FILE *file = fopen(filename_transaction, "r");
    if (file == NULL) {
        printf("Error: Unable to open transaction information file %s\n", filename_transaction);
        return -1;
    }

    initList(&transactionList); // Initialize the transaction list
    ST_transactionRecord_t temp_record;
    RecordData recordData;

    while (fscanf(file, "%u,%19[^,],%10[^,],%f,%f,%d\n",
              &temp_record.transactionID,
              temp_record.cardHolderData.primaryAccountNumber,
              temp_record.terminalData.transactionDate,
              &temp_record.terminalData.maxTransAmount,
              &temp_record.terminalData.transAmount,     // Add the & to pass the address
              (int *)&temp_record.transactionState) == 6) {  // Cast to int* to read an enum
    // Processing logic here

        recordData.transactionRecord = temp_record; // Wrap the transaction record in the union
        insertList(transactionList.size, recordData, &transactionList); // Insert the union into the transaction list
    }

    fclose(file);
    return transactionList.size;  // Return the number of records read
}
