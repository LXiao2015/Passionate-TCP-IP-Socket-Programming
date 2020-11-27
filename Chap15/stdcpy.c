#include <stdio.h>
#define BUF_SIZE 3    // 最短数组长度

int main()
{
    FILE *fp1, *fp2;
    char buf[BUF_SIZE];

    fp1 = fopen("news.txt", "r");    // 使用尽量大的文件
    fp2 = fopen("cpy.txt", "w");

    while (fgets(buf, BUF_SIZE, fp1) != NULL)
        fputs(buf, fp2);

    fclose(fp1);
    fclose(fp2);
    return 0;
}
