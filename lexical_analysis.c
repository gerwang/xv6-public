//词法分析程序
//修改者：江俊广
//参考来源：https://www.cnblogs.com/zyrblog/p/6885922.html
#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"
#include "fs.h"
#include "lexical_analysis.h"
#define bool int

//查找保留字
int searchReserve(char reserveWord[][20], char s[]){
    for(int i=0; i<32; i++){
        if(strcmp(reserveWord[i], s) == 0){//若查找成功，
            return i+1;//则返回种类码
        }
    }
    return -1;//否则返回-1，代表查找不成功，即为标识符
}

//判断是否为字母
bool isLetter(char letter){
    return ( (letter >= 'a' && letter <= 'z') || 
        (letter >= 'A' && letter <= 'Z') ||
        letter == '_');//C语言允许下划线也作为标识符的一部分
}

//判断是否为数字
bool isDigit(char digit){
    return (digit >= '0' && digit <= '9');
}

//编译预处理，取出无用的字符和注释
//r为待处理字符,必须以'\0'结尾
//处理后的字符也放在r中
void filterResource(char r[]){
    int size = strlen(r);//输入串的长度
    char tempString[size+1];
    int count = 0;
    for(int i = 0; i < size; i++){
        if(r[i] == '/' && r[i+1] == '/'){//若为单行注释"//",则去除注释后面的东西，直至遇到回车换行
            while(r[i] != '\n') i++;//向后扫描
        }
        if(r[i] == '/' && r[i+1] == '*'){//若为多行注释 "/**/",则去除该内容
            i += 2;
            while(r[i] != '*' || r[i+1] != '/'){
                i++;//继续扫描
                if(r[i] == '$'){
                    printf(2, "注释出错，没有找到*/,程序结束！！！");
                    exit();
                }
            }
            i += 2;//跨过 "*/"
        }
        if(r[i] != '\n' && r[i] != '\t' && r[i] != '\v' && r[i] != '\r'){//若出现无用字符,则过滤
            tempString[count++] = r[i];//否则加载
        }
    }
    tempString[count] = '\0';
    strcpy(r, tempString);//产生净化后的程序
}

//词法分析子程序
//*synptr：返回词语的类型
//s: 输入的字符串,必须以'\0'结尾
//token: 返回的词语
//*pptr: 当前词法分析子程序在s中扫描到的位置
void Scanner(int* synptr, char s[], char token[], int*pptr){
    //根据DFA的状态转换图设计
    int syn = *synptr, p = *pptr;
    int i, count=0;//count用来做token的指示器，收集有用字符
    char ch = s[p];//作为判断使用
    while(ch == ' '){//过滤空格，防止程序因识别不了空格而结束
        p++;
        ch = s[p];
    }
    for(i = 0; i < 20; i++){//每次收集前先清零
        token[i] = '\0';
    }
    if(isLetter(s[p])){//开头为字母
        do{
            token[count++] = s[p++];//收集
        }while(isLetter(s[p]) || isDigit(s[p]));//其后所有的字母和数字
        token[count] = '\0';
        syn = searchReserve(reserveWord, token);//查表找到种别码
        if(syn == -1) syn = 100;//若不是保留字则是标识符
    }else if(isDigit(s[p])){//首字符为数字
        while(isDigit(s[p])) token[count++] = s[p++];//收集所有的数字
        token[count] = '\0';
        syn = 99;//常数种别码
    }else if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ';' || ch == '(' || ch == ')' || ch == '^'
        || ch == ',' || ch == '\"' || ch == '\'' || ch == '~' || ch == '#' || ch == '%' || ch == '['
        || ch == ']' || ch == '{' || ch == '}' || ch == '\\' || ch == '.' || ch == '\?' || ch == ':')
    {//若为运算符或者界符，查表得到结果
        token[0] = s[p++];//形成单字符串
        token[1] = '\0';
        for(i = 0; i < 36; i++){//查运算符界符表
            if(strcmp(token, operatorOrDelimiter[i]) == 0){
                syn = 33 + i;////获得种别码，使用了一点技巧，使之呈线性映射
                break;//查到后就退出
            }
        }
    }else if(s[p] == '<'){
        if(s[p+1] == '='){//<=
            p++; syn = 38;
        }else if(s[p+1] == '<'){//<<
            p++; syn = 58;
        }else{//<
            syn = 37;
        }
        p++;
    }else if(s[p] == '>'){
        if(s[p+1] == '='){//>=
            p++; syn = 40;
        }else if(s[p+1] == '>'){//>>
            p++; syn = 59;
        }else{//>
            syn = 39;
        }
        p++;
    }else if(s[p] == '='){
        if(s[p+1] == '='){//==
            p++; syn = 42;
        }else{//=
            syn = 41;
        }
        p++;
    }else if(s[p] == '!'){
        if(s[p+1] == '='){//!=
            p++; syn = 43;
        }else{//!
            syn = 68;
        }
        p++;
    }else if(s[p] == '&'){
        if(s[p+1] == '&'){//&&
            p++; syn = 53;
        }else{//&
            syn = 52;
        }
        p++;
    }else if(s[p] == '|'){
        if(s[p+1] == '|'){
            p++; syn = 55;
        }else{
            syn = 54;
        }
        p++;
    }else if(s[p] == '\0'){//结束符
        syn = 0;
    }else{//不能被以上词法分析识别，则出错
        printf(2,"error: there is no %c \n", ch);
        exit();
    }
    *synptr = syn; *pptr = p;//修改传进来的参数值
}


// int main(){
//     char s[100]="int main(int i){\n\ti = 0;\n char* s = 'this is fine'; \n};\n";
//     char token[20] = {0};
//     int syn = -1, p = 0;
//     printf(2,"源程序为%s\n",s);
//     filterResource(s);
//     printf(2,"过滤后的程序为%s\n",s);
//     int i;
//     while(syn != 0){
//         Scanner(&syn, s, token, &p);
//         if(syn == 100){//标识符
//             for(i=0; i<1000; i++){
//                 if(strcmp(IDentifierTbl[i], token) == 0) break;//如果已经在表中，则不作处理
//                 if(strcmp(IDentifierTbl[i], "") == 0){//如果出现了标识符表中未曾出现的元素
//                     strcpy(IDentifierTbl[i], token);//将新的标识符加入表中
//                     break;
//                 }
//             }
//             printf(2,"(标识符,%s)\n", token);
//         }else if(syn >= 1 && syn <= 32){//保留字
//             printf(2,"(保留字,%s)\n", reserveWord[syn-1]);
//         }else if(syn == 99){//常数
//             printf(2,"(常数, %s)\n", token);
//         }else if(syn >= 33 && syn <= 68){
//             printf(2,"(运算符,%s)\n", operatorOrDelimiter[syn-33]);
//         }
//     }
//     for(i = 0; i < 100; i++){//输出所有的标识符
//         if(strcmp(IDentifierTbl[i], "") == 0) break;
//         printf(2, "第%d个标识符： %s\n", i+1, IDentifierTbl[i]);
//     }
//     exit();
// }