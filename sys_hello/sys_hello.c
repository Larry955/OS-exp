/*
 * OS-exp4 Implementation
 *
 * Program a module to create a subdirectory in /sys called 
 * /sys/hello, and a file named world under this directory, 
 * whose content is "hello world".
 *
 * Copyright (C) 2019 Larry <https://github.com/Larry955/OS-exp.git>.
 *
 * Released under the GPL.
 *
 */


#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/slab.h>


/*
 * The "world" file whose content is "hello world"
 */
ssize_t show_world(struct kobject *object, struct attribute *attr, char *buf) 
{
	return sprintf(buf, "hello world\n");
}

struct sysfs_ops world_ops = {
	.show = show_world
};

struct attribute world_attrs = {
	.name = "world",
	.mode = S_IWUSR | S_IRUGO
};

struct attribute *world_attrs_def[] = {
	&world_attrs,
	NULL
};

void obj_release(struct kobject *kobj) 
{
	printk("release object!\n");
	kfree(kobj);
}

struct kobj_type my_type = {
	.release = obj_release,
	.sysfs_ops = &world_ops,
	.default_attrs = world_attrs_def
};

static struct kobject *kobj;
int dir = -1;

int __init sysfs_init(void) 
{
	kobj = kmalloc(sizeof(struct kobject), GFP_KERNEL);
	if (kobj == NULL) 
	{
		printk(KERN_ALERT "create :fail!\n");
		return -1;
	}

	memset(kobj, 0, sizeof(struct kobject));
	kobj->ktype = &my_type;
	kobj->parent=NULL;

	/*
	 * Create a simple kobject with the name of "hello",
	 * located under /sys
	 */
	dir = kobject_init_and_add(kobj, &my_type, NULL, "hello");
	if (dir!=0) 
	{
		printk(KERN_ALERT "create direction hello :fail!\n");
		printk(KERN_ALERT "create file world :fail!\n");
		return -1;
	}
	printk(KERN_ALERT "create direction hello :success!\n");
	printk(KERN_ALERT "create file world :success!\n");
	return 0;
}

void __exit sysfs_exit(void) 
{
	kobject_del(kobj);
	printk(KERN_ALERT "delete direction hello and file world :success!\n");
}

module_init(sysfs_init);
module_exit(sysfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LARRY");
