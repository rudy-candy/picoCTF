#include<stdio.h>

int main(long param_1,long param_2)

{
  int iVar1;
  int iVar2;
  unsigned long uVar3;
  
  iVar1 = atoi(*(char **)(param_2 + 8));
  iVar2 = atoi(*(char **)(param_2 + 0x10));
  uVar3 = func1(iVar1,iVar2);
  printf("Result: %ld\n",uVar3 & 0xffffffff);
  return 0;
}
