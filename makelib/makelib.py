#!/usr/bin/python
import sys
from shutil import copytree
from shutil import rmtree
from shutil import copy
import os
import re



def copy_contents(source,dst):
    src_files = os.listdir(source)
    for file_name in src_files:
        full_file_name = os.path.join(source, file_name)
        print ("copying ",full_file_name," to ", dst)
        if os.path.isfile(full_file_name):
            copy(full_file_name, dst)


def copy_contents_cond(source,dst,excludes):
    src_files = os.listdir(source)
    for file_name in src_files:
        if not (file_name in excludes):
            full_file_name = os.path.join(source, file_name)
            print ("copying ",full_file_name," to ", dst)
            if os.path.isfile(full_file_name):
                copy(full_file_name,dst)


# src should be the root dirt of CommProto git
src = sys.argv[1]
# dst should be the place where your arduino libraries are
dst = sys.argv[2]

# first delete destination directory
if os.path.exists(dst):
    rmtree(dst)
os.mkdir(dst)


# copy everything from src to destination
cppSourceFiles = src +"\\CommProtoLib\\src"
copy_contents(cppSourceFiles,dst)

# copy from pd folder
espPdFiles = src +"\\CommProtoLib\\pd\\esp\\interface"
copy_contents(espPdFiles,dst)

# copy everything from interface   
interfaceDir = src +"\\CommProtoLib\\interface\\commproto"
copytree(interfaceDir, os.path.join(dst,"commproto"));

# copy stuff from esp_app
espAppSrc = src+"/EspApp"
copy_contents_cond(espAppSrc, dst,["source.cpp","CMakeLists.txt"])