#Uncomment for hardware floating point
#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
#add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
#add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)

include_directories(
        Peripheral/Inc
)

add_definitions(-DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F429_439xx)

file(GLOB_RECURSE STM32F429_SOURCES "Peripheral/*.*")

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F429IGTX_FLASH.ld)

add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)