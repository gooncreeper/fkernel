#ifndef BOOT_H
#define BOOT_H

// There is also arch_start, though it is only called from assembly and is arch-dependent
_Noreturn void generic_start(void); // Entry point once arch-dependent boot is finished

/* Exit is called when no thread will ever run (all CPU cores are unassigned) or when
 * there is an unrecovable error during booting.
 * It is simply an infinite halt loop, and will not perform any power-off routine.
 */
_Noreturn void kexit(void);

#endif //BOOT_H
