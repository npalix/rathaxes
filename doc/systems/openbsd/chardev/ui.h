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
#ifndef UI_H_
# define UI_H_

int     ui_open(dev_t dev, int oflags, int devtype, struct proc *p);
int     ui_close(dev_t dev, int fflag, int devtype, struct proc *p);
int     ui_read(dev_t dev, struct uio *uio, int ioflag);
int     ui_write(dev_t dev, struct uio *uio, int ioflag);
int     ui_ioctl(dev_t dev, u_long cmd, caddr_t data, int fflag, struct proc *p);

#endif
