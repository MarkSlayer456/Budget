#include <stdio.h>
#include <stdlib.h>

#include "budget.h"

int main() {
	int year = ask_year();
	int month = ask_month();
	printf("Looking at: %d/%d\n", month, year);
	//int add = ask_add();
	info_t *t = get_info(month, year);
	for(int add = ask_add(); ; add = ask_add()) {
		if(add == VIEW_RECEIPTS) {
			cat_spending_print(t);
		} else if(add == ADD_TO_RECEIPTS) {
			char *price = ask_price();
			cats_t cat = ask_cat();
			add_data(t, price, cat);
			save_data(t, month, year);
		} else if(add == REMOVE_FROM_RECEIPTS) {
			//TODO show last three entries with UIDs
			int uid = ask_uid();
			remove_data(t, uid);
			save_data(t, month, year);
		} else if(add == EXIT) {
			printf("Exiting program!\n");
			exit(0);
		} else {
			//TODO
			printf("unknown command!\n");
			exit(1);
		}
	}
	//printf("Thank you for using budget 2.0!\n");
	return 0;
}

