#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<errno.h>

char* cmdline = NULL;
//命令行参数表
char* argv[50];
size_t argc;
//自己的环境变量
char* my_env[1024];
int my_env_num;
//系统环境变量表
 extern char** environ;

char* my_getenv(const char* name);
void init_env(char** environ);

//获取用户名
char* get_user(){
    return my_getenv("USER");
}

//获取当前工作路径
char* get_pwd(){
    return my_getenv("PWD");
}

//获取PATH
char* get_path(){
    return my_getenv("PATH");
}

//获取env
char* my_getenv(const char* name){
    int len = strlen(name);
    for(int i = 0; i < my_env_num; ++i){
        if(strncmp(my_env[i], name, len) == 0 && my_env[i][len] == '='){
            return my_env[i] + len + 1;
        }
    }
    return NULL;
}
//修改env
void my_setenv(const char* name, const char* value){
    //有则改
    int len = strlen(name);
    char* new_env = (char*)malloc(strlen(name) + strlen(value) + 2);
    sprintf(new_env, "%s=%s", name, value);
    for(int i = 0; i < my_env_num; ++i){
        if(strncmp(my_env[i], name, len) == 0){
            free(my_env[i]);
            my_env[i] = new_env;
            return;
        }
    } 

    //无则添加
    my_env[my_env_num] = new_env;
    ++my_env_num;
}

void  print_prompt(){
    char* user = get_user();
    char* pwd = get_pwd();
    printf("[%s@我的shell %s]$ ", user, pwd);
}

void get_command(){
    size_t len;
    getline(&cmdline, &len, stdin);
    //printf("%s", cmdline);

}

bool parse_command(){
    cmdline[strlen(cmdline) - 1] = '\0';
    memset(&argv, 0, sizeof(argv));
    argc = 0;
    argv[argc++] = strtok(cmdline, " ");
    while(argv[argc++] = strtok(NULL, " "));
    --argc;
    if(argc == 0)
        return false;
    return true;
}

void debug(){
    printf("argv:");
    for(int i = 0; i < argc; ++i){
        printf("%s ", argv[i]);
    }
    printf("\n");
    printf("argc:%d\n", argc);

}

int run_command(){
    pid_t id = 0;
    id = fork();
    if(id == 0){
        //执行命令
        execvpe(argv[0], argv, my_env);
        //执行失败
        exit(1);
    }
    
    if(id){
        int status;
        pid_t chpid = wait(&status);
        if(chpid == -1){
           perror("wait failed"); 
           return 1;
        }
        if(WIFEXITED(status)){
            //子进程正常退出
            if(WEXITSTATUS(status) != 0){
                printf("指令执行失败！\n");
                return 2;
            }
            return 0;
        }
    }
}

bool check_builtin(){
    //每个内建命令都要单独判断
    if(strcmp(argv[0], "cd") == 0)
    {
        chdir(argv[1]);
        char cwd[1024];
        if(getcwd(cwd, sizeof(cwd)) != NULL){
            my_setenv("PWD", cwd);
        }else{
            perror("cd命令");
        }
        
        return true;
    }
    if(strcmp(argv[0], "exit") == 0)
    {
        exit(0);
        return true;
    }
    if(strcmp(argv[0], "myenv") == 0){
        for(int i = 0; i < my_env_num; ++i){
            printf("%s\n", my_env[i]);
        }
        return true;
    }
    if(strcmp(argv[0], "export") == 0)
    {
        //export不带参数就什么也不做
        if(argc <= 1)
            return true;
        char* argv_copy = (char*)malloc(strlen(argv[1]) + 1);
        strcpy(argv_copy, argv[1]);
        char* name = strtok(argv_copy, "=");
        char* value = strtok(NULL, "=");
        //增加的变量没有传值，无效
        if(value == NULL)
            return true;
        my_setenv(name, value);
        free(argv_copy);
        return true;
    }

    return false;
}

//初始化环境变量
void init_env(char** environ){
    //环境变量个数
    int count = 0;
    while(environ[count]){
        my_env[count] = (char*)malloc(strlen(environ[count]) + 1);
        strcpy(my_env[count], environ[count]);
        ++count;
    }
    my_env[count] = NULL;
    my_env_num = count;
}

int main(){
    init_env(environ);
    while(1){

        //命令行提示符
        print_prompt();

        //获取命令
        get_command();

        
        //解析命令
        if(parse_command() == false)
            continue;
        
        if(strncmp(cmdline, "debug", 5) == 0)
            debug();

        //执行命令
        //判断是否为内建指令，是则执行内建指令，不是则正常执行
        if(check_builtin()){

        }
        else{
            run_command();
        }
    }
    return 0;
}
