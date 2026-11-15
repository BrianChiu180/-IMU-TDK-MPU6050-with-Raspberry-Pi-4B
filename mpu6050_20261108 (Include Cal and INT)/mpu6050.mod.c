#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x79f52993, "i2c_register_driver" },
	{ 0xf6d47c40, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x24021414, "device_destroy" },
	{ 0xf57a62f1, "class_destroy" },
	{ 0x0f3b5a8c, "i2c_del_driver" },
	{ 0xdcb764ad, "memset" },
	{ 0xe34121c1, "i2c_transfer" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x92997ed8, "_printk" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x3fd78f3b, "register_chrdev_region" },
	{ 0x83d9de5c, "cdev_init" },
	{ 0xb2730d06, "cdev_add" },
	{ 0x6c66ac6d, "class_create" },
	{ 0x434998eb, "device_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x150028cc, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cdar,mpu6050");
MODULE_ALIAS("of:N*T*Cdar,mpu6050C*");
MODULE_ALIAS("of:N*T*Cinvensense,mpu6050");
MODULE_ALIAS("of:N*T*Cinvensense,mpu6050C*");

MODULE_INFO(srcversion, "CDE9FA0D21B918FB08E93E7");
