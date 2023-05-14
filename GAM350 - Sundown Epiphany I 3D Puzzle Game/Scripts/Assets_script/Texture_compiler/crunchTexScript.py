___doc__ = """
/*****************************************************************
*\file         crunchTexScript.py
*\author(s)    Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
"""
import subprocess,os

pixelformat = "DXT5" #can choose
quality = "255"

mainPath = os.path.abspath(os.path.join("../../", os.pardir))
assetsPath = os.path.join(mainPath,"Assets/Texture")

for filename in os.listdir(assetsPath):
        subprocess.call(["tex_compiler.exe", "c", os.path.join(assetsPath,filename),"-pixelformat",pixelformat,"-quality",quality])

texturePath = os.path.join(mainPath,"SundownEpiphany/Assets/Textures")

for filename in os.listdir(assetsPath):
        if filename.endswith(".dds"): 
                os.replace(os.path.join(assetsPath,filename), os.path.join(texturePath,filename))
