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
* it open the file then add the  information in the fields
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
 * Reads transaction data from a file and inserts it into the transaction list.
 * Returns the number of transactions read, or -1 if the file cannot be opened.
 */
int DFF_iReadTransactionInformationIntoList(const char *filename_transaction) {
    FILE *file = fopen(filename_transaction, "r");
    if (file == NULL) {
        printf("Error: Unable to open transaction information file %s\n", filename_transaction);
        return -1;
    }

    ST_transactionRecord_t temp_record;
    RecordData recordData;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        int result = sscanf(line, "%u,%10[^,],%f,%d,%f,%255[^,],%19[^,],%10[^\n]",
                            &temp_record.transactionSequenceNumber,
                            temp_record.terminalData.transactionDate,
                            &temp_record.terminalData.transAmount,
                            (int *)&temp_record.transactionState,
                            &temp_record.terminalData.maxTransAmount,
                            temp_record.cardHolderData.cardHolderName,
                            temp_record.cardHolderData.primaryAccountNumber,
                            temp_record.cardHolderData.cardExpirationDate);

        // Check if all expected values are read
        if (result == 8) {
            recordData.transactionRecord = temp_record;

            if (insertList(transactionList.size, recordData, &transactionList) == -1) {
                printf("Error inserting record into the transaction list.\n");
                fclose(file);
                return -1;
            }
        } else {
            printf("Error: Line did not match expected format: %s\n", line);
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
*This function updates the account information in the file by first copying all the existing records
* to a temporary file, replacing the record with the matching PAN with the updated information.
*/

void DFF_vupdateAccountFile(const char* filename_account, AccountRecord_t* foundAccount, const char* targetPAN) {
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
