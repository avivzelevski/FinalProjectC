#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

/* User data Structure */
typedef struct user_data
{
	char name[21];
	char code[9];
	char date_s[11];
	char date_e[11];
	char time_s[6];
	char time_e[6];
	int status;
}User;

/* Entrance request Structure */
typedef struct user_request
{
	char name_code[21];
	char date[11];
	char time[6];
	char status[22];
	int door;
}Request;

/* Node Structure */
typedef struct node
{
	User data;
	Request rqst_data;
	struct node *next;
	struct node *prev;
}Node;

/* List Structure */
typedef struct list
{
	Node *head;
	Node *tail;
}list;

/** Function declerations **/
/* Print all the elements in the linked list by a specific given file path */
void print(char *path, list *lst);
/* Search a specific user(s) by name, code or status */
list *search(list *lst, char *name, int status, char *code);

/* Initiate list head & tail to NULL */
void init_list(list *lst)
{
	lst->head = lst->tail = NULL;
}

/* Check if the list is empty */
int isEmpty(list *lst)
{
	return lst->head == NULL;
}

/* Check the length of the link */
int length(list *lst)
{
	int length = 0;
	Node *current;

	for (current = lst->head; current != NULL; current = current->next)
		length++;

	return length;
}

/* Insert link at the first location */
void insertFirst(list *lst, User data)
{
	//create a new link
	Node *link = (Node*)malloc(sizeof(Node));
	link->data = data;
	link->next = NULL;
	link->prev = NULL;

	if (isEmpty(lst))
	{
		//make head & tail pointing to link
		lst->head = lst->tail = link;
	}
	else
	{
		//update first prev link
		lst->head->prev = link;
		//point it to old first link
		link->next = lst->head;
		//point first to new first link
		lst->head = link;
	}
}

/* Insert link at the last location */
void insertLast(list *lst, User data)
{
	//create a new link
	Node *link = (Node*)malloc(sizeof(Node));
	link->data = data;
	link->next = NULL;
	link->prev = NULL;

	if (isEmpty(lst))
	{
		//make head & tail pointing to link
		lst->head = lst->tail = link;
	}
	else
	{
		//make link a new last link
		lst->tail->next = link;
		//mark old last node as prev of new link
		link->prev = lst->tail;
		//point last to new last node
		lst->tail = link;
	}
}

/* Insert link at the last location of request_list */
void insertLastLog(list *lst, Request data)
{
	//create a new link
	Node *link = (Node*)malloc(sizeof(Node));

	link->rqst_data = data;
	link->next = NULL;
	link->prev = NULL;

	if (isEmpty(lst))
	{
		//make head & tail pointing to link
		lst->head = lst->tail = link;
	}
	else
	{
		//make link a new last link
		lst->tail->next = link;
		//mark old last node as prev of new link
		link->prev = lst->tail;
		//point last to new last node
		lst->tail = link;
	}
}

/* delete first item */
int deleteFirst(list *lst)
{
	//if list is empty there is nothing to delete
	if (!isEmpty(lst))
	{
		//save reference to first link
		Node *temp = lst->head;
		//if only one link
		if (temp->next == NULL)
		{
			init_list(lst);
		}
		else
		{
			lst->head->next->prev = NULL;
			lst->head = lst->head->next;
		}
		free(temp);
		return 1;
	}
	return 0;
}

/* delete link at the last location */
int deleteLast(list *lst)
{
	//if list is empty there is nothing to delete
	if (!isEmpty(lst))
	{
		//save reference to last link
		Node *temp = lst->tail;
		//if only one link
		if (temp->prev == NULL)
		{
			init_list(lst);
		}
		else
		{
			lst->tail->prev->next = NULL;
			lst->tail = lst->tail->prev;
		}
		free(temp);
		return 1;
	}
	return 0;
}

/* delete a link at spesific index */
int deleteLink(list *lst, int idx)
{
	if (!isEmpty(lst))
	{
		int i;
		//start from the first link
		Node* current = lst->head;
		//navigate through list
		for (i = 0; i < idx && current != NULL; i++)
			current = current->next;
		if (current == NULL)
			return 0;
		if (current == lst->head)
			return deleteFirst(lst);
		if (current == lst->tail)
			return deleteLast(lst);
		//bypass the current link
		current->prev->next = current->next;
		current->next->prev = current->prev;
		free(current);
		return 1;
	}
	return 0;
}

