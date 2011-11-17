#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xec5ba0f9, "module_layout" },
	{ 0x87d59b1, "cdev_del" },
	{ 0x28849590, "kmalloc_caches" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x2e26db68, "cdev_init" },
	{ 0x4661e311, "__tracepoint_kmalloc" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xb72397d5, "printk" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0xcb8d5cbb, "cdev_add" },
	{ 0x142c23f8, "kmem_cache_alloc" },
	{ 0x37a0cba, "kfree" },
	{ 0xd6c963c, "copy_from_user" },
	{ 0x29537c9e, "alloc_chrdev_region" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "7D4DA8B450331F5F0ED04C3");
