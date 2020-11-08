#include <linux/module.h>
MODULE_LICENSE("GPL");

struct person {
	int age;
	int salary;
	char *name;
} P;

struct family {
	struct person *father;
	struct person *mother;
	int number_of_sons;
	int family_id;
} F;

static int hello_init(void)
{
	F.father = &P;
	// Fill and initialise P somewhere
	F.father->age = 40;
	
	// Print: before modification
	pr_cont("Before:");
	pr_info("Age: %d\n", F.father->age);

	// pointer to a field of the structure
	// (could be any (non-pointer) member in the structure)
	struct person **dad = &F.father;

	// now let us retrieve back its person
	struct family * fam;
	fam = container_of(dad, struct family, father);

	// Now i have a pointer my new struct
	fam->father->age = 50;
	
	// Print & Test
	pr_cont("After:");
	pr_info("Age: %d\n", F.father->age);
	pr_info("Age: %d\n", fam->father->age);
	
	return 0;
}

static void hello_exit(void)
{
	pr_err("Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
