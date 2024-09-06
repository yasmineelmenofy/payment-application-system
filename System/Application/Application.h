#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED
#include "Card.h"
#include "terminal.h"
#include "server.h"
#include "File handling.h"
#include "List.h"

void appStart(void);
void makePayment(void);
void viewTransactionHistory(void);
void manageCardInformation(void);
void exportTransactionHistory(void);

#endif // APPLICATION_H_INCLUDED
