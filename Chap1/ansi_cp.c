#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 100
void error_handling(char* message);

int main(void)
{
    FILE *fp1, *fp2;
    int n;
    char buf[BUF_SIZE];

    fp1 = fopen("data.txt", "r");  // 只读
    if (fp1 == NULL)
        error_handling("fp1 fopen() error!");

    fp2 = fopen("data_copy.txt", "w");  // 创建文件，只写，若已存在清空原内容
    if (fp2 == NULL)
        error_handling("fp2 fopen() error!");

    while (fgets(buf, BUF_SIZE, fp1) != NULL) {  // fgets碰到EOF会返回NULL
        if (fputs(buf, fp2) == NULL)
            error_handling("write() error!");
    }

    fclose(fp1);
    fclose(fp2);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
