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
    float balance;
    EN_accountState_t state;
    uint8_t primaryAccountNumber[20];
    uint8_t accountHolderName[25];
    uint8_t expiryDate[6];
} AccountRecord_t;

typedef struct ST_transactionRecord_t
{
    uint32_t transactionID;
    ST_cardData_t cardHolderData;
    ST_terminalData_t terminalData;
    uint32_t transactionSequenceNumber;
    EN_transState_t transactionState;
} ST_transactionRecord_t;

typedef union RecordData {
    AccountRecord_t accountRecord;
    ST_transactionRecord_t transactionRecord;
} RecordData;

void DFF_vInsertNewAccount(const char *filename_account, RecordData *new_record);
int DFF_iReadAccountInformationIntoList(const char *filename_account);
void DFF_insertnewtransactiontofile(const char *filename_transaction, RecordData *new_record);
int DFF_iReadTransactionInformationIntoList(const char *filename_transaction);
void updateAccountBalance(const char *filename, AccountRecord_t *account);
void DFF_vEditAccountRecords(const char *filename_account);
void printAccountList(void);

#endif // FILE_HANDLING_H_INCLUDED
