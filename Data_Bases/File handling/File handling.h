#ifndef FILE_HANDLING_H_INCLUDED
#define FILE_HANDLING_H_INCLUDED
#include <stddef.h>
#include <stdint.h>
#include "Card.h"
#include "Terminal.h"



#define MAX_RECORDS 100



typedef enum EN_accountState_t
{
    RUNNING,
    BLOCKED
} EN_accountState_t;

typedef enum EN_transState_t
{
APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD,
FRAUD_CARD, INTERNAL_SERVER_ERROR
}EN_transState_t;

typedef struct AccountRecord_t
{
    float balance;                             // Current account balance
    EN_accountState_t state;                  // Account state (RUNNING or BLOCKED)
    uint8_t primaryAccountNumber[20];         // Primary Account Number (PAN)
    uint8_t accountHolderName[25];            // Name of the account holder
    uint8_t expiryDate[6];                    // Expiry date of the card (MM/YY)
} AccountRecord_t;

typedef struct ST_transactionRecord_t
{
    uint32_t transactionID;                   // Unique identifier for the transaction
    ST_cardData_t cardHolderData;             // Cardholder information
    ST_terminalData_t terminalData;           // Terminal transaction data
    uint32_t transactionSequenceNumber;             // Date of the transaction (DD/MM/YYYY)
    EN_transState_t transactionState;         // State of the transaction (Approved, Declined, etc.)
} ST_transactionRecord_t;

typedef union RecordData {
    AccountRecord_t accountRecord;
    ST_transactionRecord_t transactionRecord;
} RecordData;

void DFF_vInsertNewAccount(const char *filename_account, RecordData *new_record);
int DFF_iReadAccountInformationIntoList(const char *filename_account);
void DFF_insertnewtransactiontofile(const char *filename_transaction, RecordData *new_record);
int DFF_iReadTransactionInformationIntoList(const char *filename_transaction);


#endif // FILE_HANDLING_H_INCLUDED
