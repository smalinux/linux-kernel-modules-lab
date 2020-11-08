/*
 *
 * Now we will assume that we have three containers. Container-1 has gold, 
 * Container-2 has diamond and Container-3 has filled with Water (Probably 
 * water is the most precious than gold and diamond). 
 * One guy come and ask you that which 
 * container contains Water (Container of water). I need it desperately. 
 * You would say, Container-3 has water.
 *
 * So in this example, that guy knows that one container contains water, 
 * but don’t know which container it is. 
 * He was using the member name (water) to know the container name or 
 * container address. So this is the use of Container_of macro in Linux kernel.
 *
 * */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
MODULE_LICENSE("GPL");

struct person {
	int age;
	int salary;
	char *name;
} P;
static int hello_init(void)
{
	// Fill and initialise P somewhere
	P.age = 26;
	P.salary = 20000;
	P.name = "Sohaib";
	// Print: before modification
	pr_cont("Before:");
	pr_info("Name: %s, Age: %d, Salary: %d\n", P.name, P.age, P.salary );

	// pointer to a field of the structure
	// (could be any (non-pointer) member in the structure)
	int * age_ptr = &P.age;

	// now let us retrieve back its person
	struct person * the_person;
	the_person = container_of(age_ptr, struct person, age);

	// Now i have a pointer my new struct
	the_person->age = 30;
	the_person->salary = 10000;
	the_person->name = "Jack";
	
	// Print & Test
	pr_cont("After:");
	pr_info("Name: %s, Age: %d, Salary: %d\n", P.name, P.age, P.salary );
	pr_info("Name: %s, Age: %d, Salary: %d\n", the_person->name, the_person->age, the_person->salary );
	
	return 0;
}

static void hello_exit(void)
{
	pr_err("Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
