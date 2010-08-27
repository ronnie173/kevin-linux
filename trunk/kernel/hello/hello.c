#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
MODULE_LICENSE("Dual BSD/GPL");

static char *whom = "world";
static int howmany = 1;
module_param(howmany, int, S_IRUGO);
module_param(whom, charp, S_IRUGO);

static int hello_init(void) {
	int i;
	for (i = 0; i < howmany; i++) {
		printk(KERN_ALERT "(%d) Hello, %s\n", i, whom);
	}
	
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
