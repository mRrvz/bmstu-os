#include <linux/init.h>
#include <linux/module.h>

#include "md.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Romanov Alexey");

static int __init md_init(void) {
    printk("Module md2 was loaded!\n");
    printk("Value exported from md1 : %s\n", md1_data);
    printk("Value returned md1_proc() is : %s\n", md1_proc());
    //printk("Value from static md1_local() is: %s\n", md1_local());

    return 0;
}

static void __exit md_exit(void) { printk("Module md2 unloaded!\n"); }

module_init(md_init);
module_exit(md_exit);
