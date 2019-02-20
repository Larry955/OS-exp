/*
 * OS-exp4 Implementation
 *
 * Program a module to create a subdirectory in sys/fs called 
 * /sys/kernel/hello, and a file named world under this directory, 
 * whose content is "hello world".
 *
 * Copyright (C) 2004-2007 Greg Kroah-Hartman <greg@kroah.com>
 * Copyright (C) 2019 Larry <https://github.com/Larry955/OS-exp.git>.
 *
 * Released under the GPL.
 *
 */

#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

static int world;

/*
 * The "world" file whose content is "hello world"
 */
static ssize_t show_world(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	return sprintf(buf, "hello world\n");
}

static ssize_t store_world(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	sscanf(buf, "%du", &world);
	return count;
}

static struct kobj_attribute world_attribute = 
{
	.attr = {
		.name = "world",
		.mode = S_IWUSR | S_IRUGO,
	},
	.show = show_world,
	.store = store_world,
};


/*
 * Create a group of attributes so that we can create and destory them all
 * at once.
 */
static struct attribute *attrs[] = {
	&world_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};

/*
 * An unnamed attribute group will put all of the attributes directly in
 * the kobject directory.  If we specify a name, a subdirectory will be
 * created for the attributes with the directory being the name of the
 * attribute group.
 */
static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *hello_kobj;

static int hello_init(void)
{
	int retval;

	/*
	 * Create a simple kobject with the name of "hello",
	 * located under /sys/fs/
	 */
	hello_kobj = kobject_create_and_add("hello", fs_kobj);
	if (!hello_kobj)
		return -ENOMEM;

	/* Create the files associated with this kobject */
	retval = sysfs_create_group(hello_kobj, &attr_group);
	if (retval)
		kobject_put(hello_kobj);

	return retval;
}

static void hello_exit(void)
{
	kobject_del(hello_kobj);
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");

