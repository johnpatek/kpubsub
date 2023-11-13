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

#include <linux/fs.h>
#include <linux/fs_context.h>
#include <linux/file.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mount.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/mutex.h>
#include <linux/syscalls.h>
#include <linux/version.h>

struct kps_filesystem
{
    struct file_system_type *file_system;
    const struct inode_operations *inode_ops;
    const struct super_operations *super_ops;
    const struct fs_context_operations *fs_context_ops;

    struct spinlock lock;
    struct kmem_cache *cache;
    struct vfsmount *mount;
};

struct kps_filesystem *kps_filesystem_new(
    struct file_system_type *file_system,
    const struct inode_operations *inode_ops,
    const struct super_operations *super_ops,
    const struct fs_context_operations *fs_context_ops);

void kps_filesystem_free(struct kps_filesystem *filesystem);

int kps_filesystem_mount(struct kps_filesystem *filesystem);

int kps_filesystem_unmount(struct kps_filesystem *filesystem);

int kps_filesystem_init_fs_context(
    struct kps_filesystem *filesystem, 
    struct fs_context *fc);

int kps_filesystem_create_inode(
    struct kps_filesystem *filesystem, 
    struct inode *dir, 
    struct dentry *dentry, 
    umode_t mode, 
    bool excl);

int kps_filesystem_unlink_inode(
    struct kps_filesystem *filesystem,
    struct inode *dir, 
    struct dentry *dentry);

int kps_filesystem_alloc_inode(
    struct kps_filesystem *filesystem,
    struct super_block *sb);

int kps_filesystem_free_inode(
    struct kps_filesystem *filesystem,
    struct inode *inode);

int kps_filesystem_evict_inode(
    struct kps_filesystem *filesystem,
    struct inode *inode);

int kps_filesystem_get_tree(
    struct kps_filesystem *filesystem, 
    struct fs_context *fc);

#endif