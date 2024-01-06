#define __LIBRARY__
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <asm/segment.h>

char who[24];
int len = 0;

int sys_iam(const char * name)
{
    int i = 0;
    do
    {
        who[i] = get_fs_byte(name + i);
        if(i == 23 && who[23] != '\0')
            return -(EINVAL);
    }while(who[i++] != '\0');
    len = i - 1;
    return len;
}


int sys_whoami(char* name, unsigned int size)
{
    if(!len)//sys_iam haven't been called
        return 0;
    if(size < len)
        return -(EINVAL);

    int i;
    for(i = 0; i < len; i++)
        put_fs_byte(who[i], name + i);

    return len;
}
