void main(int argc,char **argv)

{
  int iVar1;
  
  if (argc == 1) {
    puts("Hello user! Pass me a -h to learn what I can do!");
  }
  else {
    iVar1 = strcmp(argv[1],"-h");
    if (iVar1 == 0) {
      puts(
          "Oh, help? I actually don\'t do much, but I do have this flag here: picoCTF{b1scu1ts_4nd_g r4vy_755f3544}"
          );
    }
    else {
      printf("I don\'t know what \'%s\' means! I do know what -h means though!\n",argv[1]);
    }
  }
  return;
}
