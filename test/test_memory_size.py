import importlib.util
import unittest
from pathlib import Path

# Refresh these fixtures after a toolchain, linker configuration, or firmware
# change. First generate each ELF with its corresponding build workflow. For
# PlatformIO:
#
#   pio run -e pico-w
#   .pio/packages/toolchain-rp2040-earlephilhower/bin/arm-none-eabi-size -A -d .pio/build/pico-w/firmware.elf
#
# For an already configured CMake build:
#
#   cmake --build build
#   .pio/packages/toolchain-rp2040-earlephilhower/bin/arm-none-eabi-size -A -d build/MET_STATION.elf
#
# Copy each complete size output into its test below. To obtain the expected
# program_size and data_size, put the toolchain on PATH and run memory_size.py
# against the same ELF, for example:
#
#   PATH=".pio/packages/toolchain-rp2040-earlephilhower/bin:$PATH" \
#       python scripts/memory_size.py --file build/MET_STATION.elf
#
# Finally, run:
#
#   python -m unittest discover -s test -p 'test_memory_size.py'

spec = importlib.util.spec_from_file_location("memory_size", Path(__file__).resolve().parents[1] / "scripts" / "memory_size.py")
memory_size = importlib.util.module_from_spec(spec)
spec.loader.exec_module(memory_size)


class MemorySizeTests(unittest.TestCase):
    def test_parse_size_output_matches_platformio_elf_sections(self):
        output = """.pio/build/pico-w/firmware.elf  :
section                  size        addr
.boot2                    256   268435456
.text                   63012   268435712
.rodata                242720   268498728
.binary_info               44   268741448
.xip_ram                    0   352321536
.ram_vector_table         192   536870912
.uninitialized_data        32   536871104
.data                    3128   536871136
.tdata                      0   536874264
.tbss                       0   536874264
.tbss_space                 0   536874264
.bss                    49288   536874264
.tls0                       0   536923552
.tls1                       0   536923552
.persistent_data            0   536923552
.dummy_pre_heap             0   536923552
.heap                    2048   536923552
.scratch_x                  0   537133056
.scratch_y                  0   537137152
.stack_dummy             2048   537137152
.debug_info            398841           0
.debug_abbrev           70600           0
.debug_loclists        141496           0
.debug_aranges           9712           0
.debug_rnglists         18497           0
.debug_line            224641           0
.debug_str              69565           0
.comment                   70           0
.ARM.attributes            42           0
.debug_frame            24876           0
.debug_line_str           877           0
Total                 1321985
"""

        parsed = memory_size.parse_size_output(output)

        self.assertEqual(parsed["program_size"], 94024)
        self.assertEqual(parsed["data_size"], 11964)

    def test_parse_size_output_matches_cmake_elf_sections(self):
        output = """build/MET_STATION.elf  :
section                  size        addr
.boot2                    256   268435456
.text                   62988   268435712
.rodata                242720   268498704
.binary_info               44   268741424
.xip_ram                    0   352321536
.ram_vector_table         192   536870912
.uninitialized_data        32   536871104
.data                    3128   536871136
.tdata                      0   536874264
.tbss                       0   536874264
.tbss_space                 0   536874264
.bss                    49288   536874264
.tls0                       0   536923552
.tls1                       0   536923552
.persistent_data            0   536923552
.dummy_pre_heap             0   536923552
.heap                    2048   536923552
.scratch_x                  0   537133056
.scratch_y                  0   537137152
.stack_dummy             2048   537137152
.debug_info            398226           0
.debug_abbrev           70431           0
.debug_loclists        141481           0
.debug_aranges           9664           0
.debug_rnglists         18420           0
.debug_line            224271           0
.debug_str              69228           0
.comment                   70           0
.ARM.attributes            42           0
.debug_frame            24720           0
.debug_line_str           877           0
Total                 1320174
"""

        parsed = memory_size.parse_size_output(output)

        self.assertEqual(parsed["program_size"], 309136)
        self.assertEqual(parsed["data_size"], 52608)


if __name__ == "__main__":
    unittest.main()
