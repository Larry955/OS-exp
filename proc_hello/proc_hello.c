/*
 * OS-exp3 Implementation
 *
 * Add a directory named hello under proc, 
 * and a file named world under hello, whose content is "Hello World"
 *
 * Copyright (C) 2019 Larry <https://github.com/Larry955/OS-exp.git>
 *
 * Released under the GPL.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include<linux/sched.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>

static struct proc_dir_entry *proc_parent;


int len,temp;
char *msg;

static ssize_t read_proc(struct file *filp,char __user *buf,size_t count,loff_t *offp ) 
{
    if(count>temp)
        count=temp;
    temp=temp-count;

    raw_copy_to_user(buf,msg, count);
    if(count==0)
        temp=len;   
    return count;
}


static const struct file_operations proc_fops = { 
    .read       =  read_proc
};

void create_new_proc_entry(void)
{
	/*create a new directory named hello, and return a pointer point to this dir*/
    proc_parent = proc_mkdir("hello",NULL);
    if(!proc_parent)
    {
        printk(KERN_INFO "Error creating proc entry");
    }
    /*create a file named world, add read attribute to this file using proc_fops*/
    proc_create("world",0,proc_parent,&proc_fops);
    msg="hello world\n";	/*file content*/
    len=strlen(msg);
    temp=len;
    printk(KERN_INFO "1.len=%d",len);
    printk(KERN_INFO "proc initialized");
}


int proc_init (void)
{
    create_new_proc_entry();
    return 0;
}

void proc_cleanup(void)
{
    printk(KERN_INFO " Inside cleanup_module\n");
    remove_proc_entry("hello",proc_parent);
    remove_proc_entry("world",NULL);
}

MODULE_LICENSE("GPL");
module_init(proc_init);
module_exit(proc_cleanup);
