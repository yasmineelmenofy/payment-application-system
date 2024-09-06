#include <stdio.h>
#include <stddef.h>
#include "File handling.h"
#include "List.h"


/*
 * FILE: File handling.c
 * AUTHOR: Yasmine Elmenofy
 * BRIEF: This section of the project deal with file
 *writing and reading information from and to the files
 * DATA: 31/08/2024
 */

extern List accountList;
extern List transactionList;

/*
* Use this function to add new accounts to both file and the account list  .
* it open the file then add the fields information in the fileds
*/

void DFF_vInsertNewAccount(const char *filename_account, RecordData *new_record) {

    if (insertList(accountList.size, *new_record, &accountList) == -1) {
        printf("Error inserting record into the list.\n");
        return;
    }


    FILE *file = fopen(filename_account, "a");
    if (file != NULL) {
        fprintf(file, "%.2f,%d,%s,%s,%s\n",
                new_record->accountRecord.balance,
                new_record->accountRecord.state,
                new_record->accountRecord.primaryAccountNumber,
                new_record->accountRecord.accountHolderName,
                new_record->accountRecord.expiryDate);
        fclose(file);
    } else {
        printf("Error: Unable to open account information file %s\n", filename_account);
    }


}
/*
 *  Reads account information from a file and inserts it into a list.
 *
 * This function opens the specified file containing account information,
 * reads each record, and inserts it into a global list. It handles errors
 * related to file opening and record insertion.
 */
int DFF_iReadAccountInformationIntoList(const char *filename_account) {
    FILE *file = fopen(filename_account, "r");
    if (file == NULL) {
        printf("Error: Unable to open account information file %s\n", filename_account);
        return -1;
    }

    AccountRecord_t temp_record;
    RecordData recordData;
    int count = 0;
    while (fscanf(file, "%f,%d,%19[^,],%24[^,],%5[^\n]\n",
                  &temp_record.balance,
                  &temp_record.state,
                  temp_record.primaryAccountNumber,
                  temp_record.accountHolderName,
                  temp_record.expiryDate) == 5) {


        recordData.accountRecord = temp_record;
        if (insertList(accountList.size, recordData, &accountList) == -1) {
            printf("Error inserting record into the list.\n");
            fclose(file);
            return -1;
        }
        count++;
    }

    fclose(file);

    printf("Number of accounts read: %d\n", count);
    return count;
}
/*
*This functions insert transaction information into  file and insterts it into a list
*/
void DFF_insertnewtransactiontofile(const char *filename_transaction, RecordData *new_record) {
    FILE *file = fopen(filename_transaction, "a");
    if (file != NULL) {
        fprintf(file, "%u,%s,%.2f,%d,%.2f,%s,%s,%s\n",
                new_record->transactionRecord.transactionSequenceNumber,
                new_record->transactionRecord.terminalData.transactionDate, // Transaction Date
                new_record->transactionRecord.terminalData.transAmount,     // Transaction Amount
                new_record->transactionRecord.transactionState,             // Transaction State
                new_record->transactionRecord.terminalData.maxTransAmount,  // Terminal Max Amount
                new_record->transactionRecord.cardHolderData.cardHolderName, // Cardholder Name
                new_record->transactionRecord.cardHolderData.primaryAccountNumber, // PAN
                new_record->transactionRecord.cardHolderData.cardExpirationDate); // Card Expiration Date

        fclose(file);
    } else {
        printf("Error: Unable to open transaction information file %s\n", filename_transaction);
    }
}


/*
 * Reads transaction data from a file and inserts it into the transaction list.
 * Returns the number of transactions read, or -1 if the file cannot be opened.
 */
int DFF_iReadTransactionInformationIntoList(const char *filename_transaction) {
    FILE *file = fopen(filename_transaction, "r");
    if (file == NULL) {
        printf("Error: Unable to open transaction information file %s\n", filename_transaction);
        return -1;
    }

    initList(&transactionList);
    ST_transactionRecord_t temp_record;
    RecordData recordData;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        int result = sscanf(line, "%u,%10s,%f,%d,%f,%255[^,],%19s,%10s",
                            &temp_record.transactionSequenceNumber,
                            temp_record.terminalData.transactionDate,
                            &temp_record.terminalData.transAmount,
                            (int *)&temp_record.transactionState,
                            &temp_record.terminalData.maxTransAmount,
                            temp_record.cardHolderData.cardHolderName,
                            temp_record.cardHolderData.primaryAccountNumber,
                            temp_record.cardHolderData.cardExpirationDate);


        if (result == 8) {
            recordData.transactionRecord = temp_record;

            insertList(transactionList.size, recordData, &transactionList);
        }
    }

    fclose(file);
    return transactionList.size;
}


/*
 * Updates the balance and details of an account in a file based on the provided record.
 * Uses a temporary file to rewrite and update the account information.
 */
