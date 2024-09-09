#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED
#include "Card.h"
#include "terminal.h"
#include "server.h"
#include "File handling.h"
#include "List.h"


/*
 * FILE: Application.h
 * AUTHOR: Yasmine Elmenofy
 * This is the header file of Application module
 *contains the prototypes of the used function in the module
 *Date :22/08/2024
 */

void appStart(void);
void makePayment(void);
void initializeApplication();
void addNewAccount( );
void EditAccountRecords(const char *filename_account);



#endif // APPLICATION_H_INCLUDED
