#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shadow.h>
#include <string.h>
#include <crypt.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage...\n");
        exit(0);
    }

    char *input_pass;
    char *crypted_pass;
    struct spwd *shadowline;

    input_pass = getpass("PassWord:");
    shadowline = getspnam(argv[1]);
    crypted_pass = crypt(input_pass, shadowline->sp_pwdp);

    if (strcmp(crypted_pass, shadowline->sp_pwdp) == 0)
        puts("ok!\n");
    else
        puts("fails\n");

    exit(0);
}