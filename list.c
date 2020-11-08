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
	struct person_t *alex = kmalloc(sizeof(person_t), GFP_KERNEL);
	struct person_t *mark = kmalloc(sizeof(person_t), GFP_KERNEL);
	
	/* allocate memory for name field and fill every field */
	jack->name = "Jack";
	jack->age = 20;

	luke->name = "Luke";
	luke->age = 30;
	
	alex->name = "Alex";
	alex->age = 35;

	mark->name = "Mark";
	mark->age = 15;

	/* Initialize each node's list entry */
	INIT_LIST_HEAD(&jack->list);
	INIT_LIST_HEAD(&luke->list);
	INIT_LIST_HEAD(&alex->list);
	INIT_LIST_HEAD(&mark->list);
	
	
	/* add in the tail of list */
	list_add_tail(&jack->list, &person);
	/* add to the first element of list */
	list_add(&luke->list, &person);
	/* add element in between */
	__list_add(&alex->list, &luke->list, &jack->list);
	/* add element in between */
	list_replace(&jack->list, &mark->list);
	/* Take first element and move it to the last */
	list_rotate_left(&person);
	/* Get the fist entry */
	struct person_t *last = list_last_entry(&person, struct person_t, list);
	pr_info(": Last element is: %s\n", last->name);

	/* 
	 * Test APIs 
	 * */
	if(list_is_first(&luke->list, &person))
		pr_info(": Yes, This is the fist.\n");

	if(!list_empty(&person))
		pr_info(": List is Not empty.\n");

	if(!list_is_singular(&person))
		pr_info(": List have more than single elment.\n");


	// iterate over a list
	struct person_t *counter;
	int i = 0;
	pr_cont("iterate over a list");
	list_for_each_entry(counter, &person, list)
	{
        	pr_info("node: Person %d name: %s, age: %d \n", ++i, 
				counter->name, 
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
