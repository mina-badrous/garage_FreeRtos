/*
 * terminal_user_menu.h
 *
 * Created: 08/06/2018 2:55:04
 *  Author: BADROUS
 */ 
//---------------------------------------------------------------------
#ifndef TERMINAL_USER_MENU_H_
#define TERMINAL_USER_MENU_H_
//-----------------------------------------------------------------
#include "main.h"
#include "terminal_task.h"
//----------------------------------------------------------
void list_users(void);
void add_users(void);
void edit_users(void);
void delete_users(void);
void factory_reset(void);
//----------------------------------------------------------------
#endif /* TERMINAL_USER_MENU_H_ */