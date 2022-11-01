/*     -----------------------------------------C Program API with MySQL---------------------------------------    */

#include<mysql.h>														//mysql.h header file is used to establish connection with MySQL.
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#define SIZE 30
#define MAX_Members 40													//Number of Max members allowed in club.

struct Node																//Structure for Tree is declared here
{
	int ID;
	int amountFunded;
	const char* name;
	struct Node* left;
	struct Node* right;
};

struct Node* root = NULL;												//root node is declared and initialized with NULL value. 
int id[MAX_Members];													//id of all members are displayed here.
int amt[MAX_Members];													//amount given by all members are displayed here.
char name[MAX_Members][50];												//name of all members are stored here.
int TotalMembers = 0;

struct Node* createNode(int id, char name[], int amt)
{
	struct Node* Leaf = (struct Node*)malloc(sizeof(struct Node));
	Leaf->name = name;													//New Node or Leaf are created here.
	Leaf->ID = id;
	Leaf->amountFunded = amt;
	Leaf->left = NULL;
	Leaf->right = NULL;
	return Leaf;
}

const char* intToString(int num)
{
	char* snum;															// int value is converted to string value.
	snum = malloc(sizeof(char) * 10);
	sprintf(snum, "%d", num);
	return snum;
}


const char* CreateInsertQuery(int id, char name[], int amount)
{
	char* query;
	query = malloc(sizeof(char) * 200);									//pointer is used as not return a dangling value of string.
	strcpy(query, "INSERT IGNORE INTO Channeling VALUES(");				//Insert query for MySQL is generated using this function.
	strcat(query, intToString(id));										//strcpy and strcat are used for copy string nd concatenation.
	strcat(query, ", '");
	strcat(query, name);
	strcat(query, "',");
	strcat(query, intToString(amount));
	strcat(query, ")");
	return query;														//query is returned here.
}

const char* CreateDeleteQuery(int id)
{
	char* query;														//pointer is used as not return a dangling value of string.
	query = malloc(sizeof(char) * 200);									//Delete query for MySQL is generated using this function.
	strcpy(query, "DELETE FROM CHANNELING WHERE ID = ");				//strcpy and strcat are used for copy string nd concatenation.
	strcat(query, intToString(id));
	return query;														//query is returned here.
}

const char* CreateUpdateQuery(int id)
{
	char* query;														//pointer is used as not return a dangling value of string.
	query = malloc(sizeof(char) * 200);									//Update query for MySQL is generated using this function.
	int amount;															//strcpy and strcat are used for copy string nd concatenation.
	printf("Enter the amount to be changed:");							//value of amount to be changed is given by user.
	scanf("%d", &amount);
	strcpy(query, "UPDATE CHANNELING SET AMOUNT = ");
	strcat(query, intToString(amount));
	strcat(query, " WHERE ID = ");
	strcat(query, intToString(id));
	return query;														//query is returned here.
}

void generateTree(MYSQL* con)
{
	if (mysql_query(con, "SELECT * FROM CHANNELING"))					//MySQL select query is executed here
	{
		fprintf(stderr, "Table Not Found!");							//error is returned if something unfortunate occurs.
		exit(0);														//code gets exited with code 0.
	}

	MYSQL_RES* result = mysql_store_result(con);						//result of MySQL query is stored in result.
	MYSQL_ROW row;  													//row is used to fetch a particular row at a same.

	int num_fields = mysql_num_fields(result);	     					//number of columns are stored.
	int j = 0;

	while ((row = mysql_fetch_row(result)))  							//next values of row are fetched after each while loop.
	{
		for (int i = 0; i < num_fields; i++)							//for loop is used to move across columns.
		{
			id[j] = atoi(row[0]);										//value is stored in array id.
			strcpy(name[j], row[1]);									//value is stored in array name.
			amt[j] = atoi(row[2]);										//value is stored in array amt.
		}
		j++;															//j is incremented by 1.
	}
	printf("\n");

	root = createNode(id[0], name[0], amt[0]);							//root Node is initialized with owner of the club's Record.

	TotalMembers = mysql_num_rows(result);
	for (int i = 1; i < TotalMembers; i++)			           			//except for first row other rows are added to Tree.
	{
		struct Node* newNode = createNode(id[i], name[i], amt[i]);		//New Nodes are created after each loop.

		if (newNode != NULL)											//if New Node is not empty, then
		{
			struct Node* temp = root;									//a temp Node is created with initial value root.
			struct Node* prev = NULL;
			while (temp != NULL)
			{						
				prev = temp;											//prev node is initialized with temp's previous values.
				if (amt[i] > temp->amountFunded)						//binary search tree is used.
				{
					temp = temp->right;									//Node is shifted to right side.
				}
				else
				{
					temp = temp->left;									//Node is shifted to left side.
				}
			}
			if (amt[i] > prev->amountFunded)
			{
				prev->right = newNode;									//Node is created to right side.
			}
			else
			{
				prev->left = newNode;									//Node is created on the left side.
			}
		}
	}
	mysql_free_result(result);  										//result is freed from memory.
}

