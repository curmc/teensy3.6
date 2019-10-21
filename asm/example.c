#include <stdint.h>
uint32_t Number(void); // Gets def from assembly
extern int value; 

int main() {
  value = Number();
}
