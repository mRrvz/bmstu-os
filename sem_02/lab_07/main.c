#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/time.h>

#define MNFS_MAGIC 0xBADBEEF
#define CACHE_SIZE 128
#define SLAB_NAME "mnfs_slab"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mikhail Nitenko");

struct mnfs_inode {
  int i_mode;
  unsigned long i_ino;
};

static struct kmem_cache *inode_cache = NULL;
static struct mnfs_inode **inode_pointers = NULL;
static int c_size = CACHE_SIZE;
static int cached_count = 0;

static struct mnfs_inode *c_get_inode(void) {
  if (cached_count == c_size) {
    return NULL;
  }

  inode_pointers[cached_count] = kmem_cache_alloc(inode_cache, GFP_KERNEL);

  return inode_pointers[cached_count++];
}

static struct inode *mnfs_make_inode(struct super_block *sb, int mode) {
  struct inode *ret = new_inode(sb);
  struct mnfs_inode *inode_cache = NULL;

  if (ret) {
    inode_init_owner(&init_user_ns, ret, NULL, mode);

    ret->i_size = PAGE_SIZE;
    ret->i_atime = ret->i_mtime = ret->i_ctime = current_time(ret);

    inode_cache = c_get_inode();

    if (inode_cache != NULL) {
      inode_cache->i_mode = ret->i_mode;
      inode_cache->i_ino = ret->i_ino;
    }

    ret->i_private = inode_cache;
  }

  return ret;
}

static void mnfs_put_super(struct super_block *sb) {
  printk(KERN_DEBUG "+ mnfs super block destroyed!\n");
}

static struct super_operations const mnfs_super_ops = {
    .put_super = mnfs_put_super,
    .statfs = simple_statfs,
    .drop_inode = generic_delete_inode,
};

static int mnfs_fill_sb(struct super_block *sb, void *data, int silent) {
  struct inode *root = NULL;
  sb->s_blocksize = PAGE_SIZE;
  sb->s_blocksize_bits = PAGE_SHIFT;
  sb->s_magic = MNFS_MAGIC;
  sb->s_op = &mnfs_super_ops;

  root = mnfs_make_inode(sb, S_IFDIR | 0755);

  if (!root) {
    printk(KERN_ERR "+ mnfs inode allocation failed !\n");
    return -ENOMEM;
  }

  root->i_op = &simple_dir_inode_operations;
  root->i_fop = &simple_dir_operations;
  sb->s_root = d_make_root(root);

  if (!sb->s_root) {
    printk(KERN_ERR "+ mnfs root creation failed !\n");
    iput(root);

    return -ENOMEM;
  }

  return 0;
}

static struct dentry *mnfs_mount(struct file_system_type *type, int flags,
                                 char const *dev, void *data) {
  struct dentry *const entry = mount_nodev(type, flags, data, mnfs_fill_sb);

  if (IS_ERR(entry))
    printk(KERN_ERR "+ mnfs mounting failed !\n");
  else
    printk(KERN_DEBUG "+ mnfs mounted!\n");

  return entry;
}

static struct file_system_type mnfs_type = {
    .owner = THIS_MODULE,
    .name = "mnfs",
    .mount = mnfs_mount,
    .kill_sb = kill_anon_super,
};

static int __init mnfs_init(void) {
  int ret = register_filesystem(&mnfs_type);

  if (ret != 0) {
    printk(KERN_ERR "+ mnfs_module cannot register filesystem!\n");

    return ret;
  }

  inode_pointers = kmalloc(sizeof(struct mnfs_inode *) * c_size, GFP_KERNEL);
  if (inode_pointers == NULL) {
    printk(KERN_ERR "+ mnfs kmalloc error\n");

    return -ENOMEM;
  }

  inode_cache =
      kmem_cache_create(SLAB_NAME, sizeof(struct mnfs_inode), 0, 0, NULL);
  if (inode_cache == NULL) {
    kfree(inode_pointers);
    printk(KERN_ERR "+ mnfs kmem_cache_create error\n");

    return -ENOMEM;
  }

  printk(KERN_DEBUG "+ mnfs_module loaded !\n");

  return 0;
}

static void __exit mnfs_exit(void) {
  int i;
  for (i = 0; i < cached_count; i++) {
    kmem_cache_free(inode_cache, inode_pointers[i]);
  }
  kmem_cache_destroy(inode_cache);
  kfree(inode_pointers);

  int ret = unregister_filesystem(&mnfs_type);

  if (ret != 0)
    printk(KERN_ERR "+ mnfs_module cannot unregister filesystem !\n");

  printk(KERN_DEBUG "+ mnfs_module unloaded !\n");
}

module_init(mnfs_init);
module_exit(mnfs_exit);

