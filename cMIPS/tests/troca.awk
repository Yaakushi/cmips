//{ a=($1 && 0xF0000000) / 268435456;
    b=($1 && 0x0F000000) / 16777216;
    c=($1 && 0x00F00000) / 1048576;
    d=($1 && 0x000F0000) / 65536;
    e=($1 && 0x0000F000) / 4096; 
    f=($1 && 0x00000F00) / 256;
    g=($1 && 0x000000F0) / 16;
    h=($1 && 0x0000000F);
    printf("%x%x%x%x%x%x%x%x\n",a,b,c,d,e,f,g,h);
}