/* Insert a link after a spesific index */
int insertAfter(list *lst, User data, int idx)
{
	if (!isEmpty(lst))
	{
		int i;
		// Start from the first link
		Node* current = lst->head;
		Node *newLink = NULL;
		// Navigate through list
		for (i = 0; i < idx && current != NULL; i++)
			current = current->next;
		if (current == NULL)
			return 0;
		if (current == lst->tail)
		{
			insertLast(lst, data, NULL);
			return 1;
		}
		// Create a link
		newLink = (Node*)malloc(sizeof(Node));
		newLink->data = data;
		// Assign new link pointers
		newLink->prev = current;
		newLink->next = current->next;
		// Chain current and current->next to new link
		current->next->prev = newLink;
		current->next = newLink;

		return 1;
	}
	return 0;
}

/* Insert a link before a spesific index */
int insertBefore(list *lst, User data, int idx)
{
	if (!isEmpty(lst))
	{
		int i;
		// Start from the first link
		Node* current = lst->head;
		Node* newLink = NULL;
		// Navigate through list
		for (i = 0; i < idx && current != NULL; i++)
			current = current->next;
		if (current == NULL)
			return 0;
		if (current == lst->head)
		{
			insertFirst(lst, data);
			return 1;
		}
		// Create a link
		newLink = (Node*)malloc(sizeof(Node));
		newLink->data = data;
		// Assign new link pointers
		newLink->next = current;
		newLink->prev = current->prev;
		// Chain current and current->prev to new link
		current->prev->next = newLink;
		current->prev = newLink;

		return 1;
	}
	return 0;
}

/* Write list to a file by a specific path */
void writeToFile(char *path, list *lst) {
	Node *current_node = lst->head;
	FILE *fp = fopen(path, "w");
	char* headers = "Name                 Code     S Start date End date   Stime Etime\r\n";

	// Write the headers line to the file
	fputs(headers, fp); // Write the headers line to the file

	// Write each node (user) in the linked list to the file
	while (current_node != NULL) {
		fprintf(fp, "%-20s %-8s %-1d %-10s %-10s %-5s %-5s\n",
			current_node->data.name,
			current_node->data.code,
			current_node->data.status,
			current_node->data.date_s,
			current_node->data.date_e,
			current_node->data.time_s,
			current_node->data.time_e);
		current_node = current_node->next;
	}
	// Close fp file
	fclose(fp);
}

// Write each node (user) in the linked list to the  log file
void writeToLogFile(char *path, list *lst, char date[10], char time[5]) {
	Node *current_node = lst->head;
	FILE *fp = fopen(path, "w");
	char* headers = "Name/Code             door Status                date       time\r\n";

	// Write the headers line to the file
	fputs(headers, fp);

	// Write each node (user) in the linked list to the file
	while (current_node != NULL) {
		strcpy(current_node->rqst_data.date, date);
		strcpy(current_node->rqst_data.time, time);

		fprintf(fp, "%-20s %-4d %-21s %-11s %-5s\n",
			current_node->rqst_data.name_code,
			current_node->rqst_data.door,
			current_node->rqst_data.status,
			current_node->rqst_data.date,
			current_node->rqst_data.time
		);
		current_node = current_node->next;
	}
	// Close fp file
	fclose(fp);
}

/* Print all the elements in the linked list by a specific given file path */
void print(char *path, list *lst) { //Node *head) {
	Node *current_node = lst->head;
	char* headers;

	// Set header line according to the specific file path
	if (strcmp(path, "access.txt") == 0) {
		//Print headers line to the console
		headers = "Name                 Code     S Start date End date   Stime Etime\n";
		printf("\nThe current users list:\n%s", headers); 
	}
	else if (strcmp(path, "log.txt") == 0) {
		//Print headers line to the console
		headers = "Name / Code         door  Status                date        time\n";
		printf("\nThe entrance attempts list:\n%s", headers); 
	}

	// Print the relevent list to the console by path
	while (current_node != NULL) {
		if (strcmp(path, "access.txt") == 0) {
			printf("%-20s %-8s %-1d %-10s %-10s %-5s %-5s\n",
				current_node->data.name,
				current_node->data.code,
				current_node->data.status,
				current_node->data.date_s,
				current_node->data.date_e,
				current_node->data.time_s,
				current_node->data.time_e);
		}
		else if (strcmp(path, "requests.txt") == 0)
			printf("%-1d,%-8s\n", current_node->rqst_data.door, current_node->rqst_data.name_code);

		else if (strcmp(path, "log.txt") == 0) {
			printf("%-20s %-4d %-21s %-11s %-5s\n",
				current_node->rqst_data.name_code,
				current_node->rqst_data.door,
				current_node->rqst_data.status,
				current_node->rqst_data.date,
				current_node->rqst_data.time
			);
		}
		current_node = current_node->next;
	}
}

