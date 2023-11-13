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

#include "kps.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John R Patek Sr <johnpatek2@gmail.com>");
MODULE_DESCRIPTION("Linux pub/sub IPC");
MODULE_VERSION("0.0.0");

static struct file_system_type _file_system_type;
static const struct inode_operations _inode_operations;
// static const struct file_operations _file_operations;
static const struct super_operations _super_operations;
static const struct fs_context_operations _fs_context_operations;

struct kps_filesystem *filesystem;

// file_system_type
static int init_fs_context(struct fs_context *fc)
{
    return kps_filesystem_init_fs_context(filesystem, fc);
}

// inode_operations
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 2, 0)
static int create_inode(struct mnt_idmap *idmap, struct inode *dir, struct dentry *dentry, umode_t mode, bool excl)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(5, 12, 0)
static int create_inode(struct user_namespace *ns, struct inode *dir, struct dentry *dentry, umode_t mode, bool excl)
#else
static int create_inode(struct inode *dir, struct dentry *dentry, umode_t mode, bool excl)
#endif
{
    return kps_filesystem_create_inode(filesystem, dir, dentry, mode, excl);
}

static int unlink_inode(struct inode *dir, struct dentry *dentry)
{
    return kps_filesystem_unlink_inode(filesystem, dir, dentry);
}

// super_operations
static int alloc_inode(struct super_block *sb)
{
    return kps_filesystem_alloc_inode(filesystem, sb);
}

static int free_inode(struct inode *inode)
{
    return kps_filesystem_free_inode(filesystem, inode);
}

static int evict_inode(struct inode *inode)
{
    return kps_filesystem_evict_inode(filesystem, inode);
}

// fs_context_operations
static int fs_context_free(struct fs_context *fc)
{

}

static int fs_context_get_tree(struct fs_context *fc)
{
    
}

static struct file_system_type _file_system_type = {
    .name = "kpubsub",
    .init_fs_context = init_fs_context,
    .kill_sb = kill_litter_super,
    .fs_flags = FS_USERNS_MOUNT,
};

static const struct inode_operations _inode_operations = {
    .lookup = simple_lookup,
    .create = create_inode,
    .unlink = unlink_inode,
};

static const struct super_operations _super_operations = {
    .alloc_inode = alloc_inode,
    .free_inode = free_inode,
    .evict_inode = evict_inode,
};

static const struct fs_context_operations _fs_context_operations = {
    .free = fs_context_free,
    .get_tree = fs_context_get_tree,
};

static int __init kps_init(void)
{
    int ret;
    pr_info("kps_init: initializing kpubsub\n");
    filesystem = kps_filesystem_new(
        &_file_system_type,
        &_inode_operations,
        &_super_operations,
        &_fs_context_operations);

    if (unlikely(!filesystem))
    {
        pr_err("kps_init: failed to create filesystem\n");
        ret = 1;
        goto error;
    }

    ret = kps_filesystem_mount(filesystem);
    if (unlikely(ret))
    {
        pr_err("kps_init: failed to mount filesystem");
        goto error;
    }

error:
    if (filesystem)
    {
        kps_filesystem_free(filesystem);
    }
done:
    return ret;
}

static void __exit kps_exit(void)
{
    pr_info("kps_exit: exiting kpubsub\n");
}

module_init(kps_init);
module_exit(kps_exit);