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
#include "ui.h"

struct cdevsw ui_cdevsw =
{
    ui_open,
    ui_close,
    ui_read,
    ui_write,
    ui_ioctl,
    (dev_type_stop((*))) lkmenodev,
    (dev_type_tty((*))) lkmenodev,
    (dev_type_poll((*))) lkmenodev,
    (dev_type_mmap((*))) lkmenodev,
    0,
    0,
    NULL
};

MOD_DEV("ui_lkm", LM_DT_CHAR, -1, &ui_cdevsw); 

static int  ui_load(struct lkm_table *lkmt, int cmd)
{
    printf("ui_lkm loaded !\n");
    return (0);
}

static int  ui_unload(struct lkm_table *lkmt, int cmd)
{
    printf("ui_lkm unloaded !\n");
    return (0);
}

static int	ui_stat(struct lkm_table *lkmt, int cmd)
{
    printf("ui_lkm stated !\n");
    return (0);
}

int ui_lkm(struct lkm_table *lkmt, int cmd, int ver)
{
    DISPATCH(lkmt, cmd, ver, ui_load, ui_unload, ui_stat);
    return (0);
}
