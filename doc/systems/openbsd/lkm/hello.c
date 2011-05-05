#include <sys/types.h>
#include <sys/systm.h>
#include <sys/conf.h>
#include <sys/exec.h>
#include <sys/lkm.h>
#include <sys/errno.h>
#include <stdio.h>


int point_handler(struct lkm_table *l, int cmd)
{
    switch (cmd)
    {
        case LKM_E_LOAD :
            printf("Hello World!\n");
            break;
        case LKM_E_UNLOAD :
            printf("Bye!!\n");
            break;
        case LKM_E_STAT :
            printf("Stat!!\n");
            break;
        default :
            break;
    }
    return (0);
}

MOD_MISC("my_mod");

my_mod( struct lkm_table *lkmtp, int cmd, int ver)
{
    DISPATCH(lkmtp, cmd, ver, point_handler, point_handler, point_handler)
}
