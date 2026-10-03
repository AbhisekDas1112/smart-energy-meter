#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

#define DEVICE_NAME "smart_meter"
#define CLASS_NAME "smartmeter"

static dev_t device_number;
static struct cdev smart_meter_cdev;
static struct class *smart_meter_class;
static struct device *smart_meter_device;

static unsigned long pulse_count = 0;
static DEFINE_MUTEX(meter_mutex);

/* Open device */
static int smart_meter_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "smart_meter: device opened\n");
    return 0;
}

/* Close device */
static int smart_meter_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "smart_meter: device closed\n");
    return 0;
}

/* Read pulse count */
static ssize_t smart_meter_read(
    struct file *file,
    char __user *buffer,
    size_t length,
    loff_t *offset)
{
    char data[100];
    int data_length;

    mutex_lock(&meter_mutex);

    data_length = snprintf(
        data,
        sizeof(data),
        "%lu\n",
        pulse_count
    );

    mutex_unlock(&meter_mutex);

    if (*offset >= data_length)
        return 0;

    if (copy_to_user(buffer, data, data_length))
        return -EFAULT;

    *offset += data_length;

    return data_length;
}

/* Write pulse count */
static ssize_t smart_meter_write(
    struct file *file,
    const char __user *buffer,
    size_t length,
    loff_t *offset)
{
    char data[32];
    unsigned long new_count;

    if (length >= sizeof(data))
        return -EINVAL;

    if (copy_from_user(data, buffer, length))
        return -EFAULT;

    data[length] = '\0';

    if (kstrtoul(data, 10, &new_count) != 0)
        return -EINVAL;

    mutex_lock(&meter_mutex);

    pulse_count = new_count;

    mutex_unlock(&meter_mutex);

    printk(
        KERN_INFO
        "smart_meter: pulse count updated to %lu\n",
        pulse_count
    );

    return length;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = smart_meter_open,
    .release = smart_meter_release,
    .read = smart_meter_read,
    .write = smart_meter_write
};

/* Module initialization */
static int __init smart_meter_init(void)
{
    int result;

    printk(KERN_INFO "smart_meter: initializing driver\n");

    result = alloc_chrdev_region(
        &device_number,
        0,
        1,
        DEVICE_NAME
    );

    if (result < 0) {
        printk(KERN_ALERT "smart_meter: failed to allocate device number\n");
        return result;
    }

    cdev_init(&smart_meter_cdev, &fops);

    result = cdev_add(
        &smart_meter_cdev,
        device_number,
        1
    );

    if (result < 0) {
        unregister_chrdev_region(device_number, 1);
        return result;
    }

    smart_meter_class = class_create(CLASS_NAME);

    if (IS_ERR(smart_meter_class)) {
        cdev_del(&smart_meter_cdev);
        unregister_chrdev_region(device_number, 1);
        return PTR_ERR(smart_meter_class);
    }

    smart_meter_device = device_create(
        smart_meter_class,
        NULL,
        device_number,
        NULL,
        DEVICE_NAME
    );

    if (IS_ERR(smart_meter_device)) {
        class_destroy(smart_meter_class);
        cdev_del(&smart_meter_cdev);
        unregister_chrdev_region(device_number, 1);
        return PTR_ERR(smart_meter_device);
    }

    printk(KERN_INFO "smart_meter: driver loaded successfully\n");

    return 0;
}

/* Module cleanup */
static void __exit smart_meter_exit(void)
{
    device_destroy(
        smart_meter_class,
        device_number
    );

    class_destroy(smart_meter_class);

    cdev_del(&smart_meter_cdev);

    unregister_chrdev_region(
        device_number,
        1
    );

    printk(KERN_INFO "smart_meter: driver unloaded\n");
}

module_init(smart_meter_init);
module_exit(smart_meter_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Smart Energy Meter Project");
MODULE_DESCRIPTION(
    "Linux character device driver for a virtual smart energy meter"
);
MODULE_VERSION("1.0");
