#include <stddef.h>
#ifndef BUDGET_H_

#define BUDGET_H_

#define MAX_RECEIPTS		1024

#define ADD_TO_RECEIPTS				5000
#define REMOVE_FROM_RECEIPTS		5001
#define VIEW_RECEIPTS				5002
#define EXIT						6000

typedef enum {
	PERSONAL = 1,
	GAS = 2,
	BILLS = 3,
	OTHER = 4,
	FOOD = 5
} cats_t;

typedef struct {
	int uid;
	char *price;
	cats_t cat;
} info_node_t;

typedef struct {
	info_node_t **nodes;
	int total;
	int curr_uid;
} info_t;

int ask_month();
int ask_year();
int ask_add(); // 0 means they print 1 means they add

int ask_uid();

int ask_for_int(const char *question); //TODO implement

char *ask_price();
cats_t ask_cat();

info_t *get_info(float month, float year);

int save_data(info_t *info, float month, float year);
void add_data(info_t *info, char *price, cats_t cat);

void remove_data(info_t *info, int uid);
void arr_remove(void **arr, int j, size_t size);

float total_spending(info_t *info);
void cat_spending_print(info_t *info);


void str_remove_newline(char *str, size_t size);

#endif
