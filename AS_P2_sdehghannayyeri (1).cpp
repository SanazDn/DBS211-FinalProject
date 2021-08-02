#define _CRT_SECURE_NO_WARNINGS
#include <mysql.h>
#include <iostream>
#include <sstream>
#include<string>
#include<stdio.h>

using namespace std;

struct Employee {
	int employeeNumber;
	char lastName[50];
	char firstName[50];
	char email[100];
	char phone[50];
	char extension[10];
	char reportsTo[100];
	char jobTitle[50];
	char city[50];
};


int menu(void);
int findEmployee(MYSQL* conn, int employeeNumber, struct Employee* emp);
void displayEmployee(MYSQL* conn, struct Employee emp);
void displayAllEmployees(MYSQL* conn);
void insertEmployee(MYSQL* conn, struct Employee emp);
void updateEmployee(MYSQL* conn, int employeeNumber);
void deleteEmployee(MYSQL* conn, int employeeNumber);

int main(void) {
	struct Employee emp = { 0,"","","","","","","","" };

	int test;
	int option;
	int exequery;
	int id ;


	MYSQL* conn;
	conn = mysql_init(0);


	conn = mysql_real_connect(conn, "mymysql.senecacollege.ca", "db_vhaddadi", "Selena@2016", "db_vhaddadi", 3306, nullptr, 0);

	do {

		option = menu();

		switch (option)
		{


		case 1:
			printf("Enter Employee Number: ");
			scanf("%d", &id);
		
			test = findEmployee(conn, id, &emp);

			if (test == 1) {
				displayEmployee(conn, emp);
			}
			break;

		case 2:
			displayAllEmployees(conn);

			break;
		case 3:

			insertEmployee(conn, emp);
				
			
		break;

		case 4:

			printf("Enter Employee Number: ");
			scanf("%d", &id);

			test = findEmployee(conn, id, &emp);

			if (test == 1) {
				updateEmployee(conn, id);
			}
			break;

		case 5:

			deleteEmployee(conn, id);

			break;

		case 0:
			printf("\n");
			cout << "Exiting Employee Data Program." << endl;

			break;

		default:
			printf("ERROR: Incorrect Option: Try Again\n");

		}
	} while (option != 0);


	mysql_close(conn);

	return 0;

}


int menu(void) {

	int option;
	printf("\n");
	printf("********************* HR Menu *********************\n");
	printf("1) Find Employee\n");
	printf("2) Employees Report\n");
	printf("3) Add Employee\n");
	printf("4) Update Employee\n");
	printf("5) Remove Employee\n");
	printf("0) Exit\n");
	//do {
	printf("Please select from the above options: ");
	scanf("%d", &option);
	
	printf("\n");
	
	return option;

};



int findEmployee(MYSQL* conn, int employeeNumber, struct Employee* emp) {

	int  exequery;
	int test = -1;

	string query = "select e.employeeNumber, e.lastName, e.firstName, e.email, of.phone, e.extension, COALESCE(concat(em.firstName, ' ', em.lastname),' '), e.jobTitle, of.city FROM employees AS e LEFT JOIN employees AS em	ON e.reportsTo = em.employeeNumber inner join offices As of	on e.officeCode = of.officeCode	where e.employeeNumber =  " + to_string(employeeNumber) + ";";

	const char* q = query.c_str();
	exequery = mysql_query(conn, q);

	MYSQL_RES* res;
	res = mysql_store_result(conn);
	int num_rows = mysql_num_rows(res);

	if (res) {
		
		MYSQL_ROW row;
		row = mysql_fetch_row(res);
		if (num_rows > 0) {

			printf("\n");
			emp->employeeNumber = stoi(row[0]);
			strncpy(emp->lastName, row[1], 50);
			strncpy(emp->firstName, row[2], 50);
			strncpy(emp->email, row[3], 100);
			strncpy(emp->phone, row[4], 50);
			strncpy(emp->extension, row[5], 10);
			strncpy(emp->reportsTo, row[6], 100);
			strncpy(emp->jobTitle, row[7], 50);
			strncpy(emp->city, row[8], 50);
			test = 1;
		}

		else {
			
			printf("\n");
			cout << "Employee " + to_string(employeeNumber) + " does not exist." << endl;
			test = 0;
		}
	}
	   
	return  test;
}



