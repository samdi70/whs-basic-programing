#include <stdio.h>
#include <memory.h>
#include "json_c.c"
#include <string.h>

int count_if(json_value node) {
    int count = 0;

    if (node.type == JSON_OBJECT) {
        // 1) 지금 이 노드 자체가 If인지 확인
        json_value nt = json_get(node, "_nodetype");
        if (nt.type == JSON_STRING && strcmp((char*)nt.value, "If") == 0) {
            count++;
        }
        // 2) 이 객체 안의 모든 값들을 하나씩 다시 검사 (재귀 호출)
        json_object *obj = (json_object *)node.value;
        for (int i = 0; i <= obj->last_index; i++) {
            count += count_if(obj->values[i]);
        }
    } else if (node.type == JSON_ARRAY) {
        // 배열이면 모든 원소를 하나씩 다시 검사 (재귀 호출)
        json_array *arr = (json_array *)node.value;
        for (int i = 0; i <= arr->last_index; i++) {
            count += count_if(arr->values[i]);
        }
    }

    return count;
}




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
            printf("[%d] function!\n", count_func);
            // 함수의 리턴 타입 출력하기!
            json_value decl = json_get(item, "decl");
            char *fname = json_get_string(decl, "name");

            // if의 개수 출력하기!
            json_value body = json_get(item, "body");
            int if_count = count_if(body);

            json_value type = json_get(decl, "type");
            json_value type2 = json_get(type, "type");
            
            if (strcmp(json_get_string(type2, "_nodetype"), "PtrDecl") == 0) {
                type2 = json_get(type2, "type");
            }
            json_value type3 = json_get(type2, "type");

            // 함수의 이름 출력하기!
            char *ftype = json_get_string(type3, "names", 0); 
            printf("function name : %s\n", fname);
            printf("functions return type : %s\n", ftype);

            // 파라미터 타입과 이름 출력하기!
            json_value decl_type = json_get(decl, "type");
            json_value type_args = json_get(decl_type, "args");

            if (type_args.type == JSON_NULL) {
                printf("no params\n");
            } else {
                int param_count = json_len(json_get(type_args, "params"));
                for (int p = 0; p < param_count; p++) {
                    json_value args_params = json_get(type_args, "params", p);
                    char *pname = json_get_string(args_params, "name");

                    json_value ptype1 = json_get(args_params, "type");
                    if (strcmp(json_get_string(ptype1, "_nodetype"), "PtrDecl") == 0) {
                        ptype1 = json_get(ptype1, "type");
                    }
                    json_value ptype2 = json_get(ptype1, "type");
                    char *ptype = json_get_string(ptype2, "names", 0);

                    printf("param %d : type=%s, name=%s\n", p, ptype, pname);
                }
            }
            printf("if 개수 : %d\n", if_count);
            printf("----------------------------------\n");
        }

    }
    printf("==================================\n");
    // 존재하는 함수의 전체 개수 출력하기!
    printf("all function counting : %d\n", count_func);
    printf("==================================\n");


    return 0;
}