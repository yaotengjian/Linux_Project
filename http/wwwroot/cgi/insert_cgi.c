#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql.h>

int insert_data(char *name, int age, char *sex)
{
	MYSQL *mysql_fd = mysql_init(NULL);

	if(mysql_real_connect(mysql_fd, "127.0.0.1", "root", "", "25_class",\
				3306, NULL, 0) == NULL){
		printf("connect error!\n");
		return 1;
	}else{
		printf("connect success!\n");
	}

	char sql[1024];

	sprintf(sql, "INSERT INTO student_info (name, age, sex) VALUES (\'%s\',%d,\'%s\')", name, age, sex);

	printf("%s\n", sql);
	mysql_query(mysql_fd, sql);

	mysql_close(mysql_fd);
//	printf("Client Version: %s\n", mysql_get_client_info());
	return 0;
}

int main()
{
	char data[1024];
	if(getenv("METHOD") != NULL){
		if(strcasecmp(getenv("METHOD"), "GET") == 0){
			strcpy(data, getenv("QUERY_STRING"));
		}else{
			int content_length = atoi(getenv("CONTENT_LENGTH"));
			int i = 0;
			for(; i < content_length; i++){
				read(0, data+i, 1);
			}
			data[i] = 0;
		}
	}

	//name=''&age=''&sex=''
	char name[32];
	int age;
	char sex[8];
	printf("%s\n", data);

	//name=lisi&age=25&sex=m
	char *p = strtok(data, "=&");
	p = strtok(NULL, "=&");
	strcpy(name, p);

	strtok(NULL,"=&");
	p = strtok(NULL, "=&");
	age = atoi(p);
	strtok(NULL, "=&");
	p = strtok(NULL, "=&");
	strcpy(sex, p);

	//sscanf(data, "name=%s&age=%d&sex=%s", name, &age, sex);
	insert_data(name, age, sex);
}