void displayEmployee(MYSQL* conn, struct Employee emp) {

	printf("employeeNumber = %d\n", emp.employeeNumber);
	printf("lastName = %s \n", emp.lastName);
	printf("firstName = %s\n", emp.firstName);
	printf("email = %s \n", emp.email);
	printf("phone = %s\n", emp.phone);
	printf("extension = %s\n", emp.extension);
	printf("reportsTo = %s\n", emp.reportsTo);
	printf("jobTitle = %s\n", emp.jobTitle);
	printf("city = %s\n", emp.city);
}
void displayAllEmployees(MYSQL* conn) {

	string query1 = " SELECT 	e.employeeNumber, concat(e.firstName, ' ', e.lastName), e.email, of.phone, e.extension,	 COALESCE(concat(em.firstName, ' ', em.lastname),' ') FROM employees AS e LEFT JOIN employees AS em ON e.reportsTo = em.employeeNumber inner join offices As of on e.officeCode = of.officeCode ;";

	const char* q1 = query1.c_str();
	int exequery1 = mysql_query(conn, q1);

	if (!exequery1) {

		MYSQL_RES* res1;
		res1 = mysql_store_result(conn);
		int num_rows = mysql_num_rows(res1);

		if (mysql_num_rows(res1) == 0) {
			cout << "There is no employees information to be displayed." << endl;
		}
		else {

			cout << "  E		Employee Name			 Email				 Phone			 Ext			 Manager" << endl;
			cout << "--------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
			MYSQL_ROW row1;

			while (row1 = mysql_fetch_row(res1)) {



				cout << " " << row1[0] << "	";
				cout.setf(ios::left);
				cout.width(20);
				cout << row1[1] << "	 ";
				cout.width(40);
				cout << row1[2] << "	";
				cout.width(25);
				cout << row1[3] << "	";
				cout.width(20);
				cout << row1[4] << "	" << row1[5] << endl;



			}

		}
	}
}

void insertEmployee(MYSQL* conn, struct Employee emp) {
	int test, id;
	printf("Enter Employee Number: ");
	scanf("%d", &id);

	test = findEmployee(conn, id, &emp);

	if (test == 1) {
	printf ("An employee with the same employee number exists.\n");
	
	}else if (test == 0) {
		
		string query2 = "insert  into `employees` (`employeeNumber`,`lastName`,`firstName`,`email`,`extension`,`jobTitle`,`officeCode`,`reportsTo`) values( ";

		printf("\n");
		printf("Employee Number: ");
		scanf("%d", &(emp.employeeNumber));
		query2 += to_string(emp.employeeNumber);
		query2 += ',';
		
		printf("Last Name: ");
		scanf("%s", emp.lastName);
		query2 += "\'";
		query2 += (emp.lastName);
		query2 += "\',";
		printf("First Name:");
		scanf("%s", emp.firstName);
		query2 += "\'";
		query2 += (emp.firstName);
		query2 += "\',";
		printf("Email : ");
		scanf("%s", emp.email);
		query2 += "\'";
		query2 += (emp.email);
		query2 += "\',";
		printf("extension : ");
		scanf("%s", emp.extension);
		query2 += "\'";
		query2 += (emp.extension);
		
		query2 += "\',";
		cin.ignore(2000, '\n');
		printf("Job Title :");
	
		cin.getline(emp.jobTitle, 50, '\n');
		query2 += "\'";
		query2 += (emp.jobTitle);
		query2 += "\',";
		query2 += "\'";
		query2 += to_string (1);
		query2 += "\',";
		query2 += "\'";
		query2 += to_string (1002);
		query2 += "\'";
		query2 += ")";
		
		printf("\n");
		const char* q2 = query2.c_str();
		int exequery2 = mysql_query(conn, q2);

		if (!exequery2) {

			MYSQL_RES* res2;
			res2 = mysql_store_result(conn);
			printf("The new employee is added successfully.\n");
		}	else {

				cout << "Query failed.\n" << mysql_errno(conn) << " " << mysql_error(conn) << endl;
				}
	}
}

void updateEmployee(MYSQL* conn, int employeeNumber) {
	
		char next[10];
		cout << "New Extension :";
		cin >> next;

		string query3 = "UPDATE employees  SET extension = '";
		query3 += next;
		query3 += "\'";
		query3 += " WHERE employeeNumber = ";
		query3 += to_string(employeeNumber);
		query3 += "\;";
		
		const char* q3 = query3.c_str();
		int exequery3 = mysql_query(conn, q3);
		if (!exequery3) {
			MYSQL_RES* res3;
			res3 = mysql_store_result(conn);
			cout << " Employee " + to_string(employeeNumber) + " updated." << endl;
		}	else {
		
					cout << "Query failed." << mysql_errno(conn) << " " << mysql_error(conn) << endl;
				}
}





void deleteEmployee(MYSQL* conn, int employeeNumber) {

	struct Employee emp = { 0,"","","","","","","","" };
	
	int test, id;
	printf("Enter Employee Number: ");
	scanf("%d", &id);

	test = findEmployee(conn, id, &emp);

	if (test == 1) {
		
		string query4 = "delete from employees 	where employeeNumber like '";
		query4 += to_string(id);
		query4 += "\'";
		query4 += "\;";

		const char* q4 = query4.c_str();
		int exequery4 = mysql_query(conn, q4);
		(long)mysql_affected_rows(conn);

//		cout << mysql_affected_rows(conn) << endl;

		if (!exequery4 && mysql_affected_rows(conn)==1) {

			MYSQL_RES* res4;
			res4 = mysql_store_result(conn);
			cout << " The Employee is deleted." << endl;
		}
		else {

			cout << "Query failed." << mysql_errno(conn) << " " << mysql_error(conn) << endl;

		}

	}
}