/* Read data from request.txt file to a linked list */
void readRequsts(char *path, list *lst) {
	FILE *fp1;
	char temp[12], code[21];
	int door;
	Request r;

	// Reads all the data from the file and put it in fp
	fp1 = fopen(path, "rb"); //Reads all the data from the file and put it in fp
	if (!fp1)
	{
		printf("File not found!\n");
		return;
	}

	// Assign door and code parameters from the request.txt file to their variables
	while (fscanf(fp1, "%1d,%20s", &door, code) != EOF) {
		strcpy(r.name_code, code);
		r.door = door;

		insertLastLog(lst, r);
	}
	// Close fp file
	fclose(fp1);
}

/* Read data from access.txt file to a linked list */
void readAccess(char *path, list *lst) // Node *head) // 
{
	FILE *fp;
	char temp[70], name[21], code[9], date_s[11], date_e[11], time_s[6], time_e[6];
	int status;
	User d;

	//Reads all the data from the file and put it in fp
	fp = fopen(path, "r"); 
	if (!fp)
	{
		printf("File not found!\n");
		return;
	}

	//Cuts off the first line (headlines) from fp
	fgets(temp, 70, fp); 

	// Assign name, code, status, date_s, date_e, start_s, start_e parameters from the request.txt file to their variables
	while (fscanf(fp, "%20s %8s %1d %10s %10s %5s %5s", name, code, &status, date_s, date_e, time_s, time_e) != EOF)
	{
		strcpy(d.name, name);
		strcpy(d.code, code);
		d.status = status;
		strcpy(d.date_s, date_s);
		strcpy(d.date_e, date_e);
		strcpy(d.time_s, time_s);
		strcpy(d.time_e, time_e);

		insertLast(lst, d);

	}
	// Close fp file
	fclose(fp);
}

/* Add a new user to the users' list */
void AddUser(list *new)
{
	int status, count, check;
	User data;
	Node *newuser = (Node*)malloc(sizeof(Node));
	Node *cmpuser = new->head;
	list *some_list = (list*)malloc(sizeof(list));
	init_list(some_list);
	

	// Get the new user's parameters
	printf("Enter user name: ");
	scanf("%s", newuser->data.name);
	printf("Enter user code: ");
	scanf("%s", newuser->data.code);

	// Check if the CODE is already exist in the access list
	some_list = search(new, "", 0, newuser->data.code);
	if (some_list != NULL) {
		printf("ERROR: Code already in use, please choose different code: ");
		scanf("%s", newuser->data.code);
	}

	printf("Enter user entrance status: ");
	scanf("%d", &status);
	newuser->data.status = status;
	printf("Enter user permission start date: ");
	scanf("%s", newuser->data.date_s);
	printf("Enter user permission end date: ");
	scanf("%s", newuser->data.date_e);
	printf("Enter user permission start time: ");
	scanf("%s", newuser->data.time_s);
	printf("Enter user permission start time: ");
	scanf("%s", newuser->data.time_e);

	// Insert the new user into the list and keep the list sorted 
	count = check = 0;
	while (cmpuser != NULL)
	{
		if (strcmp(cmpuser->data.code, newuser->data.code) > 0) {
			insertBefore(new, newuser->data, count);
			check = 1;
			break;
		}
		count++;
		cmpuser = cmpuser->next;
	}
	if (check == 0)
		insertLast(new, newuser->data);

	free(newuser);
}

/* Search a specific user(s) by name, code or status */
list *search(list *lst, char *name, int status, char *code) {
	Node *current_node = lst->head;
	list *rslt_list = (list *)malloc(sizeof(list));
	init_list(rslt_list);
	int check = 0;

	// Check if the list is empty
	if (isEmpty(lst)) {
		return;
	}

	// Go through the list to find the specific user(s) and save it/them to a new list
	while (current_node != NULL) {
		if (strcmp(current_node->data.name, name) == 0 || current_node->data.status == status || strcmp(current_node->data.code, code) == 0) {
			insertLast(rslt_list, current_node->data);
			check++;
		}

		current_node = current_node->next;
	}
	if (check == 0) {
		return rslt_list = NULL;
	}
	return rslt_list;
}

