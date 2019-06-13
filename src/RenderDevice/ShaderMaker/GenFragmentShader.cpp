﻿#include"GenFragmentShader.h"
#include"../ShaderDefine.h"
#include<hgl/graph/Renderable.h>
#include<hgl/graph/Texture.h>
#include<hgl/LogInfo.h>
#include<stdio.h>
#include<string.h>

namespace hgl
{
    namespace graph
    {
        namespace shadergen
        {
            fs::fs()
            {
                in_normal=sitNone;
                in_color=sitNone;

                light_mode=HGL_NONE_LIGHT;
                sun_light=false;

                color_material=false;

                hgl_zero(mtc_tex_type);

                alpha_test=false;
                outside_discard=false;
            }

            void fs::add_frag_color()
            {
                add("layout(location=0,index=0) out vec4 " HGL_FS_FRAG_COLOR ";\n");
            }

            void fs::add_alpha_test()
            {
                alpha_test=true;

                add_uniform_float(HGL_FS_ALPHA_TEST);
            }

            void fs::add_in_normal()
            {
                add_in_vec3(HGL_FS_NORMAL);

                in_normal=sitVertexAttrib;
            }

            void fs::set_color_material()
            {
                color_material=true;

                add_uniform_vec4(HGL_MATERIAL_COLOR);
            }

            void fs::add_in_color()
            {
                in_color=sitVertexAttrib;

                add_in_fv(HGL_FS_COLOR,4);
            }

            /**
            * 增加一张贴图
            * @param mtc_index 成份索引
            * @param tex_type 纹理类型
            * @param pixel_format 象素格式
            * @param source 数据源名称
            */
            void fs::add_in_texture(int mtc_index,uint tex_type,uint pixel_format,const char *source)
            {
                MATERIAL_TEXTURE_CHANNEL_NAME mtc_name;

                GetMaterialTextureName(mtc_name,mtc_index);

                mtc_tex_type[mtc_index]=tex_type;

                tex_pf[mtc_index] = pixel_format;

                tex_coord[mtc_index].Strcat(HGL_FS_TEXCOORD);
                tex_coord[mtc_index].Strcat(source);

                tex_sampler_rgb[mtc_index].Strcat("texture(");            //texture会处理过滤,如果不过滤使用texelFetch会更高效
                tex_sampler_rgb[mtc_index].Strcat(mtc_name);
                tex_sampler_rgb[mtc_index].Strcat(",");
                tex_sampler_rgb[mtc_index].Strcat(HGL_FS_TEXCOORD);
                tex_sampler_rgb[mtc_index].Strcat(source);
                tex_sampler_rgb[mtc_index].Strcat(")");

                tex_sampler_alpha[mtc_index].Strcat("texture(");            //texture会处理过滤,如果不过滤使用texelFetch会更高效
                tex_sampler_alpha[mtc_index].Strcat(mtc_name);
                tex_sampler_alpha[mtc_index].Strcat(",");
                tex_sampler_alpha[mtc_index].Strcat(HGL_FS_TEXCOORD);
                tex_sampler_alpha[mtc_index].Strcat(source);
                tex_sampler_alpha[mtc_index].Strcat(")");

                if(pixel_format == GL_RED
                 ||pixel_format == GL_DEPTH)
                {
                    tex_sampler_rgb[mtc_index].Strcat(".rrr");
                    tex_sampler_alpha[mtc_index].Strcat(".r");
                }
                else
                {
                    if (pixel_format == GL_RGBA)
                    {
                        tex_sampler_rgb[mtc_index].Strcat(".rgb");
                        tex_sampler_alpha[mtc_index].Strcat(".a");
                    }
                    else
                    if (pixel_format == GL_RGB)
                    {
                        tex_sampler_rgb[mtc_index].Strcat(".rgb");
                        tex_sampler_alpha[mtc_index].Clear();
                    }
                    else
                    if (pixel_format == GL_RG)
                    {
                        tex_sampler_rgb[mtc_index].Strcat(".rrr");
                        tex_sampler_alpha[mtc_index].Strcat(".b");
                    }
                }

                add_sampler(mtc_name,tex_type);
            }

            /**
            * 增加一个贴图坐标数据
            * @param num 坐标维数
            * @param fragname 贴图名称
            */
            void fs::add_in_texcoord(int num,const char *fragname)
            {
                UTF8String texcoord_name;

                texcoord_name.Strcat(HGL_FS_TEXCOORD);
                texcoord_name.Strcat(fragname);

                add_in_fv(texcoord_name,num);
            }

