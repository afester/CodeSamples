#ifndef __USB_LOOKUP_H__
#define __USB_LOOKUP_H__

void getModelById(unsigned int vendorId, unsigned int productId, char** resultVendor, char** resultProduct);
void getVendorById(unsigned int vendorId, char** resultVendor);

#endif
