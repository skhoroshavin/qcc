#!/usr/bin/python3
import os

src_dir = os.path.realpath(__file__)
src_dir = os.path.dirname(src_dir)+"/../src"
src_dir = os.path.abspath(src_dir)

out = open("qcc_all_in_one.h", "w")
loaded_files = []

def load_file(name):
    if name in loaded_files:
        return
    loaded_files.append(name)
    f = open(src_dir + "/" + name)
    for l in f:
        if (l.startswith("#pragma once")):
            continue
        if (l.startswith("#include")):
            x = l.split('"')
            if (len(x) > 1):
                load_file(x[1])
                continue
        out.write(l)

out.write("#ifndef __QCC__\n")
out.write("#define __QCC__\n")
out.write("\n")

for filename in os.listdir(src_dir):
    if filename.endswith(".h"):
        load_file(filename)

out.write("\n")
out.write("#ifdef __QCC_IMPLEMENTATION__\n")
out.write("\n")

for filename in os.listdir(src_dir):
    if filename.endswith(".c"):
        load_file(filename)

out.write("\n")
out.write("#endif /* __QCC_IMPLEMENTATION__ */\n")
out.write("\n")

out.write("#endif /* __QCC__ */\n")
