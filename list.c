#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include<linux/slab.h>                 //kmalloc()
#include <linux/list.h>
MODULE_LICENSE("GPL");

struct person_t {
	char * name;
	int age;
	struct list_head list; 		/* kernel's list structure */
} person_t;

/* Static allocate list head */
LIST_HEAD(person);

static int hello_init(void)
{
	struct person_t *jack = kmalloc(sizeof(person_t), GFP_KERNEL);
	struct person_t *luke = kmalloc(sizeof(person_t), GFP_KERNEL);
	
	/* allocate memory for name field and fill every field */
	jack->name = "Jack";
	jack->age = 20;

	luke->name = "Luke";
	luke->age = 30;

	/* Initialize each node's list entry */
	INIT_LIST_HEAD(&jack->list);
	INIT_LIST_HEAD(&luke->list);
	
	list_add_tail(&jack->list, &person);
	list_add_tail(&luke->list, &person);

	// iterate over a list
	struct person_t *counter;
	int i = 0;
	pr_cont("iterate over a list");
	list_for_each_entry(counter, &person, list)
	{
        	pr_info("node: Person %d name: %s, age: %d \n", ++i, counter->name, 
				counter->age);
    	}
	
	return 0;
}

static void hello_exit(void)
{
	pr_err("Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
