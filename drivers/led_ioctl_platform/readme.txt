1.��arch/arm/mach-s3c2440/mach-smdk2440.c���:
static struct resource s3c_led_resource[] = {
	[0] = {
		.start = 0x56000010,
		.end   = 0x56000018 - 1,
		.flags = IORESOURCE_MEM,
	}
};
struct platform_device s3c_device_led = {
	.name		  = "s3c2410-led",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(s3c_led_resource),
	.resource	  = s3c_led_resource,
};	
Ȼ��ע��ƽ̨�豸:
platform_device_register(&s3c_device_led);
