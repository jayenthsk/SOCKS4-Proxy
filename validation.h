
char ver[20], cd[20], dest_ip[20], port[20], userid[20], nullbyt[20], res_code[20];
unsigned int ip;
int validate(char buffer[255])
{

    sscanf(buffer, "%s %s %s %s %s %s", ver, cd, port, dest_ip, userid, nullbyt);
    
    if (strlen(buffer) <= 9)
    {
        // Error code 91
        strcpy(res_code, "91");
        bzero(buffer, 1024);
        // nc testing sprintf \n
        sprintf(buffer, "%s %s %s %s\n", "0", res_code, port, dest_ip);

        return 0;
    }

    if (strcmp(ver, "4"))
    {
        // Error code 91 - version wrong
        strcpy(res_code, "91");
        bzero(buffer, 1024);
        sprintf(buffer, "%s %s %s %s\n", "0", res_code, port, dest_ip);
        return 0;
    }

    if (strcmp(cd, "1"))
    {
        // Error code 91 - not a CONNECT request (cd)
        strcpy(res_code, "91");
        bzero(buffer, 1024);
        sprintf(buffer, "%s %s %s %s\n", "0", res_code, port, dest_ip);
        return 0;
    }

    // Ident protocol:
    FILE *fp = fopen("config.txt", "r");

    if(fp == NULL){
        strcpy(res_code, "92");
        bzero(buffer, 1024);
        sprintf(buffer, "%s %s %s %s\n", "0", res_code, port, dest_ip);
        return 0;
    }

    char line[255], cip[22], cport[22], cuser_id[22];
    int validate_flag = 0;

    while (fgets(line, sizeof(line), fp))
    {

        sscanf(line, "%s %s %s", cip, cport, cuser_id);

        if (((int)inet_addr(cip) == ip) && !strcmp(cport, port) && !strcmp(cuser_id, userid))
        {
            // ip did not match
            validate_flag = 1;
        }
    }

    if (!validate_flag)
    {
        strcpy(res_code, "93");
        bzero(buffer, 1024);
        sprintf(buffer, "%s %s %s %s\n", "0", res_code, port, dest_ip);
        return 0;
    }

    strcpy(res_code, "90");
    bzero(buffer, 1024);
    sprintf(buffer, "%s %s %s %s\n", "0", res_code, port, dest_ip);
    return 1;
}