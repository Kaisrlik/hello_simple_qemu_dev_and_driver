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

#define HELLO_DEVICE_ID  0xbcc7
#define HELLO_REG_SIZE   0x0200

// Request address
#define REQ_DATA_LO   0x048
#define REQ_DATA_HI   0x04c
#define REQ_ADDR_LO   0x050
#define REQ_ADDR_HI   0x054
#define REQ_BE_OPCODE 0x058

// Acknowladge address
#define ACK_DATA_LO   0x05c
#define ACK_DATA_HI   0x060
#define ACK_STATUS    0x064

struct req {
        uint32_t addr;
        uint32_t data_lo;
        uint32_t data_hi;
        uint8_t  opcode;
        uint8_t  be;
};

struct hello {
    PCIDevice pdev;

//     uint64_t pci_regs[HELLO_REG_SIZE];
    MemoryRegion pci_regs_mr;

    struct req *req;
};


static uint64_t hello_mem_read(void *opaque, hwaddr off, unsigned size)
{
    return 42;
}

static void hello_mem_write(void *opaque, hwaddr off, uint64_t val,
                               unsigned size)
{
//     struct hello *h = HELLO(opaque);
//     h->pci_regs[off] = val;
}

static const MemoryRegionOps hello_reg_ops = {
    .read = hello_mem_read,
    .write = hello_mem_write,
    .endianness = DEVICE_BIG_ENDIAN,
    .valid = {
        .min_access_size = 8,
        .max_access_size = 8,
//         .accepts = hello_mmio_accepts,
    },
    .impl = {
        .min_access_size = 8,
        .max_access_size = 8,
    },
};

#define TYPE_HELLO_DMA_DEVICE "hello"
DECLARE_INSTANCE_CHECKER(struct hello, HELLO, TYPE_HELLO_DMA_DEVICE)

static void hello_instance_init(Object *obj)
{
}

__attribute__((unused)) static void hello_set_irq(void *opaque, int irq_num, int level)
{
//     struct hello *h = HELLO(opaque);
//     qemu_set_irq(h->qirqs[irq_num], level);
}

__attribute__((unused)) static int hello_map_irq(PCIDevice *pci_dev, int irq_num)
{
    return irq_num;
}

static void hello_realize(PCIDevice *pdev, Error **errp)
{
    struct hello *h = HELLO(pdev);
    h->req = g_malloc(sizeof(struct req));

    // initialize some memory
    memory_region_init_io(&h->pci_regs_mr, OBJECT(h), &hello_reg_ops, h,
            "reg_io", HELLO_REG_SIZE);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &h->pci_regs_mr);
}

static void hello_uninit(PCIDevice *pdev)
{
    struct hello *h = HELLO(pdev);
    free(h->req);
}

static uint32_t hello_pci_read_config(PCIDevice *dev, uint32_t addr, int len)
{
    uint32_t val;

    val = pci_default_read_config(dev, addr, len);
    qemu_log_mask(LOG_GUEST_ERROR, "!!! %s: addr:0x%x, l:0x%x\n", __func__, addr, len);

    if (len != 1)
        return val;

    switch (addr) {
    case 0x00:
        val = 0xff;
        break;
    case ACK_DATA_LO:
        // maybe add to req. struct
        val = 0xaa;
        break;
    case ACK_DATA_HI:
        // maybe add to req. struct
        val = 0x55;
        break;
    case ACK_STATUS:
        val = 0x01;
        qemu_log_mask(LOG_GUEST_ERROR, "%s: read status is fired!\n", __func__);
        break;
    default:
    }

    return val;

}

static void hello_pci_write_config(PCIDevice *pdev,
        uint32_t addr, uint32_t val, int len)
{
    struct hello *h = HELLO(pdev);
    switch (addr) {
    case 0x80:
        break;
    case REQ_DATA_LO:
        h->req->data_lo = val;
        break;
    case REQ_DATA_HI:
        h->req->data_hi = val;
        break;
    case REQ_ADDR_LO:
        h->req->addr = val;
        break;
    case REQ_BE_OPCODE:
        h->req->opcode = val;
        qemu_log_mask(LOG_GUEST_ERROR, "%s: write is fired!\n", __func__);
        break;
    default:
        pci_default_write_config(pdev, addr, val, len);
    }
}

static void hello_class_init(ObjectClass *class, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(class);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(class);

    k->realize = hello_realize;
    k->config_write = hello_pci_write_config;
    k->config_read = hello_pci_read_config;
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
    static const TypeInfo hello_info = {
        .name          = TYPE_HELLO_DMA_DEVICE,
        .parent        = TYPE_PCI_DEVICE,
        .instance_size = sizeof(struct hello),
        .instance_init = hello_instance_init,
        .class_init    = hello_class_init,
        .interfaces = interfaces,
    };

    type_register_static(&hello_info);
}
type_init(register_types)
