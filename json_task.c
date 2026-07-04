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

    // 3. 과제 시작 
    json_value ext = json_get(json, "ext");
    int ext_len = json_len(ext);

    printf("==================================\n");
    printf("ext length is : %d\n", ext_len);
    printf("==================================\n");

    int count_func = 0;

    for (int i=0; i<ext_len; i++){
        json_value item = json_get(json, "ext", i);
        char *nodetype = json_get_string(item, "_nodetype");

        if (strcmp(nodetype, "FuncDef") == 0){
            // 함수 개수 올리기!
            count_func++;
            
            // 함수의 리턴 타입 출력하기!
            json_value decl = json_get(item, "decl");
            char *fname = json_get_string(decl, "name");

            json_value type = json_get(decl, "type");
            json_value type2 = json_get(type, "type");
            
            if (strcmp(json_get_string(type2, "_nodetype"), "PtrDecl") == 0) {
                type2 = json_get(type2, "type");
            }
            json_value type3 = json_get(type2, "type");
            char *ftype = json_get_string(type3, "names", 0); 
            
            printf("%d - functions return type : %s\n", i, ftype);
            
        }

    }
    printf("==================================\n");
    // 존재하는 함수의 전체 개수 출력하기
    printf("all function counting : %d\n", count_func);
    printf("==================================\n");


    return 0;
}