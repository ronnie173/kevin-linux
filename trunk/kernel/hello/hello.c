#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void) {
	printk(KERN_ALERT "Hello, world\n");
	
	/*task_struct *cur;
	cur = get_current();*/
	printk(KERN_INFO "The process is [%s]\n",
			current->comm);
	/*printk(KERN_INFO "The pid is %d\n",
			current->pid);*/
	return 0;
}

static void hello_exit(void) {
	printk(KERN_ALERT "Goodbye, world\n");
}

module_init(hello_init);
module_exit(hello_exit);
