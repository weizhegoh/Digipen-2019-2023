___doc__ = """
/*****************************************************************
*\file         objScript.py
*\author(s)    Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
"""

import subprocess,os


mainPath = os.path.abspath(os.path.join("../../", os.pardir))
assetsPath = os.path.join(mainPath,"Assets/Object")

for filename in os.listdir(assetsPath):
       if filename.endswith(".obj"): 
           #print(os.path.join(filename))
           subprocess.call(["obj_compiler.exe", os.path.join(assetsPath,filename)])

staticPath = os.path.join(mainPath,"SundownEpiphany/Assets/Models/binary/Object")


for filename in os.listdir(assetsPath):
        if filename.endswith(".BinaryObj"): 
                os.replace(os.path.join(assetsPath,filename), os.path.join(staticPath,filename))

# for filename in os.listdir(directory):
#     if filename.endswith(".asm") or filename.endswith(".py"): 
#          # print(os.path.join(directory, filename))
#         continue
#     else:
#         continue
# 3

# import subprocess

# # Call your exe
# subprocess.call('3DObject_Parser.exe sphere.obj')

# # if you want to print output 
# p = subprocess.check_output('3DObject_Parser.exe sphere.obj')
# print(p)