void updateAccountBalance(const char *filename, AccountRecord_t *account) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file.\n");
        return;
    }
    FILE *temp_file = fopen("temp_accounts.txt", "w");
    if (temp_file == NULL) {
        printf("Error: Unable to create temporary file.\n");
        fclose(file);
        return;
    }

    AccountRecord_t temp_record;
    int found = 0;

    while (fscanf(file, "%f,%d,%19[^,],%24[^,],%5[^\n]\n",
                  &temp_record.balance,
                  &temp_record.state,
                  temp_record.primaryAccountNumber,
                  temp_record.accountHolderName,
                  temp_record.expiryDate) == 5) {

        if (strcmp(temp_record.primaryAccountNumber, account->primaryAccountNumber) == 0) {
            temp_record.balance = account->balance;
            temp_record.state = account->state;
            strcpy(temp_record.accountHolderName, account->accountHolderName);
            strcpy(temp_record.expiryDate, account->expiryDate);
            found = 1;
        }

        fprintf(temp_file, "%.2f,%d,%s,%s,%s\n",
                temp_record.balance,
                temp_record.state,
                temp_record.primaryAccountNumber,
                temp_record.accountHolderName,
                temp_record.expiryDate);
    }

    fclose(file);
    fclose(temp_file);

    if (found) {
        remove(filename);
        rename("temp_accounts.txt", filename);
        printf("Account balance updated successfully.\n");
    } else {
        printf("Account not found.\n");
        remove("temp_accounts.txt");
    }
}

/*
 * Allows editing of account information based on the PAN entered by the user.
 * Updates both the linked list and the account file with new values.
 */
void DFF_vEditAccountRecords(const char *filename_account) {
    char targetPAN[20];
    printf("Enter the PAN of the account you want to edit: ");
    scanf("%19s", targetPAN);
    listnode *current = accountList.head;
    AccountRecord_t *foundAccount = NULL;

    while (current != NULL) {
        if (strcmp(current->entry.accountRecord.primaryAccountNumber, targetPAN) == 0) {
            foundAccount = &current->entry.accountRecord;
            break;
        }
        current = current->next;
    }

    if (foundAccount == NULL) {
        printf("No account found with the provided PAN.\n");
        return;
    }
    printf("Current Information:\n");
    printf("Balance: %.2f\n", foundAccount->balance);
    printf("State: %d\n", foundAccount->state);
    printf("Name: %s\n", foundAccount->accountHolderName);
    printf("Expiry Date: %s\n", foundAccount->expiryDate);
    printf("Enter new information (leave unchanged fields empty):\n");

    char newBalance[10], newState[5], newName[25], newExpiryDate[6];
clearInputBuffer();
    printf("New Balance (current %.2f): ", foundAccount->balance);
    float newBalanceValue;
    int balanceChanged = scanf("%f", &newBalanceValue);
    if (balanceChanged == 1) {
        foundAccount->balance = newBalanceValue;
    }

    printf("New State (current %d): ", foundAccount->state);
    scanf("%s", newState);
    if (strlen(newState) > 0) {
        foundAccount->state = atoi(newState);
    }

    printf("New Name (current %s): ", foundAccount->accountHolderName);
    scanf(" %24[^\n]", newName);
    if (strlen(newName) > 0) {
        strncpy(foundAccount->accountHolderName, newName, sizeof(foundAccount->accountHolderName));
    }

    printf("New Expiry Date (current %s): ", foundAccount->expiryDate);
    scanf("%5s", newExpiryDate);
    if (strlen(newExpiryDate) > 0) {
        strncpy(foundAccount->expiryDate, newExpiryDate, sizeof(foundAccount->expiryDate));
    }

    FILE *file = fopen(filename_account, "r");
    FILE *tempFile = fopen("temp_accounts.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Error: Unable to open account information file.\n");
        return;
    }

    AccountRecord_t temp_record;
    while (fscanf(file, "%f,%d,%19[^,],%24[^,],%5[^\n]\n",
                  &temp_record.balance,
                  &temp_record.state,
                  temp_record.primaryAccountNumber,
                  temp_record.accountHolderName,
                  temp_record.expiryDate) == 5) {
        if (strcmp(temp_record.primaryAccountNumber, targetPAN) == 0) {
            fprintf(tempFile, "%.2f,%d,%s,%s,%s\n",
                    foundAccount->balance,
                    foundAccount->state,
                    foundAccount->primaryAccountNumber,
                    foundAccount->accountHolderName,
                    foundAccount->expiryDate);
        } else {

            fprintf(tempFile, "%.2f,%d,%s,%s,%s\n",
                    temp_record.balance,
                    temp_record.state,
                    temp_record.primaryAccountNumber,
                    temp_record.accountHolderName,
                    temp_record.expiryDate);
        }
    }

    fclose(file);
    fclose(tempFile);
    remove(filename_account);
    rename("temp_accounts.txt", filename_account);

    printf("Account updated successfully.\n");
}

/*
 * Prints all accounts in the linked list, displaying details such as PAN, balance, state, and expiry date.
 * Formats the output in a table-like structure for easy reading.
 */
void printAccountList(void) {
    listnode *current = accountList.head;

    if (current == NULL) {
        printf("No accounts found in the list.\n");
        return;
    }

    printf("Account List:\n");
    printf("-----------------------------------------------------\n");
    printf("|   PAN               |   Balance   |   State      |   Name                |   Expiry Date   |\n");
    printf("-----------------------------------------------------\n");
    while (current != NULL) {
        printf("| %19s |   %.2f     |   %d        | %20s | %15s |\n",
               current->entry.accountRecord.primaryAccountNumber,
               current->entry.accountRecord.balance,
               current->entry.accountRecord.state,
               current->entry.accountRecord.accountHolderName,
               current->entry.accountRecord.expiryDate);
        current = current->next;
    }

    printf("-----------------------------------------------------\n");
}
