#ifndef __KERNEL_HARDWARE_MPU_H__
# define __KERNEL_HARDWARE_MPU_H__

// Define MPU arch.
typedef enum mpu_e
{
	MPU_SH7305,
	MPU_SH7724,
	MPU_SH7337,
	MPU_SH7355,
	MPU_UNKNOWN,
} mpu_t;

#endif /*__KERNEL_HARDWARE_MPU_H__*/
