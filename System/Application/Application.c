
#include "stdio.h"
#include "Application.h"

/*
 * FILE: application.c
 * AUTHOR: Yasmine Elmenofy
 * BRIEF: This section of the project deal with interaction with the user on
 *starting the program the user will have menu with option make payment or add
 *account or edit account or view transaction history
 * DATA: 3/09/2024
 */



void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
* Use this function to initialize both list of accounts and transactions .
* the reading the information from the both files to the lists
*/

void initializeApplication() {
    printf("Initializing account list...\n");
    initList(&accountList);

    printf("Initializing transaction list...\n");
    initList(&transactionList);

    printf("Reading account information from file...\n");
    if (DFF_iReadAccountInformationIntoList("accounts.txt") == 0) {
        printf("Error: Failed to read account information.\n");
    }

    printf("Reading transaction information from file...\n");
    if (DFF_iReadTransactionInformationIntoList("transactions.txt") == 0) {
        printf("Error: Failed to read transaction information.\n");
    }

    printf("Initialization complete.\n");
}


/*
* Use this function to add new accounts to both file and the account list  .
* it ask the user for name , expairation date and the PAN and check the information by
*calling the functions from card module then copying data from card data to record data
*then calling insert new account to add the account as a new record to both file and list
*/
void addNewAccount( ) {
    const char *filename_account = "accounts.txt";
    EN_cardError_t cardStatus;
    ST_cardData_t cardData;
    RecordData newRecord;
    clearInputBuffer();
    cardStatus = getCardHolderName(&cardData);
    if (cardStatus != CARD_OK) {
        printf("Invalid cardholder name.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    cardStatus = getCardExpiryDate(&cardData);
    if (cardStatus != CARD_OK) {
        printf("Invalid expiry date.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    cardStatus = getCardPAN(&cardData);
    if (cardStatus != CARD_OK) {
        printf("Invalid PAN.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    strcpy(newRecord.accountRecord.accountHolderName, cardData.cardHolderName);
    strcpy(newRecord.accountRecord.primaryAccountNumber, cardData.primaryAccountNumber);
    strcpy(newRecord.accountRecord.expiryDate, cardData.cardExpirationDate);
    newRecord.accountRecord.balance = 0.0; // Initial balance
    newRecord.accountRecord.state = RUNNING; // Initial state
    printf("Adding new account:\n");
    printf("Account Holder Name: %s\n", newRecord.accountRecord.accountHolderName);
    printf("PAN: %s\n", newRecord.accountRecord.primaryAccountNumber);
    printf("Expiry Date: %s\n", newRecord.accountRecord.expiryDate);
    DFF_vInsertNewAccount(filename_account, &newRecord);

    printf("Account created successfully!\n");
}

/*
* Use this function to  make payment process   .
* Start → Get cardholder name → Get card expiration date → Get card PAN →
*Get transaction date → Is card expired? (Yes: Declined Expired Card → No) →
* Get transaction amount → Is exceeded amount? (Yes: Declined amount exceeding limit → No) → Is valid account?
(No: Declined invalid account → Yes) → Is amount available?
*(No: Declined insufficient funds → Yes) → Update account balance → Save transaction → End.
*/
void makePayment(void) {
    EN_cardError_t cardStatus;
    ST_cardData_t cardData;
    ST_terminalData_t termData;
    EN_terminalError_t termStatus;

    clearInputBuffer();
    cardStatus = getCardHolderName(&cardData);
    if (cardStatus != CARD_OK) {
        printf("Invalid cardholder name.\n");
        clearInputBuffer();
        return;
    }

    clearInputBuffer();
    cardStatus = getCardExpiryDate(&cardData);
    if (cardStatus != CARD_OK) {
        printf("Invalid expiry date.\n");
        clearInputBuffer();
        return;
    }

    clearInputBuffer();
    cardStatus = getCardPAN(&cardData);
    if (cardStatus != CARD_OK) {
        printf("Invalid PAN.\n");
        clearInputBuffer();
        return;
    }

    clearInputBuffer();
    termStatus = getTransactionDate(&termData);
    if (termStatus != TERMINAL_OK) {
        printf("Invalid transaction date.\n");
        clearInputBuffer();
        return;
    }

    termStatus = isCardExpired(&cardData, &termData);
    if (termStatus == EXPIRED_CARD) {
        printf("Transaction declined: Card is expired.\n");
        return;
    }

    clearInputBuffer();
    termStatus = getTransactionAmount(&termData);
    if (termStatus != TERMINAL_OK) {
        printf("Invalid transaction amount.\n");
        return;
    }

    clearInputBuffer();
    termStatus = setMaxAmount(&termData, 900.00);
    if (termStatus != TERMINAL_OK) {
        printf("Invalid max transaction amount.\n");
        return;
    }

    termStatus = isBelowMaxAmount(&termData);
    if (termStatus == EXCEED_MAX_AMOUNT) {
        printf("Transaction declined: Amount exceeds the allowed limit.\n");
        return;
    }

    clearInputBuffer();
    ST_transaction transData = {0};
    transData.cardHolderData = cardData;
    transData.terminalData = termData;

    // Call receiveTransactionData and print account details until found
    EN_transState_t transStatus = receiveTransactionData(&transData);

    if (transStatus == APPROVED) {
        printf("Payment successful!\n");

        // Update the balance in the account list
        listnode *current = accountList.head;
        while (current != NULL) {
            if (strcmp(current->entry.accountRecord.primaryAccountNumber,
                       transData.cardHolderData.primaryAccountNumber) == 0) {
                current->entry.accountRecord.balance -= transData.terminalData.transAmount;
                printf("Updated balance for PAN: %s, New Balance: %.2f\n",
                       current->entry.accountRecord.primaryAccountNumber, current->entry.accountRecord.balance);
                break;
            }
            current = current->next;
        }




    } else {
        // Print transaction declined message based on transStatus
        printf("Transaction declined: ");
        switch (transStatus) {
            case FRAUD_CARD:
                printf("Fraud card.\n");
                break;
            case DECLINED_STOLEN_CARD:
                printf("Stolen card.\n");
                break;
            case DECLINED_INSUFFECIENT_FUND:
                printf("Insufficient funds.\n");
                break;
            case INTERNAL_SERVER_ERROR:
                printf("Internal server error.\n");
                break;
            default:
                printf("Unknown error.\n");
        }
    }
}





void appStart(void) {
    int choice;
    do {
        printf("\nChoose an option:\n");
        printf("1. Make a payment\n");
        printf("2. View transactions history\n");
        printf("3. Add new account\n");
        printf("4. update the account information\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                makePayment();
                break;
            case 2:
                listSavedTransactions();
                break;
            case 3:
                addNewAccount();
                break;
            case 4:
                DFF_vEditAccountRecords("accounts.txt");
                break;
            case 5:
                printf("Existing the application");
            default:
                printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }
    } while (choice != 5);
}

int main(void) {
    // Initialize the application
    initializeApplication();

    // Print account list (ensure this is valid and the account list is populated)
    printAccountList();

    // Start the application
    appStart();

    return 0;
}
