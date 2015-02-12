#include <libudev.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "usblookup.h"

static char* StrDup(const char* src) {
   int memLen = strlen(src) + 1;
   char* result = malloc(memLen);
   memcpy(result, src, memLen);
   return result;
}


void getVendorById(unsigned int vendorId, char** resultVendor) {
   getModelById(vendorId, 0, resultVendor, NULL);
}


void getModelById(unsigned int vendorId, unsigned int productId, char** resultVendor, char** resultProduct) {
    struct udev *udevCtx = udev_new();
    struct udev_hwdb *hwdbCtx = udev_hwdb_new (udevCtx);

    char modalias[20];
    sprintf(modalias, "usb:v%04Xp%04X*", vendorId, productId);
    struct udev_list_entry *properties = udev_hwdb_get_properties_list_entry (hwdbCtx, modalias, 0);
    if (properties) {
       struct udev_list_entry *vendorProperty;
       vendorProperty = udev_list_entry_get_by_name(properties, "ID_VENDOR_FROM_DATABASE");
       if (vendorProperty) {
          const char* vendor = udev_list_entry_get_value(vendorProperty);
          *resultVendor = StrDup(vendor);
       }

       if (resultProduct) {
          struct udev_list_entry *productProperty;
          productProperty = udev_list_entry_get_by_name(properties, "ID_MODEL_FROM_DATABASE");
          if (productProperty) {
             const char* product = udev_list_entry_get_value(productProperty);
             *resultProduct = StrDup(product);
          }
       }
    }

    udev_hwdb_unref (hwdbCtx);
    udev_unref (udevCtx);
}

