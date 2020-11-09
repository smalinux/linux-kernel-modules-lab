#include <linux/hashtable.h> // hashtable API
#include <linux/module.h>    // module_{init,exit}, MODULE_*
#include <linux/string.h>    // strcpy, strcmp
#include <linux/types.h>     // u32 etc.
MODULE_LICENSE("GPL");

#define MAX 32

struct h_node {
	int data;
	char name[MAX];
	struct hlist_node node;
};

DECLARE_HASHTABLE(tbl, 3);

// Just to demonstrate the behavior when two keys are equal.
static u32 myhash(const char *s) {
	u32 key = 0;
	char c;

	pr_cont("Calculating the key of %s....", s);

	while ((c = *s++)) {
		key += c;
		pr_err("key = %u\n", key);
	}

	return key;
}

static int __init myhashtable_init(void)
{
	struct h_node a, b, c, *cur;
	u32 key_a, key_b, key_c;
	unsigned bkt;

	pr_info("myhashtable: module loaded\n");

	a.data = 3;
	strcpy(a.name, "foo");

	b.data = 7;
	strcpy(b.name, "oof");

	c.data = 3;
	strcpy(c.name, "ofo");

	/* Calculate key for each element.
	* Since the above hash function only sums the characters, we will
	* end up having two identical keys here.
	*/
	key_a = myhash(a.name);
	key_b = myhash(b.name);
	key_c = myhash(c.name);

	pr_info("myhashtable: key_a = %u, key_b = %u\n", key_a, key_b);

	// Initialize the hashtable.
	hash_init(tbl);

	// Insert the elements.
	hash_add(tbl, &a.node, key_a);
	hash_add(tbl, &b.node, key_b);
	hash_add(tbl, &c.node, key_c);

	// List all elements in the table.
	pr_cont("List all elements in the table");
	hash_for_each(tbl, bkt, cur, node) {
        	pr_info("myhashtable: element: data = %d, name = %s\n",
				cur->data, cur->name);
	}

	// Get the element with name = "foo".
	pr_cont("Get the element with name = \"foo\"");
	hash_for_each_possible(tbl, cur, node, key_a) {
		pr_info("myhashtable: match for key %u: data = %d, name = %s\n",
				key_a, cur->data, cur->name);

		// Check the name.
		if (!strcmp(cur->name, "foo")) {
			pr_info("myhashtable: element named \"foo\" found!\n");
			break;
		}
	}

	// Remove elements.
	hash_del(&a.node);
	hash_del(&b.node);

	return 0;
}

static void __exit myhashtable_exit(void)
{
	// Do nothing (needed to be able to unload the module).
	pr_info("myhashtable: module unloaded\n");
}


module_init(myhashtable_init);
module_exit(myhashtable_exit);
MODULE_VERSION("0.1");
MODULE_DESCRIPTION("Silly kernel hashtable API example module.");
MODULE_AUTHOR("Sohaib Mhmd");
/**
 * Link:
 * https://stackoverflow.com/a/60873789/5688267
 */