/* Update user permissions */
void updateUser(char *path, list *accs_lst, list *srch_list, char *name, int status, char *code) {
	int count, check;
	char date_s[11], date_e[11], time_s[6], time_e[6];
	Node *updt_node;
	Node *cmpuser = accs_lst->head;

	updt_node = srch_list->head;

	// Search user to update by its NAME
	if (strcmp(name, "") != 0) {
		// Update user's TIME-RANGE permissions
		if (status == 0) {
			printf("Enter new START and END date, START and END time (dd/mm/yyyy dd/mm/yyyy hh:mm hh:mm) for user %s: ", name);
			scanf("%s %s %s %s", updt_node->data.date_s, updt_node->data.date_e, updt_node->data.time_s, updt_node->data.time_e);
			printf("User details after the changes:\n %-20s %-8s %-1d %-10s %-10s %-5s %-5s\n",
				updt_node->data.name,
				updt_node->data.code,
				updt_node->data.status,
				updt_node->data.date_s,
				updt_node->data.date_e,
				updt_node->data.time_s,
				updt_node->data.time_e);
		}
		// Update user's permissions STATUS
		else {
			updt_node->data.status = status;
		}
	}

	// Search user to update by its CODE
	else {
		// Update user's TIME-RANGE permissions
		if (status == 0) {
			printf("Enter new START and END date, START and END time (dd/mm/yyyy hh:mm) for user %s: ", name);
			scanf("%s %s %s %s", updt_node->data.date_s, updt_node->data.date_e, updt_node->data.time_s, updt_node->data.time_e);
			printf("User details after the changes:\n %-20s %-8s %-1d %-10s %-10s %-5s %-5s\n",
				updt_node->data.name,
				updt_node->data.code,
				updt_node->data.status,
				updt_node->data.date_s,
				updt_node->data.date_e,
				updt_node->data.time_s,
				updt_node->data.time_e);
		}

		// Update user's permissions STATUS
		else {
			updt_node->data.status = status;
		}
	}

	// Insert the updated user to the linked list instead of the old one and write the new list the access.txt
	count = check = 0;
	while (cmpuser != NULL)
	{
		if (strcmp(cmpuser->data.code, updt_node->data.code) == 0) {
			insertBefore(accs_lst, updt_node->data, count);
			deleteLink(accs_lst, count + 1);
			check = 1;
			break;
		}
		count++;
		cmpuser = cmpuser->next;
	}
	if (check == 0)
		insertLast(accs_lst, updt_node->data);

	// Write the access list with the updated user to the access.txt file
	writeToFile(path, accs_lst);
	free(srch_list);
}

/* Get the current date and time */
void getDateTime(char *day, char *month, char *year, char *hours, char *mins)
{
	time_t rawtime;
	struct tm *timeinfo;
	int day_n, month_n, year_n, hours_n, mins_n;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	// Store struct tm parameters as integers
	day_n = timeinfo->tm_mday;
	month_n = timeinfo->tm_mon + 1;
	year_n = timeinfo->tm_year + 1900;
	hours_n = timeinfo->tm_hour;
	mins_n = timeinfo->tm_min;

	// Convert integer parameters to strings
	sprintf(day, "%d", day_n);
	sprintf(month, "%d", month_n);
	sprintf(year, "%d", year_n);
	sprintf(hours, "%d", hours_n);
	sprintf(mins, "%d", mins_n);

	// Change one digit value parameter to two digits (2->02)
	if (day_n < 10) {
		sprintf(day, "0%d", day_n);
	}
	if (month_n < 10) {
		sprintf(month, "0%d", month_n);
	}
	if (hours_n < 10) {
		sprintf(hours, "0%d", hours_n);
	}
	if (mins_n < 10) {
		sprintf(mins, "0%d", mins_n);
	}
}

