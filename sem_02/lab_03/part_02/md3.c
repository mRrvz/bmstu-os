#include <linux/init.h>
#include <linux/module.h>

#include "md.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Romanov Alexey");

static int __init md_init(void) {
    printk("Module md3 was loaded!\n");
    printk("Value exported from md1 : %s\n", md1_data);
    printk("Value returned md1_proc() is : %s\n", md1_proc());

    return -1;
}

module_init(md_init);
