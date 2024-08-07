#!/usr/bin/env python

import glob
import os

enc = "utf-8"

# Change to the directory where the script is located,
# so that the script can be run from any location
os.chdir(os.path.dirname(os.path.realpath(__file__)))

# Generate include files

f = open("theme_data.h", "wb")

f.write(b"// THIS FILE HAS BEEN AUTOGENERATED, DON'T EDIT!!\n")

# Generate png image block
f.write(b"\n// png image block\n")

pixmaps = glob.glob("*.png")
pixmaps.sort()

for x in pixmaps:
    var_str = x[:-4] + "_png"

    s = "\nstatic const unsigned char " + var_str + "[] = {\n\t"
    f.write(s.encode(enc))

    pngf = open(x, "rb")

    b = pngf.read(1)
    while len(b) == 1:
        f.write(hex(ord(b)).encode(enc))
        b = pngf.read(1)
        if len(b) == 1:
            f.write(b", ")

    f.write(b"\n};\n")
    pngf.close()

# Generate shaders block
f.write(b"\n// shaders block\n")

shaders = glob.glob("*.gsl")
shaders.sort()

for x in shaders:
    var_str = x[:-4] + "_shader_code"

    s = "\nstatic const char *" + var_str + " = \n"
    f.write(s.encode(enc))

    sf = open(x, "rb")

    b = sf.readline()
    while b != "":
        if b.endswith("\r\n"):
            b = b[:-2]
        if b.endswith("\n"):
            b = b[:-1]
        s = '			"' + b
        f.write(s.encode(enc))
        b = sf.readline()
        if b != "":
            f.write(b'"\n')

    f.write(b'";\n')
    sf.close()

f.close()
