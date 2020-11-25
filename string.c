#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
MODULE_LICENSE("GPL");

static int hello_init(void)
{
	pr_info("sting: module loaded!\n");
	
	char str[20];
	strcpy(str, "hi, strcpy");
	pr_info("strcpy: %s\n", str);
	// ____________________________________________________________________
	char str1[10];
	strcat(str1, "Con");
	strcat(str1, "Cat");
	pr_info("strcat: %s\n", str1);
	// ____________________________________________________________________
	//int x;
	//x = strcasecmp("AAAA","aaa");
	//pr_info("strcmp: %d", x);
	// ____________________________________________________________________
	int point = 123;
	char *s = "123-";
	int out = get_option(&s, &point);
	pr_info("get_option %d\n", out);


	return 0;
}

static void hello_exit(void)
{
	pr_err("Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
