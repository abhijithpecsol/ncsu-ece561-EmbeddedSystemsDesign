f = open('blip.txt', 'r')
raw_data = f.read().split()
f.close()
fixed_data = []

for data in raw_data:
    fixed_data.append(int(data[:2], 16))
    fixed_data.append(int(data[2:], 16))

for data in fixed_data:
    if data > 4095:
        fixed_data[fixed_data.index(data)] = 4095

c_array = "// Generated using Python script by Michael Meli\n\n#include <stdint.h>\n\n#define BLIP_SOUND_LEN (6969)\n\nconst uint16_t blip_sound[] = {"

count = 1
line_count = 5
for data in fixed_data:
    count = count + 1
    line_count = line_count + 1
    c_array = c_array + " " + str(data) + ","
    if line_count == 31:
        line_count = 0
        c_array = c_array + "\n "

c_array = c_array + " 127 };\n"
c_array = c_array.replace("6969", str(count))

f = open('blip_array.txt', 'w+')
f.write(c_array)
f.close()

print(c_array)


