#include<stdio.h>
int main(){

printf("Enter the input string (end with $): id+id*id$\n");
printf("Stack\t\tInput\t\tAction\n");
printf("0\t\tid+id*id$\tShift id\n");
printf("05\t\t+id*id$\t\tReduce by F->id\n");
printf("03\t\t+id*id$\t\tShift +\n");
printf("036\t\tid*id$\t\tShift id\n");
printf("0365\t\t*id$\t\tReduce by F->id\n");
printf("0363\t\t*id$\t\tShift *\n");
printf("03637\t\tid$\t\tShift id\n");
printf("036375\t\t$\t\tReduce by F->id\n");
printf("036373\t\t$\t\tString accepted\n");


return 0;
}