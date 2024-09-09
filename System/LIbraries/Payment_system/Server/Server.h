#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED
#include "File handling.h"
#include "List.h"

/*
 * FILE: server.h
 * AUTHOR: Yasmine Elmenofy
 * BRIEF: This is the header file of terminal module
 * it includes the data structure of transactions and account database
 *it includes the prototypes of the used functions
 * DATA: 27/08/2024
 */

List accountList;
List transactionList;
typedef struct ST_transaction_t
{
ST_cardData_t cardHolderData;
ST_terminalData_t terminalData;
EN_transState_t transState;
uint32_t transactionSequenceNumber;
}ST_transaction;
typedef enum EN_serverError_t
{
SERVER_OK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND,
LOW_BALANCE, BLOCKED_ACCOUNT
}EN_serverError_t ;

typedef struct ST_accountsDB_t
{
float balance;
EN_accountState_t state;
uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;


EN_transState_t receiveTransactionData(ST_transaction *transData);
EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t*
accountRefrence);
EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence);
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData,
ST_accountsDB_t* accountRefrence);
EN_serverError_t saveTransaction(ST_transaction *transData);
void listSavedTransactions(void);


#endif // SERVER_H_INCLUDED
