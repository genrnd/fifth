#ifndef VMA__H
#define VMA__H

#include <stdint.h>

/** common memory representation structure
 */
struct vma {
  uint8_t *data;
  uint16_t idx;
  uint16_t size;
};

#endif // VMA__H

