﻿#ifndef HGL_SHADER_DEFINE_INCLUDE
#define HGL_SHADER_DEFINE_INCLUDE

namespace hgl
{
    namespace graph
    {
        //这两个变量未来改用get出来的值
        #define HGL_MAX_TEX_COORD       32
        #define HGL_MAX_VERTEX_BUFFER   32

        //vertex shader 矩阵部分
        #define HGL_VS_PROJECTION_MATRIX        "ProjectionMatrix"
        #define HGL_VS_MODELVIEW_MATRIX         "ModelViewMatrix"
        #define HGL_VS_MVP_MATRIX               "ModelViewProjectionMatrix"
        #define HGL_VS_NORMAL_MATRIX            "NormalMatrix"

        //vertex shader 传入部分，必须与VertexBufferName中的名称对应
        #define HGL_VS_NORMAL                   "Normal"
        #define HGL_VS_COLOR                    "Color"
        #define HGL_VS_VERTEX                   "Vertex"

        //fragment shader 部分
        #define HGL_FS_NORMAL                   "FragmentNormal"
        #define HGL_FS_COLOR                    "FragmentColor"
        #define HGL_FS_POSITION                 "FragmentPosition"

        #define HGL_MATERIAL_COLOR              "MaterialColor"

        #define HGL_FS_FRAG_COLOR               "FragColor"

        #define HGL_FS_ALPHA_TEST               "AlphaTest"

        #define HGL_FS_TEXCOORD                 "FragmentTexCoord_"

        //仅在fs中使用的
        #define HGL_FS_DIFFUSE_COLOR            "DiffuseColor"
        #define HGL_FS_ALPHA                    "Alpha"

        //太阳光数据,此数据即可能在VS,也可能在FS
        #define HGL_SUN_LIGHT_DIRECTION         "SunLightDirection"     //太阳光方向
        #define HGL_SUN_LIGHT_INTENSITY         "SunLightIdtensity"     //太阳光强度
        #define HGL_SUN_LIGHT_SPECULAR          "SunLightSpecular"      //太阳光高光
    }//namespace graph
}//namespace hgl
#endif//HGL_SHADER_DEFINE_INCLUDE
