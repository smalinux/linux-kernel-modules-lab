#include <linux/hashtable.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/slab.h>		// kmalloc()
MODULE_LICENSE("GPL");

#define LENGTH 11

typedef struct num_node {
	char data[LENGTH];
	struct hlist_node node;
} num_node;

DECLARE_HASHTABLE(tbl, 3);

// Just to demonstrate the behavior when two keys are equal.
static u32 myhash(const char *s)
{
	u32 key = 0;
	char c;

	//pr_err("Calculating the key of %s....", s);

	while ((c = *s++)) {
		key += c;
		//pr_err("key = %u\n", key);
	}

	return key;
}

static u32 add_num(char * num)
{
	u32 key;
	struct num_node *a = kmalloc(sizeof(num_node), GFP_KERNEL);
	strcpy(a->data, num);
	key = myhash(a->data);
	hash_add(tbl, &a->node, key);
	pr_err("Phone num %s added!\n", a->data);
	return key;
};

static int __init myhashtable_init(void)
{
	struct num_node *cur;
	unsigned bkt;
	u32 key;

	pr_info("myhashtable: module loaded\n");

	// Initialize the hashtable.
	hash_init(tbl);

	key = add_num("01157133430");
	add_num("01157133403");
	add_num("01157134303");
	add_num("01157122403");
	add_num("01157166403");


	add_num("01111111111");
	add_num("01122222222");
	add_num("01133333333");
	add_num("01144444444");
	add_num("01155555555");

	// List all elements in the table.
	pr_cont("List all elements in the table");
	hash_for_each(tbl, bkt, cur, node) {
        	pr_info("myhashtable: Phone num = %s\n", 
				cur->data);
	}

	// Get the element = "01157133430".
	pr_cont("Get the phone number = \"01157133430\"");
	hash_for_each_possible(tbl, cur, node, key) {
		pr_info("myhashtable: match for key %u: Phone = %s\n",
				key, cur->data);

		// Check the value
		if (!strcmp(cur->data, "01157133430")) {
			pr_err("myhashtable: \"01157133430\" found!\n");
			break;
		}
	}

	return 0;
}

static void myhashtable_exit(void)
{

	struct num_node *cur;
	unsigned bkt;

	// Remove elements.
	hash_for_each(tbl, bkt, cur, node) {
		hash_del_rcu(&cur->node);
	}

	hash_for_each(tbl, bkt, cur, node) {
        	pr_info("myhashtable: Phone num = %s\n", 
				cur->data);
	}

	pr_info("myhashtable: module unloaded\n");
}

module_init(myhashtable_init);
module_exit(myhashtable_exit);
