#include "qemu/osdep.h"

#include "hw/pci/pci.h"
#include "hw/pci/pci_bus.h"
#include "hw/irq.h"
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
    /* PCI side space */
    MemoryRegion pci_mmio;
    MemoryRegion pci_io;

    /* PCI registers (excluding pass-through) */
    uint64_t pci_regs[0xf];
    MemoryRegion pci_regs_mr;

    /* Interrupt generation */
    qemu_irq *qirqs;
};

#define TYPE_HELLO_DMA_DEVICE "hello"
DECLARE_INSTANCE_CHECKER(struct hello, HELLO, TYPE_HELLO_DMA_DEVICE)

static void hello_instance_init(Object *obj)
{
//    struct hello *hello = ESC2(obj);
}

__attribute__((unused)) static void hello_set_irq(void *opaque, int irq_num, int level)
{
    struct hello *h = HELLO(opaque);
    qemu_set_irq(h->qirqs[irq_num], level);
}

__attribute__((unused)) static int hello_map_irq(PCIDevice *pci_dev, int irq_num)
{
    return irq_num;
}

static void hello_realize(PCIDevice *pdev, Error **errp)
{
    struct hello *h = HELLO(pdev);
//     PCIHostState *pci = PCI_HOST_BRIDGE(pdev);
    qemu_mutex_init(&h->mutex);

    // initialize some memory
    memory_region_init(&h->pci_io, OBJECT(h), "reg_io", 0x10000);
    memory_region_init(&h->pci_mmio, OBJECT(h), "reg_mmio", 0x10000);

//     pci->bus = pci_register_root_bus(pdev, pdev->id, hello_set_irq, hello_map_irq, h,
//                                      &h->pci_mmio, &h->pci_io, 0, 4, TYPE_PNV_PHB4_ROOT_BUS);
//     pci->bus = pci_register_root_bus(pdev, pdev->id, NULL, NULL, h,
//                                      &h->pci_mmio, &h->pci_io, 0, 4, TYPE_PNV_PHB4_ROOT_BUS);
//     pci_setup_iommu(pci->bus, hello_dma_iommu, h);
//     pci->bus->flags |= PCI_BUS_EXTENDED_CONFIG_SPACE;
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
