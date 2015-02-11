#include <stdio.h>
#include <libudev.h>


int main() {
    printf("Querying hwdb:\n");
    struct udev *udevCtx = udev_new ();
    printf("udev ctx: %p\n", udevCtx);

    struct udev * remaining = udev_unref (udevCtx);
    printf("udev ctx: %p\n", remaining);
}
