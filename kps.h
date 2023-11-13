/*
kpubsub: publisher/subscriber IPC for Linux
Copyright (C) 2023  John R Patek Sr

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef KPS_H
#define KPS_H

#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

struct kps_attr
{
    size_t kps_flags;   // default: 0
    size_t kps_maxmsg;  // default: 10
    size_t kps_msgsize; // default: 1024
};

inline int kps_open(const char * path, int oflags, const struct kps_attr *attr)
{
    return syscall(3,"kps_open");
}

int kps_close(int fd);

#endif