/* Check if current date is in the range of the user's permissioned dates and times */
char *isInRange(list *check, char date[10], char time[5]) {
	char *status;
	char rqst_date_s[10], rqst_date_e[10];
	char day[2], month[2], year[4], hour[2], min[2], date_s[30], date_e[30], time_s[10], time_e[10];
	Node *current_rqst = check->head;
	struct tm dt_s, dt_e;

	/* Flip date_s format from d/m/y to y/m/d */
	//Cut the start date and time to smaller segments (day, month, year, hour, minute)
	strncpy(day, current_rqst->data.date_s, 2);
	strncpy(month, current_rqst->data.date_s + 3, 2);
	strncpy(year, current_rqst->data.date_s + 6, 4);
	strncpy(hour, current_rqst->data.time_s, 2);
	strncpy(min, current_rqst->data.time_s + 3, 2);
	// Apply date and time segments to tm structure
	dt_s.tm_mday = atoi(day);
	dt_s.tm_mon = atoi(month) - 1;
	dt_s.tm_year = atoi(year) - 1900;
	dt_s.tm_hour = atoi(hour);
	dt_s.tm_min = atoi(min);
	// Combine segments back to date string
	strftime(date_s, sizeof(date_s), "%Y/%m/%d", &dt_s);
	// Combine segments back to time string
	strftime(time_s, sizeof(time_s), "%R", &dt_s);

	/* Flip date_e format from d/m/y to y/m/d */
	//Cut the end date and time to smaller segments (day, month, year, hour, minute)
	strncpy(day, current_rqst->data.date_e, 2);
	strncpy(month, current_rqst->data.date_e + 3, 2);
	strncpy(year, current_rqst->data.date_e + 6, 4);
	strncpy(hour, current_rqst->data.time_e, 2);
	strncpy(min, current_rqst->data.time_e + 3, 2);
	// Apply date and time segments to tm structure
	dt_e.tm_mday = atoi(day);
	dt_e.tm_mon = atoi(month) - 1;
	dt_e.tm_year = atoi(year) - 1900;
	dt_e.tm_hour = atoi(hour);
	dt_e.tm_min = atoi(min);
	// Combine segments back to date string
	strftime(date_e, sizeof(date_e), "%Y/%m/%d", &dt_e);
	// Combine segments back to time string
	strftime(time_e, sizeof(time_e), "%R", &dt_e);

	// Check if current date is in the range of the user's permissioned dates
	if (strcmp(date_e, date) > 0 && strcmp(date_s, date) < 0) {
		if (strcmp(time_e, time) > 0 && strcmp(time_s, time) < 0) {
			status = "Entry available";
		}
		else
			status = "Not Authorised - Time";
	}
	else {
		status = "Not Authorised - Date";
	}

	return status;
}

/* Check if the entrance is valid due the users permissions */
void checkRequest(list *accs_list, list *rqst_list, char *day, char *month, char *year, char *hours, char *mins) {
	list *check = (list*)malloc(sizeof(list));
	Node *current_rqst = rqst_list->head;
	char *status, *name, date[11], time[6];

	// Combine day, month, year paramaters into date string format yyyy/mm/dd
	sprintf(date, "%4s/%2s/%2s", year, month, day);
	// Combine hours, mins paramaters into time string format HH/MM (24h)
	sprintf(time, "%s:%s", hours, mins);

	// Go through the entrance request list and check permissions for each requests by user CODE
	while (current_rqst != NULL) {
		// Return user's data (as a list Node) from access list according to its CODE from the request list
		check = search(accs_list, "", 0, current_rqst->rqst_data.name_code);
		
		// User CODE in request list not found in access list
		if (check == NULL) {
			status = "Unknown user";
			strcpy(current_rqst->rqst_data.status, status);
		}

		// User CODE found in access list
		else {
			strcpy(current_rqst->rqst_data.name_code, check->head->data.name);

			// Check for user permission status 'blocked'
			if (check->head->data.status == 1) {
				status = "Not Authorised user";
				strcpy(current_rqst->rqst_data.status, status);
			}

			// Check for user permission status 'master'
			else if (check->head->data.status == 5) {
				status = "Entry available";
				strcpy(current_rqst->rqst_data.status, status);
			}

			// Check for user permission status 'authorised door 1&2'
			else if (check->head->data.status == 4) {
				status = isInRange(check, date, time);
				strcpy(current_rqst->rqst_data.status, status);
			}

			// Check for user permission status 'authorised door 1 OR 2 only'
			else if (check->head->data.status == current_rqst->rqst_data.door + 1) {
				status = isInRange(check, date, time);
				strcpy(current_rqst->rqst_data.status, status);
			}

			// User is not authorised to enter the door
			else {
				status = "Not Authorised - Door";
				strcpy(current_rqst->rqst_data.status, status);
			}
		}
		current_rqst = current_rqst->next;
	}

	// Combine day, month, year paramaters into date string format dd/mm/yyyy
	sprintf(date, "%2s/%2s/%4s", day, month, year);
	// Write the entrace attempts in the log.txt file
	writeToLogFile("log.txt", rqst_list, date, time);

}