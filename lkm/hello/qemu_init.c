#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/syscall.h> /* Definition of SYS_* constants */
#include <linux/reboot.h> /* Definition of LINUX_REBOOT_* constants */
#include <linux/kdev_t.h> /* MKDEV */

// int syscall(SYS_finit_module, int fd, const char *param_values, int flags);

void init_rootfs(void)
{
	mkdir("/dev", 0755);
	mkdir("/root", 0700);
	mkdir("/sys", 0755);
	mkdir("/proc", 0755);
	mkdir("/tmp", 0755);
	mkdir("/mnt", 0755);

	mount("proc", "/proc", "proc", 0, NULL);
	mount("sysfs", "/sys", "sysfs", 0, NULL);

	mknod("/dev/console", 0666 | S_IFCHR, MKDEV(5, 1));
	mknod("/dev/null", 0666 | S_IFCHR, MKDEV(1, 3));
}

static void poweroff(void)
{
	syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2,
		LINUX_REBOOT_CMD_POWER_OFF, NULL);
}

static void run_test(void)
{
	int fd = open("/hello.ko", O_RDONLY);
	if (fd < 0) {
		printf("load resgaurd failed\n");
		return;
	}

	syscall(SYS_finit_module, fd, "run_ktest=y", 0);
	close(fd);
}

void main()
{
	init_rootfs();
	run_test();
	poweroff();
}
