#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/pci.h>

#define HELLO_DEVICE_ID	     0xb001

struct hello_dev {
	struct pci_dev *pdev;
	struct miscdevice hello_misc;
};

static inline struct hello_dev *hello_get_priv(struct pci_dev *pdev)
{
	return pci_get_drvdata(pdev);
}

static long hello_ioctl(struct file *file, unsigned int cmd,
			unsigned long arg)
{
	struct hello_dev *h = container_of(file->private_data,
					   struct hello_dev, hello_misc);

	pci_warn(h->pdev, "Unsupported cmd: %d\n", cmd);
	return -ENOSYS;
}

static const struct file_operations hello_fops = {
	.owner          = THIS_MODULE,
	.unlocked_ioctl = hello_ioctl,
};

static int hello_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	struct hello_dev *h;
	int err;
	pci_info(pdev, "Probing hello driver.\n");

	err = pci_enable_device(pdev);
	if (err) {
		pci_err(pdev, "Can't enable device.\n");
		goto err_dev;
	}

	h = kzalloc(sizeof(struct hello_dev), GFP_KERNEL);
	if (!h) {
		pci_err(pdev, "kzalloc()\n");
		goto err_dev;
	}
	pci_set_drvdata(pdev, h);

	h = hello_get_priv(pdev);
	h->pdev = pdev;

	h->hello_misc.minor = MISC_DYNAMIC_MINOR;
	h->hello_misc.name = "Hello dev";
	h->hello_misc.fops = &hello_fops;
	h->hello_misc.mode = S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP;

	err = misc_register(&h->hello_misc);
	if (err) {
		pci_err(h->pdev, "Cannot create hello device.\n");
		goto err_kfree;
	}
	pci_info(h->pdev, "Hello device is created.\n");

	return 0;
err_kfree:
	kfree(h);
err_dev:
	pci_disable_device(pdev);
	return err;
}

static void hello_remove(struct pci_dev *pdev)
{
	struct hello_dev *h = hello_get_priv(pdev);

	misc_deregister(&h->hello_misc);
	pci_warn(h->pdev, "Hello misc was removed\n");

	pci_set_drvdata(pdev, NULL);
	kfree(h);

	pci_disable_device(pdev);
}

static const struct pci_device_id hello_pci_tbl[] = {
	{ PCI_VDEVICE(INTEL, HELLO_DEVICE_ID) },
	/* required last entry */
	{0, }
};

MODULE_DEVICE_TABLE(pci, hello_pci_tbl);

static void hello_shutdown(struct pci_dev *pdev)
{
	pci_info(pdev, "Shutdown is not implement yet.\n");
}

static struct pci_driver hello_driver = {
	.name     = "Hello",
	.id_table = hello_pci_tbl,
	.probe = hello_probe,
	.remove = hello_remove,
	.shutdown = hello_shutdown,
};

module_pci_driver(hello_driver);

MODULE_AUTHOR("Intel Corporation, <jan.kaisrlik@intel.com>");
MODULE_DESCRIPTION("Simple hello driver");
MODULE_LICENSE("GPL v2");

// vim: ts=8 sw=8
