################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/camera_app.c \
../src/fmc_imageon_utils.c \
../src/video_detector.c \
../src/video_frame_buffer.c \
../src/video_generator.c \
../src/video_resolution.c \
../src/xtpg_app.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/camera_app.o \
./src/fmc_imageon_utils.o \
./src/video_detector.o \
./src/video_frame_buffer.o \
./src/video_generator.o \
./src/video_resolution.o \
./src/xtpg_app.o 

C_DEPS += \
./src/camera_app.d \
./src/fmc_imageon_utils.d \
./src/video_detector.d \
./src/video_frame_buffer.d \
./src/video_generator.d \
./src/video_resolution.d \
./src/xtpg_app.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../system_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


