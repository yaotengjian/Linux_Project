#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void cal(char *data)
{
	//first_data=1000&second_data=2000
	int d1, d2;
	sscanf(data, "first_data=%d&second_data=%d", &d1, &d2);

	printf("<html>\n");

	printf("<h3>%d+%d=%d</h3>\n", d1, d2, d1+d2);
	printf("<h3>%d-%d=%d</h3>\n", d1, d2, d1-d2);
	printf("<h3>%d*%d=%d</h3>\n", d1, d2, d1*d2);
	printf("<h3>%d/%d=%d</h3>\n", d1, d2, d1/d2);
	printf("<h3>%d%%%d=%d</h3>\n", d1, d2, d1%d2);

	printf("</html>\n");
}

int main()
{

	char method[32];
	char buff[1024];
	if(getenv("METHOD") != NULL){
		strcpy(method, getenv("METHOD"));

		if(strcasecmp(method, "GET") == 0){
			strcpy(buff, getenv("QUERY_STRING"));
		}
		else if(strcasecmp(method, "POST") == 0){
			int content_len = atoi(getenv("CONTENT_LENGTH"));
			char c;
			int i = 0;
			for(; i < content_len; i++){
				read(0, &c, 1);
				buff[i] = c;
			}
			buff[i] = '\0';
		}else{
		}

		cal(buff);
	}
	return 0;
}