void PreorderTree(struct Node* temp)									//Tree is displayed in Preorder form.
{
	if (temp == NULL)													//if temp is Null exit from function.
	{
		return;
	}																	//data is printed.
	printf("%d\t\t\t\t%s\t\t\t\t%d\n", temp->ID, temp->name, temp->amountFunded);
	PreorderTree(temp->left);											//left subtree is printed.
	PreorderTree(temp->right);											//right subtree is printed.
}

void main()
{
	MYSQL* con = mysql_init(NULL);										//console is created here.
	char pwd[25];														
	printf("Enter Password:");											//password is taken from user for MySQL user.
	scanf("%s", pwd);

	if (con == NULL)													//if con is NULL error is displayed.
	{
		fprintf(stderr, "Initialization Failed!!\n");
		exit(-1);
	}

	if (mysql_real_connect(con, "localhost", "root", pwd, NULL, 3306, NULL, CLIENT_MULTI_STATEMENTS) == NULL)
	{																	//Connection occurs with MySQL and C.
		fprintf(stderr, "Database Connection Failed!!\n");				//Appropriate Message is printed.
		exit(0);
	}

	if (mysql_query(con, "CREATE DATABASE IF NOT EXISTS CProject"))		//Database is created if it doesn't exists.
	{
		fprintf(stderr, "Database Not Created!!\n");					//Error Message is printed here.
		exit(0);
	}

	if (mysql_query(con, "Use CProject"))								//database created is used, i.e. database values can be changed now.
	{
		fprintf(stderr, "Database in Use\n");							//Error Message is printed here, if it occurs.
		exit(0);
	}

	if (mysql_query(con, "CREATE TABLE IF NOT EXISTS CHANNELING(ID INT PRIMARY KEY, NAME VARCHAR(255), AMOUNT INT)"))
	{																	//Table is Created here, if it does'nt exists.
		fprintf(stderr, "Table Not Created\n");							//Error Message is printed here.
		exit(0);
	}

	if (mysql_query(con, "SELECT * FROM CHANNELING"))					//all data is extracted from table.
	{
		fprintf(stderr, "Table Not Found!");							//Error Message is printed here, if it occurs. 
		exit(0);
	}

	MYSQL_RES* result = mysql_store_result(con);						//result is stored in result variable.
	MYSQL_ROW row;														//row stores the entire row and converts it into string format during runtime.

	int num_rows = mysql_num_rows(result);								//number of rows are stored in it.
	int num_fields = mysql_num_fields(result);							//number of fields are stored in it.

	if (num_rows == 0)													//if result is null or no data is present in table then record is created with Owner as first value.
	{
		int id = 0;
		printf("Enter ID of the owner:");								//ID of owner is given by user.
		scanf("%d", &id);
		int amount = 0;
		printf("Enter amount Funded:");									//amount funded by owner is stored here.
		scanf("%d", &amount);
		char name[50];
		printf("Enter the name of Owner:");								//name of owner is stored here.
		scanf("%s", name);
		char query[200];
		strcpy(query, CreateInsertQuery(id, name, amount));				//Owner's data is inserted here.
		if (mysql_query(con, query))
		{
			fprintf(stderr, "Data Not Uploaded!!");
			exit(0);
		}
	}

	mysql_free_result(result);											//result data is freed from memory.

	generateTree(con);													//Tree is generated from given data extracted from MySQL.

	printf("\t\t-------------------------------------------------------\n");
	printf("\t\t--------------------WELCOME TO CLUB--------------------\n");
	printf("\t\t-------------------------ORION-------------------------\n");
	printf("\t\t-------------------------------------------------------\n");

	int a = 0, choice;
	while (a == 0)														//desired infinite Loop is created here.
	{
		printf("What Would Like To Perform??\n");
		printf("(1)Add a new member\n(2)Display\n(3)Remove a member\n(4)Update Member's Record\n(5)Exit");
		printf("\nEnter Your Choice:");									//choice is taken from user here.
		scanf("%d", &choice);											
		if (choice == 1)												//if choice gets value 1, then
		{
			int id, amount;												//id is taken from user.
			printf("Enter ID:");
			scanf("%d", &id);
			printf("Enter Name:");										//name is taken from user.
			char name[50];
			scanf("%s", name);
			printf("Enter Amount:");									//amount is taken from user.
			scanf("%d", &amount);
			char query[200];
			strcpy(query, CreateInsertQuery(id, name, amount));			//Insert Query is created using function.
			if (mysql_query(con, query))								//Query is executed here.
			{
				fprintf(stderr, "Data Not Inserted!!\n");				//Error Message is printed here.
				exit(1);
			}
			else
			{
				printf("Data Added Successfully!!\n");					//Appropriate Message is printed here if data is added successfully.
			}
			generateTree(con);											//New Tree is generated after insertion.
		}
		else if (choice == 2)											//if Choice is 2, then
		{
			int ch, b = 0;
			while (b == 0)												//Desired Infinite while loop is used.
			{
				printf("(1)Display Funds Deposited\n(2)Display All Members\n(3)Display Amount Owner Owned this month\n(4)Exit\n");
				printf("Enter Your Choice:");							//Display Options are given to user and choice is taken from user.
				scanf("%d", &ch);
				if (ch == 1)											//if ch is 1, then
				{
					float funds = 0;
					if (mysql_query(con, "SELECT AMOUNT FROM CHANNELING"))
					{													//Amount Deposited by members' data is extracted from MySQL.
						fprintf(stderr, "Table Not Found\n");
						exit(1);
					}
					MYSQL_RES* resultAMT = mysql_store_result(con);		//result is stored in here.
					MYSQL_ROW rowAMT;
					int num_fieldAMT = mysql_num_fields(resultAMT);
					while ((rowAMT = mysql_fetch_row(resultAMT)))		//while is used to fetch row.
					{
						for (int i = 0; i < num_fieldAMT; i++)			//for loop is used to hover across columns.
						{
							funds += (90 * (atoi(rowAMT[i])) / 100);	//90% of funds is given to club development and 10% is given parent Member.
						}
					}
					printf("Funds Raised by Members:%f", funds);		//funds are given.
				}
				else if (ch == 2)
				{
					printf("Members of Club ORION are listed below:\nID                        NAME                        Amount Deposited\n");
					PreorderTree(root);									//Tree is displayed in Preorder format.
				}
				else if (ch == 3)
				{
					if (root->left != NULL || root->right != NULL)		//Amount Got to Owner is Displayed.
					{
						printf("Amount Owned by Owner : %d", root->left->amountFunded + root->right->amountFunded);
					}
					else
					{
						printf("No Members present in the Club");
					}
				}
				else if (ch == 4)
				{
					printf("Exiting....");								//Moves to Main Menu directly by breaking while loop.
					b = 1;
				}
				else
				{
					printf("You Have Given Wrong Input!!");				//user Input Error.
				}
			}
		}
		else if (choice == 3)
		{
			int id;														//Id is taken in which data is to be changed.
			printf("Enter the ID of person to be updated:");
			scanf("%d", &id);											//Input is taken from input.
			char query[200];
			strcpy(query, CreateUpdateQuery(id));						//Updated Query is created here.
			if (mysql_query(con, query))								//Query is executed here. 
			{
				fprintf(stderr, "ID Not Found\n");						//Error Message is Handled.
			}
			else
			{
				printf("Data Updated!!");
			}
			generateTree(con);											//New Tree is generated here.
		}
		else if (choice == 4)
		{
			int id;														//Id is taken in which data is to be deleted.
			printf("Enter the ID of member to be deleted:");
			scanf("%d", &id);											//Input is taken from input.
			if (id == root->ID)
			{															
				printf("Can't delete the record of owner!!");			//checks if input is user's ID or not.
			}
			else
			{
				char query[200];
				strcpy(query, CreateDeleteQuery(id));					//Delete Query is created here.
				if (mysql_query(con, query))							//Query is executed here. 
				{
					fprintf(stderr, "ID No Found\n");
				}
				generateTree(con);										//New Tree is generated here.
			}
		}
		else if (choice == 5)
		{																//Exited from code.
			printf("Exiting.......\n");
			a = 1;
		}
		else
		{
			printf("You have Entered Wrong Input!!!\n");				//User Wrong Input Error.
		}
	}

	mysql_close(con);													//Data is changed and saved in DBMS.

	exit(2);															//program exits with code(1)
}