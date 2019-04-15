#include <stdio.h>
#include <time.h>
#include <locale.h>
#include "functions.h"

#define ACCESS_PATH "access.txt"
/***
* This is the path where the access file is located.
* When you submit the program make sure this path match to: "access.txt"
***/
#define REAQUESTS_PATH "requests.txt"
/***
* This is the path where the requests file is located.
* When you submit the program make sure this path match to: "requests.txt"
***/
#define LOG_PATH "log.txt"
/***
* This is the path where the log file is located.
* When you submit the program make sure this path match to: "log.txt"
***/

void main() {
	list *accs_lst = (list *)malloc(sizeof(list));
	list *srch_list = (list*)malloc(sizeof(list));
	list *rqst_lst = (list*)malloc(sizeof(list));
	int option, action, srch_status;
	char *updt_name = (char*)malloc(sizeof(char));
	char srch_name[21], srch_code[9], year[5], month[3], day[3], hours[3], mins[3];
	char *temp;

	getDateTime(day, month, year, hours, mins);

	init_list(accs_lst);
	init_list(srch_list);
	init_list(rqst_lst);
	readAccess(ACCESS_PATH, accs_lst);
	readRequsts(REAQUESTS_PATH, rqst_lst);
	checkRequest(accs_lst, rqst_lst, day, month, year, hours, mins);

	/* Display Menu */
	while (1) {
		// Operations menu
		{
			printf("\n *  *  *  *  *  *  *  *  *  *  *  *  *\n");
			printf("\n *  Operations menu                  *\n");
			printf("\n *  1. Search for user               *\n");
			printf("\n *  2. Add new user                  *\n");
			printf("\n *  3. Update user permissions       *\n");
			printf("\n *  4. Print log file                *\n");
			printf("\n *  5. Print user list (acsess.txt)  *\n");
			printf("\n *  6. Quit                          *\n");
			printf("\n *  *  *  *  *  *  *  *  *  *  *  *  *\n");
			printf("\n Please select your action [1-5]. Exit [6]:");
		}
		
		// Check for invalid input (not number)
		if (scanf("%d", &option) != 1) {
			printf(" \n*Error: Invalid input. Try again.\n");
			scanf("%s", &temp); //clear input buffer
			continue;
		} 
		
		switch (option) {

		// Search for user by its name(s) (1) or status (2) or code (3) or main menu (0)
		case 1: {
			printf("Choose searching by user NAME (1) or STATUS (2) or CODE (3). MAIN MENU (0): ");
			scanf("%d", &action);

			// Invalid action input
			if (action > 3) {
				printf("ERROR: Invalid action.\n Please choose searching by user NAME (1) or STATUS (2) or CODE (3). MAIN MENU (0): ");
				scanf("%d", &action);
			}

			// Check for chosen action: Search a user by user NAME/STATUS/CODE or go back to main menu
			switch (action)
			{
				// Go back to main menu
				case 0: {
					break;
				}
				// Search a user by user NAME
				case 1: {
					printf("Enter the NAME you want to search for: ");
					scanf("%20s", srch_name);
					srch_list = search(accs_lst, srch_name, 0, "");
					break;
				}
				// Search a user by user STATUS
				case 2: {
					printf("Enter the STATUS [1-5] you want to search for: ");
					scanf("%d", &srch_status);
					srch_list = search(accs_lst, "", srch_status, "");
					break;
				}
				// Search a user by user CODE
				case 3: {
					printf("Enter the CODE you want to search for: ");
					scanf("%8s", &srch_code);
					srch_list = search(accs_lst, "", 0, srch_code);
					break;
				}	
			}
			
			// Check if any result has been found
			if (srch_list != NULL && action != 0) {
				print(ACCESS_PATH, srch_list);
			}
			// No result found
			else if (action != 0) {
				printf("ERROR: User not found! Please Try again\n");
				printf("Choose searching by user NAME (1) or STATUS (2) or CODE (3). MAIN MENU (0): ");
				scanf("%d", &action);
			}

			break;
		}

		// add new user
		case 2: {
			AddUser(accs_lst);
			writeToFile(ACCESS_PATH, accs_lst);
			print(ACCESS_PATH, accs_lst);

			break;
		}

		// Update user's permissions
		case 3: {
			printf("Type '1' (name) or '2' (code) to choose which user to update, '3' for users list. MAIN MENU (0): ");
			scanf("%d", &action);

			// Invalid action input
			if (action > 3) {
				printf("ERROR: Invalid action.\n Type '1' (name) or '2' (code) to choose which user to update, '3' for users list. MAIN MENU (0): ");
				scanf("%d", &action);
			}

			// Print the current users list to the console
			else if (action == 3) {
				print(ACCESS_PATH, accs_lst);
				printf("Type '1' (name) or '2' (code) to choose which user to update. MAIN MENU (0): ");
				scanf("%d", &action);
			}

			// Check for chosen action: Update user by its user NAME / CODE or go back to main menu
			switch (action)
			{
				// Go back to main menu
				case 0: {
					break;
				}
				// Update user by its user NAME
				case 1: {
					printf("Enter a user NAME to update: ");
					scanf("%s", srch_name);
					srch_list = search(accs_lst, srch_name, 0, "");

					// Check that there is only one user by a given user NAME
					if (length(srch_list) < 2) {
						printf("Choose which parameters to update: STATUS (1), TIME RANGE (2), MAIN MENU (0): ");
						scanf("%d", &option);

						// Invalid action input
						if (option > 2) {
							printf("ERROR: Invalid action.\n Please choose again STATUS (1), TIME RANGE (2), MAIN MENU (0): ");
							scanf("%d", &option);
						}

						// (NAME) Check for chosen action: Update user's permissions by STATUS/TIME RANGE or go back to main menu
						switch (option)
						{
						// Go back to main menu
						case 0: {
							break;
						}
						// Update user's STATUS permissions
						case 1: {
							printf("Enter the new status permission for user %s: ", srch_name);
							scanf("%d", &srch_status);
							updateUser(ACCESS_PATH, accs_lst, srch_list, srch_name, srch_status, "");
							break;
						}
						// Update user's DATE TIME range permission							
						case 2: {
							updateUser(ACCESS_PATH, accs_lst, srch_list, srch_name, 0, "");
							break;
						}
						}
					}
					// There are multiple users with the same name -> update user by his CODE
					else {
						printf("ERROR: There is more then one user named: %s\n", srch_name);
						action = 2;
					}

					break;
				}
				// Update user by its user CODE
				case 2: {
					printf("Enter a user CODE to update: ");
					scanf("%s", srch_code);
					printf("Choose which parameters to update: STATUS (1), TIME RANGE (2), back to MAIN MENU (0): ");
					scanf("%d", &option);
					srch_list = search(accs_lst, "", 0, srch_code);

					// Invalid action input
					if (option > 2) {
						printf("ERROR: Invalid action.\n Please choose again STATUS (1), TIME RANGE (2), back to MAIN MENU (0): ");
						scanf("%d", &option);
					}

					// (CODE) Check for chosen action: Update user's permissions by STATUS/TIME RANGE or go back to main menu
					switch (option)
					{
					// Go back to main menu
					case 0: {
						break;
					}
					// Update user's STATUS permissions
					case 1: {
						printf("Enter the new status permission for user code %s: ", srch_code);
						scanf("%d", &srch_status);
						updateUser(ACCESS_PATH, accs_lst, srch_list, "", srch_status, srch_code);
						break;
					}
					// Update user's DATE TIME range permission	
					case 2: {
						updateUser(ACCESS_PATH, accs_lst, srch_list, "", 0, srch_code);
						break;
					}
					}

					break;			
				}
			}

			// Check entrance requests after updating user's permissions
			init_list(rqst_lst);
			readRequsts(REAQUESTS_PATH, rqst_lst);
			checkRequest(accs_lst, rqst_lst, day, month, year, hours, mins);
			
			break;
		}

		// Print log file to the console
		case 4: {
			print(LOG_PATH, rqst_lst);
			break;
		}

		// Print access file to the console
		case 5: {
			print(ACCESS_PATH, accs_lst);
			break;
		}

		// Exit program
		case 6: {
			free(accs_lst);
			free(srch_list);
			free(rqst_lst);
			free(updt_name);
			return(0);
			break;
		}

		default: {
			printf("Invalid Option. Please Try again.");
			getch();
		}
		} // End of Switch
	} // End of While
}