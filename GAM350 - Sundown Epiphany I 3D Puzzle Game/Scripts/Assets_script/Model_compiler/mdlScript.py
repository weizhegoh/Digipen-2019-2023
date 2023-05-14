___doc__ = """
/*****************************************************************
*\file         mdlScript.py
*\author(s)    Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
"""
import subprocess,os

mainPath = os.path.abspath(os.path.join("../../", os.pardir))
assetsPath = os.path.join(mainPath,"Assets/Model")

for filename in os.listdir(assetsPath):
        #print(os.path.join(mainPath,"Assets/Model"))
        if filename.endswith(".fbx") or filename.endswith(".dae") or filename.endswith(".FBX"):
                subprocess.call(["FbxCompiler.exe", os.path.join(assetsPath,filename)])
                #print(filename)   
          #print(os.path.join(os.path.dirname(os.path.dirname()),filename)))
           #subprocess.call(["FbxCompiler.exe", os.path.join("../../Assets/Model",filename)])

animationPath = os.path.join(mainPath,"SundownEpiphany/Assets/Models/binary/Animation")
staticPath = os.path.join(mainPath,"SundownEpiphany/Assets/Models/binary/Static")

for filename in os.listdir(assetsPath):
         if filename.endswith(".BinaryAnimated"): 
                 os.replace(os.path.join(assetsPath,filename), os.path.join(animationPath,filename))

for filename in os.listdir(assetsPath):
         if filename.endswith(".BinaryStatic"): 
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