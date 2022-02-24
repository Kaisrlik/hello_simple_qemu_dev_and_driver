#include "qemu/osdep.h"

#include "hw/pci/pci.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "qemu/osdep.h"
#include "qemu/queue.h"
#include "qemu/timer.h"
#include "qemu/units.h"
#include "qom/object.h"
#include "trace.h"

#define HELLO_DEVICE_ID  0xb001

struct hello {
    PCIDevice pdev;
    QemuMutex mutex;
};

#define TYPE_HELLO_DMA_DEVICE "hello"
DECLARE_INSTANCE_CHECKER(struct hello, HELLO, TYPE_HELLO_DMA_DEVICE)

static void hello_instance_init(Object *obj)
{
//    struct hello *hello = ESC2(obj);
}

static void hello_realize(PCIDevice *pdev, Error **errp)
{
    struct hello *hello = HELLO(pdev);
    qemu_mutex_init(&hello->mutex);
}

static void hello_uninit(PCIDevice *pdev)
{
    struct hello *hello = HELLO(pdev);

    qemu_mutex_destroy(&hello->mutex);
}

static void hello_class_init(ObjectClass *class, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(class);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(class);

    k->realize = hello_realize;
    k->exit = hello_uninit;
    k->vendor_id = PCI_VENDOR_ID_INTEL;
    k->device_id = HELLO_DEVICE_ID;
    k->revision = 0x00;
    k->class_id = PCI_CLASS_OTHERS;
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static void register_types(void)
{
    static InterfaceInfo interfaces[] = {
        { INTERFACE_PCIE_DEVICE },
        { },
    };
    static const TypeInfo escape_info = {
        .name          = TYPE_HELLO_DMA_DEVICE,
        .parent        = TYPE_PCI_DEVICE,
        .instance_size = sizeof(struct hello),
        .instance_init = hello_instance_init,
        .class_init    = hello_class_init,
        .interfaces = interfaces,
    };

    type_register_static(&escape_info);
}
type_init(register_types)
