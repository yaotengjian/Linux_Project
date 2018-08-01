#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql.h>

int select_data()
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

	sprintf(sql, "SELECT * FROM student_info");

	printf("%s\n", sql);
	mysql_query(mysql_fd, sql);

	 MYSQL_RES *res = mysql_store_result(mysql_fd);

	 int row = mysql_num_rows(res);
	 int col = mysql_num_fields(res);
	 printf("row: %d, col: %d\n", row, col);

	MYSQL_FIELD *field = mysql_fetch_fields(res);
	int i = 0;
	for(; i < col; i++){
		printf("%s\t", field[i].name);
	}
	printf("\n");


	printf("<table border=\"1\">");
	for(i=0; i < row; i++){
		int j = 0;
		MYSQL_ROW r = mysql_fetch_row(res);

		printf("<tr>\n");
		for(; j < col; j++){
			printf("<td>%s</td>", r[j]);
		}
		printf("</tr>\n");

	}
	printf("</table>");

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

	select_data();
}







