#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "budget.h"

//TODO make a ask function that takes a string as the question and returns the given int value
// instead of having a bunch of ask functions
int ask_uid() {
	printf("which receipt would you like to remove (uid)\n"); //TODO revise this
	size_t uid_size = 64;
	char *str = (char *) malloc(sizeof(char) * uid_size);
	fgets(str, uid_size, stdin);
	str_remove_newline(str, uid_size);
	int uid = atoi(str);
	return uid;	
}

int ask_month() {
	printf("What month would you like to look at/modify\n");
	size_t month_size = 64;
	char *str = (char *) malloc(sizeof(char) * month_size);
	fgets(str, month_size, stdin);
	str_remove_newline(str, month_size);
	int month = atoi(str);
	return month;	
}

int ask_year() {
	printf("What year would you like to look at/modify\n");
	size_t year_size = 64;
	char *str = (char *) malloc(sizeof(char) * year_size);
	fgets(str, year_size, stdin);
	str_remove_newline(str, year_size);
	int year = atoi(str);
	return year;
}

// 1 adds and 0 prints
int ask_add() {
	printf("Would you like to exit or view, add, or remove receipts!\n");
	size_t add_size = 64;
	char *str = (char *) malloc(sizeof(char) * add_size);
	fgets(str, add_size, stdin);
	str_remove_newline(str, add_size);
	if(strcmp(str, "add") == 0 || strcmp(str, "a") == 0) {
		return ADD_TO_RECEIPTS;
	} else if(strcmp(str, "view") == 0 || strcmp(str, "v") == 0) {
		return VIEW_RECEIPTS;
	} else if(strcmp(str, "remove") == 0 || strcmp(str, "r") == 0) {
		return REMOVE_FROM_RECEIPTS;
	} else if(strcmp(str, "exit") == 0) {
		return EXIT;
	} else {
		fprintf(stderr, "Invalid input!\n");
		return ask_add();	
	}
}

char *ask_price() {
	printf("Price : ");
	size_t price_size = 64;
	char *str = (char *) malloc(sizeof(char) * price_size);
	fgets(str, price_size, stdin);
	str_remove_newline(str, price_size);
	return str;
}

cats_t ask_cat() {
	printf("Category (personal, gas, bills, other, food) : ");
	size_t cat_size = 128;
	char *str = (char *) malloc(sizeof(char) * cat_size);
	fgets(str, cat_size, stdin);
	str_remove_newline(str, cat_size);
	cats_t cat = OTHER;
		
	if(strcmp(str, "personal") == 0) {
		cat = PERSONAL;
	} else if(strcmp(str, "gas") == 0) {
		cat = GAS;
	} else if(strcmp(str, "bills") == 0) {
		cat = BILLS;
	} else if(strcmp(str, "other") == 0) {
		cat = OTHER;
	} else if(strcmp(str, "food") == 0) {
		cat = FOOD;
	}
	
	return cat;
}


info_t *get_info(float month, float year) {
	size_t file_name_size = 1024;
	char *file_name = (char *) malloc(sizeof(char) * file_name_size);
	file_name = strdup(".budget");

	size_t month_size = 32;
	char month_str[month_size];
	gcvt(month, month_size, month_str);

	size_t year_size = 32;
	char year_str[year_size];
	gcvt(year, year_size, year_str);

	strcat(file_name, "-");
	strcat(file_name, month_str);
	strcat(file_name, "-");
	strcat(file_name, year_str);
	strcat(file_name, ".csv");
	
	FILE *f = fopen(file_name, "r");
	info_t *new = (info_t *)malloc(sizeof(info_t));
	new->nodes = (info_node_t **)malloc(sizeof(info_node_t *) * MAX_RECEIPTS);
	//memset(new->nodes, 0, sizeof(info_node_t *) * MAX_RECEIPTS);
	if(f == NULL) {
		return new;
	} else {
		size_t buff = 512;
		char *str = (char *)malloc(sizeof(char) * buff);
		int i = 0;
		while(fgets(str, buff, f) != NULL) {
			if(str[0] == '\n' || str[0] == '\0') return new;
			new->nodes[i] = (info_node_t *) malloc(sizeof(info_node_t));
			str_remove_newline(str, buff);
			char *tok = strtok(str, ",");
			cats_t cat = OTHER;
			
			if(strcmp(tok, "personal") == 0) {
				cat = PERSONAL;
			} else if(strcmp(tok, "gas") == 0) {
				cat = GAS;
			} else if(strcmp(tok, "bills") == 0) {
				cat = BILLS;
			} else if(strcmp(tok, "other") == 0) {
				cat = OTHER;
			} else if(strcmp(tok, "food") == 0) {
				cat = FOOD;
			}

			new->nodes[i]->cat = cat;
			
			tok = strtok(NULL, ",");
			new->nodes[i]->price = strdup(tok);
			
			tok = strtok(NULL, ",");
			new->nodes[i]->uid = atoi(tok);
			i++;
		}
	}
	fclose(f);
	return new;
}

