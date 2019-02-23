// // This file is for testing the interpreter without an arduino. Keep commented if not using

// #include "../include/shell/ubasic.h"

// #include <string.h>
// #include <stdio.h>



// void cutstr(char *str, size_t n){
//     if(n != 0 && str != 0){
//         size_t len = strlen(str);
//         if (n > len)
//             return;  // Or: n = len;
//         memmove(str, str+n, len - n + 1);
//     }
// }

// int main(){
//     char program[1000];
//     char buffer[80];
//     char *b = buffer;
//     size_t bufferSize = 80;
//     size_t lineSize;
    
//     for(;;){
//         lineSize = getline(&b, &bufferSize, stdin);
//         // char line[50];
        
//         strcat(program, &buffer[strlen(buffer) - lineSize]);
        
//         if(strncmp(&program[strlen(program) - lineSize], "run\n", 3)==0){
//             cutstr(program, 6);
//             int len = strlen(program);
//             program[len-4] = 0;

//             ubasic_init(program);

//             do {
//                 ubasic_run();
//             } while(!ubasic_finished());
//         }
//     }
//     return 0;
// }