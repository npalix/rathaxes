/*
**      Copyright (c) 2008, Amsallem David <filth@rathaxes.org>
**
**      Permission to use, copy, modify, and/or distribute this software for any
**      purpose with or without fee is hereby granted, provided that the above
**      copyright notice and this permission notice appear in all copies.
**
**      THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
**      WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
**      MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
**      ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
**      WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
**      ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
**      OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <sys/param.h>
#include <sys/conf.h>
#include <sys/exec.h>
#include <sys/systm.h>
#include <sys/lkm.h>
#include <sys/tty.h>
#include <sys/ttycom.h>
#include "rs232.h"

struct cdevsw rs232_cdevsw =
{
    /* open */      rs232_open,
    /* close */     rs232_close,
    /* read */      rs232_read,
    /* write */     rs232_write,
    /* ioctl */     rs232_ioctl,
    /* stop */      (dev_type_stop((*))) lkmenodev,
    /* tty */       (dev_type_tty((*))) lkmenodev,
    /* poll */      (dev_type_poll((*))) lkmenodev,
    /* mmap */      (dev_type_mmap((*))) lkmenodev,
    0,
    //    /* type */      D_TTY,
    0,
    //    /* flags */     D_KQFILTER,
    NULL
    //    /* KQFILTER*/	ttkqfilter
};

MOD_DEV("rs232_lkm", LM_DT_CHAR, -1, &rs232_cdevsw); 

static int  rs232_load(struct lkm_table *lkmt, int cmd)
{
    printf("rs232_lkm loaded !\n");
    printf("come on dude, ready to panic ?\n");
    return (0);
}

static int  rs232_unload(struct lkm_table *lkmt, int cmd)
{
    printf("Get a haircut and get a real job !\n");
    printf("rs232_lkm unloaded !\n");
    return (0);
}

static int	rs232_stat(struct lkm_table *lkmt, int cmd)
{
    printf("Wanna see more stuff ? ;)\n");
    printf("rs232_lkm stated !\n");
    return (0);
}

int rs232_lkm(struct lkm_table *lkmt, int cmd, int ver)
{
    DISPATCH(lkmt, cmd, ver, rs232_load, rs232_unload, rs232_stat)
}