            bool fs::add_end()
            {
                UTF8String fin_color;

                if(mtc_tex_type[mtcDiffuse])        //如果漫反射贴图也为否，则完全无色彩信息，退出
                {
                    if(outside_discard)        //有出界放弃处理
                    {
                        add(    "\tvec2 diff_tex_coord="+tex_coord[mtcDiffuse]+".xy;\n\n");

                        add(    "\tif(diff_tex_coord.x<0||diff_tex_coord.x>1\n"
                                "\t ||diff_tex_coord.y<0||diff_tex_coord.y>1)discard;\n\n");
                    }

                    add(U8_TEXT("\tvec3 " HGL_FS_DIFFUSE_COLOR "=")+tex_sampler_rgb[mtcDiffuse]+U8_TEXT(";\n"));

                    if(in_color)    //还有顶点颜色传入
                    {
                        if(color_material)
                            fin_color="vec4(" HGL_FS_DIFFUSE_COLOR "*" HGL_FS_COLOR ".rgb*" HGL_MATERIAL_COLOR ".rgb," HGL_FS_COLOR ".a*" HGL_MATERIAL_COLOR ".a)";
                        else
                            fin_color="vec4(" HGL_FS_DIFFUSE_COLOR "*" HGL_FS_COLOR ".rgb," HGL_FS_COLOR ".a)";
                    }
                    else
                    {
                        if(color_material)
                            fin_color="vec4(" HGL_FS_DIFFUSE_COLOR "*" HGL_MATERIAL_COLOR ".rgb," HGL_MATERIAL_COLOR ".a)";
                        else
                            fin_color=HGL_FS_DIFFUSE_COLOR;
                    }
                }
                else    //无漫反射贴图
                {
                    if(!in_color)               //无顶点颜色
                    {
                        if(!color_material)     //无材质颜色
                        {
                            LOG_ERROR("No Diffuse,No VertexColor,No Color Material.");
                            RETURN_FALSE;      //即使有其它贴图，也必须有颜色
                        }

                        fin_color=HGL_MATERIAL_COLOR;
                    }
                    else
                    {
                        if(color_material)
                            fin_color=HGL_FS_COLOR "*" HGL_MATERIAL_COLOR;
                        else
                            fin_color=HGL_FS_COLOR;
                    }
                }

                if(mtc_tex_type[mtcAlpha])                //透明贴图
                {
                    add(U8_TEXT("\tfloat tex_alpha=")+tex_sampler_alpha[mtcAlpha]+U8_TEXT(";\n\n"));

                    add(U8_TEXT("\tfloat " HGL_FS_ALPHA "=tex_alpha;\n"));

                    add();
                }

                if(alpha_test)        //alpha test
                {
                    if(mtc_tex_type[mtcAlpha])
                        add("\tif("+fin_color+U8_TEXT(".a*tex_alpha<" HGL_FS_ALPHA_TEST ")discard;\n"));
                    else
                        add("\tif("+fin_color+U8_TEXT(".a<" HGL_FS_ALPHA_TEST ")discard;\n"));

                    add();
                }

                if(in_normal)        //计算法线
                {
                    //add_format("\tfloat normal_instensity=max(dot(%s,vec3(0.0,0.0,1.0)),0.0);\n",HGL_FS_NORMAL);
                    //add();
                }

                if (sun_light)
                {
                    if (light_mode == HGL_PIXEL_LIGHT)
                        add("\tfloat " HGL_SUN_LIGHT_INTENSITY "=max(dot(" HGL_FS_NORMAL "," HGL_SUN_LIGHT_DIRECTION "),0.0);\n");

                    add(U8_TEXT("\t" HGL_FS_FRAG_COLOR "=vec4(") + fin_color + U8_TEXT(".rgb*" HGL_SUN_LIGHT_INTENSITY ","));

                    if(tex_pf[mtcDiffuse] == HGL_PC_RGBA
                     ||tex_pf[mtcDiffuse] == HGL_PC_LUMINANCE_ALPHA)
                    {
                        add(fin_color + U8_TEXT(".a"));

                        if (mtc_tex_type[mtcAlpha])
                            add("*" HGL_FS_ALPHA ");\n");
                        else
                            add(");\n");
                    }
                    else
                    {
                        if (mtc_tex_type[mtcAlpha])
                            add(HGL_FS_ALPHA ");\n");
                        else
                            add("1);\n");
                    }
                }
                else
                {
                    if(mtc_tex_type[mtcAlpha])
                        add(U8_TEXT("\t" HGL_FS_FRAG_COLOR "=vec4(")+fin_color+U8_TEXT(".rgb,")+fin_color+U8_TEXT(".a*" HGL_FS_ALPHA ");\n"));
                    else
                    {
                        //if(hsv_clamp_color)
                        //    add(U8_TEXT("\t" HGL_FS_FRAG_COLOR "=vec4(rgb2hsv(") + fin_color + U8_TEXT(".rgb).rgb,1);\n"));
                        //else
                            add(U8_TEXT("\t" HGL_FS_FRAG_COLOR "=vec4(")+fin_color+U8_TEXT(".rgb,1);\n"));
                    }
                }

                return(true);
            }
        }//namespace shadergen

