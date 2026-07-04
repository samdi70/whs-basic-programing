#include <stdio.h>
#include <memory.h>
#include "json_c.c"
#include <string.h>

int main(void){

    // 1. ast.json 파일 불러와서 총 크기 설정하고 동적 할당
    FILE *fp;
    int c;
    fp = fopen("ast.json", "rb");
    if (fp == NULL){
        printf("파일열기 실패\n");
    } else {
        printf("파일열기 성공\n");
    }

    fseek(fp, 0, SEEK_END);
    unsigned long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    printf("json size is : %lu\n", size);

    char *str = (char *)malloc(size + 1);
    fread(str, 1, size, fp);
    str[size] = '\0';

    fclose(fp);

    // 2. str에 들어있는 json 텍스트를 json 형식으로 변환
    json_value json = json_create(str);

    // 3. 함수의 개수 추출하기 (과제#1)
    json_value ext = json_get(json, "ext");
    int ext_len = json_len(ext);
    printf("==================================\n");
    printf("ext length is : %d\n", ext_len);

    int count_func = 0;

    for (int i=0; i<ext_len; i++){
        json_value item = json_get(json, "ext", i);
        char *nodetype = json_get_string(item, "_nodetype");

        if (strcmp(nodetype, "FuncDef") == 0){
            count_func++;
        }

    }

    printf("function counting : %d\n", count_func);
    printf("==================================\n");




    return 0;
}