int save_data(info_t *info, float month, float year) {
	size_t file_name_size = 1024;
	char *file_name = (char *) malloc(sizeof(char) * file_name_size);

	file_name = strdup(".budget");

	size_t month_size = 32;
	char month_str[month_size];
	gcvt((float) month, month_size, month_str);

	size_t year_size = 32;
	char year_str[year_size];
	gcvt((float) year, year_size, year_str);

	strcat(file_name, "-");
	strcat(file_name, month_str);
	strcat(file_name, "-");
	strcat(file_name, year_str);
	strcat(file_name, ".csv");
	
	FILE *f = fopen(file_name, "w");
	size_t line_size = 256;
	char *line = (char *) malloc(sizeof(char) * line_size);
	float j = 0;
	for(int i = 0; i < MAX_RECEIPTS; i++) {
		//strcpy(line, ""); // resets line after each loop
		line = strdup(""); // resets line after each loop
		if(info->nodes[i] == NULL) break;
		size_t cat_size = 64;
		char *cat_str = (char *) malloc(sizeof(char) * cat_size);
		cats_t cat = info->nodes[i]->cat;
		switch(cat) {
			case 1: // personal
				strcpy(cat_str, "personal");
				break;
			case 2: // gas
				strcpy(cat_str, "gas");
				break;
			case 3: // bills
				strcpy(cat_str, "bills");
				break;
			case 4: // other
				strcpy(cat_str, "other");
				break;
			case 5: // food
				strcpy(cat_str, "food");
				break;
		}

		size_t price_size = 64;
		char *price_str = (char *) malloc(sizeof(char) * price_size);
		price_str = info->nodes[i]->price;
	//	float price = floor(pow(10, 2)*info->nodes[i]->price)/pow(10, 2);
	//	printf("price = %f\n", price);
		//float price = info->nodes[i]->price;
	//	gcvt(price, price_size, price_str);

		size_t uid_size = 64;
		char *uid_str = (char *) malloc(sizeof(char) * uid_size);
		gcvt((float) j, uid_size, uid_str);

		strcat(cat_str, ",");
		strcat(line, cat_str);
		strcat(line, price_str);
		strcat(line, ",");
		strcat(line, uid_str);
		strcat(line, "\n");

		fputs(line, f);
		j++;
	}
	fclose(f);
	return 0;
}

void add_data(info_t *info, char *price, cats_t cat) {
	int uid = 0;
	for(int i = 0; i < MAX_RECEIPTS; i++) {
		if(info->nodes[i] == NULL) {
			uid = i;
			break;
		}
	}
	info->nodes[uid] = (info_node_t *) malloc(sizeof(info_node_t));
	info->nodes[uid]->uid = uid;
	info->nodes[uid]->price = price;
	info->nodes[uid]->cat = cat;
	printf("Added with UID = %d\n", uid);
	return;
}

void remove_data(info_t *info, int uid) {	
	for(int i = 0; i < MAX_RECEIPTS; i++) {
		if(uid == i) {
			size_t cat_size = 64;
			char cat_str[cat_size]; //TODO make this a function cat_to_str or somthing
			cats_t cat = info->nodes[i]->cat;
			switch(cat) {
				case 1: // personal
					strcpy(cat_str, "personal");
					break;
				case 2: // gas
					strcpy(cat_str, "gas");
					break;
				case 3: // bills
					strcpy(cat_str, "bills");
					break;
				case 4: // other
					strcpy(cat_str, "other");
					break;
				case 5: // food
					strcpy(cat_str, "food");
					break;
			}
			char *price = info->nodes[i]->price;
			int uid = info->nodes[i]->uid;
			printf("Receipt delete: $%s, %s, %d\n", price, cat_str, uid);
			arr_remove((void **) info->nodes, i, MAX_RECEIPTS);
			return;	
		}
	}
	return;
}

void arr_remove(void **arr, int j, size_t size) {
	for(int i = j; i < size; i++) {
		arr[i] = arr[i + 1];
		if(arr[i] == NULL) break;
	}
}


void cat_spending_print(info_t *info) {
	float personal = 0.0;
	float gas = 0.0;
	float bills = 0.0;
	float other = 0.0;
	float food = 0.0;
	if(info->nodes == NULL) {
		printf("No receipts found!");	
	}
	for(int i = 0; i < MAX_RECEIPTS; i++) {
		if(info->nodes[i] == NULL) break;
		cats_t cat = info->nodes[i]->cat;
		switch(cat) {
			case 1: // personal
				personal += atof(info->nodes[i]->price);
				break;
			case 2: // gas
				gas += atof(info->nodes[i]->price);
				break;
			case 3: // bills
				bills += atof(info->nodes[i]->price);
				break;
			case 4: // other
				other += atof(info->nodes[i]->price);
				break;
			case 5: // food
				food += atof(info->nodes[i]->price);
				break;
		}
	}
	printf("Personal : %.2f\n", personal);
	printf("Gas : %.2f\n", gas);
	printf("Bills : %.2f\n", bills);
	printf("Other : %.2f\n", other);
	printf("Food : %.2f\n", food);
	printf("Total : %.2f\n", total_spending(info));
}


float total_spending(info_t *info) {
	float total = 0.0;
	if(info->nodes == NULL) return total;
	for(int i = 0; i < MAX_RECEIPTS; i++) {
		if(info->nodes[i] == NULL) break;
		total += atof(info->nodes[i]->price);
	}
	return total;
}


void str_remove_newline(char *str, size_t size) {
	for(size_t i = 0; i < size; i++) {
		if(str[i] == '\n') {
			str[i] = '\0';
			return;
		}
	}
}