        /**
        * 生成Fragment Shader代码
        * @param state 渲染状态
        * @return 生成的Fragment Shader代码
        * @return NULL 生成失败
        */
#ifdef _DEBUG
        char *MakeFragmentShader(const RenderState *state,const OSString &filename)
#else
        char *MakeFragmentShader(const RenderState *state)
#endif//_DEBUG
        {
            if(!state)return(nullptr);

            shadergen::fs code;

            code.add_version(330);        //OpenGL 3.3

            //颜色
            {
                if(state->vertex_color)                    //使用顶点颜色
                {
                    code.add_in_color();
                }
                else
                if(state->color_material)                //使用独立颜色传入
                {
                    code.set_color_material();            //使用材质颜色
                }
            }

            //灯光
            if(state->light_mode>HGL_NONE_LIGHT)
            {
                //法线
                //法线无论那一种方式传入，都必须在vertex shader中乘上mv_matrix

                if(state->vertex_normal)                //使用顶点法线
                {
                    code.add_in_normal();

                    //光照
                    code.set_light_mode(state->light_mode);

                    if(state->sun_light)
                    {
                        if (state->light_mode == HGL_VERTEX_LIGHT)
                        {
                            code.add_in_float(HGL_SUN_LIGHT_INTENSITY);
                        }
                        else
                        if (state->light_mode == HGL_PIXEL_LIGHT)
                        {
                            code.add_uniform_vec3(HGL_SUN_LIGHT_DIRECTION);
                        }

                        code.add_sun_light();
                    }
                }

//                if(state->normal_map)
//                {
//                }
            }

            if(state->tex_number)                            //如果有贴图
            {
                 int count=0;

                MATERIAL_TEXTURE_CHANNEL_NAME mtc_name,mtc_name_height;

                if(state->height_map)                            //有高度图的处理方式
                {
                    GetMaterialTextureName(mtc_name_height,mtcHeight);
                    code.add_in_texcoord(2,mtc_name_height);        //从高度图输入各种坐标
                }

                for(int mtc=0;mtc<mtcMax;mtc++)
                {
                    if(mtc==mtcHeight)continue;                    //高度图不在fs处理

                    if(!state->vbc[mtc])continue;

                    if(state->height_map
                    &&(mtc>=mtcDiffuse&&mtc<mtcPalette))        //使用高度图时，这一部分贴图无需贴图坐标
                    {
                        code.add_in_texture(mtc,state->tex_type[mtc],state->tex_pf[mtc],mtc_name_height);                //加入贴图，从高度图顶点得来的纹理坐标
                    }
                    else
                    {
                        if(!state->tex_type[mtc])continue;

                        GetMaterialTextureName(mtc_name,mtc);

                        code.add_in_texture(mtc,state->tex_type[mtc],state->tex_pf[mtc],mtc_name);                        //从对应通道得来的纹理
                        code.add_in_texcoord(state->vbc[mtc],mtc_name);                                                    //从对应通道得来的纹理坐标
                    }

                    count++;
                }

                if(count)        //如果没有加贴图，不加这个空行
                    code.add();
            }

            //alpha test
            if(state->alpha_test)
            {
                code.add_alpha_test();
                code.add();
            }

            code.add_frag_color();                    //指定颜色输出
            code.add();

            //if(state->render_mode)                    //是否是延迟渲染
            //    code.add_gbuffer_encode();

            code.add_main_begin();

            //if(state->render_mode)
            //    code.add_ds_end();

            code.add_end();                            //收尾各方面的处理

            code.add_main_end();

#ifdef _DEBUG
            code.debug_out(filename+OS_TEXT(".fs.glsl"));
#endif//_DEBUG

            return code.end_get();
        }//char *MakeFragmentShader(RenderState *state)
    }//namespace graph
}//namespace hgl
