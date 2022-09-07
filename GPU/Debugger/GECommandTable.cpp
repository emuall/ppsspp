// Copyright (c) 2022- PPSSPP Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official git repository and contact information can be found at
// https://github.com/hrydgard/ppsspp and http://www.ppsspp.org/.

#include <cstring>
#include "Common/Common.h"
#include "Common/Log.h"
#include "GPU/Debugger/GECommandTable.h"
#include "GPU/ge_constants.h"

struct GECmdAlias {
	GECommand reg;
	const char *aliases[3];
};

static constexpr GECmdInfo geCmdInfo[] = {
	{ GE_CMD_NOP, "nop", GECmdFormat::NONE },
	{ GE_CMD_VADDR, "setvaddr", GECmdFormat::RELATIVE_ADDR },
	{ GE_CMD_IADDR, "setiaddr", GECmdFormat::RELATIVE_ADDR },
	{ GE_CMD_UNKNOWN_03, "unknown03", GECmdFormat::NONE },
	{ GE_CMD_PRIM, "prim", GECmdFormat::PRIM },
	{ GE_CMD_BEZIER, "bezier", GECmdFormat::BEZIER },
	{ GE_CMD_SPLINE, "spline", GECmdFormat::SPLINE },
	{ GE_CMD_BOUNDINGBOX, "btest", GECmdFormat::PRIM },
	{ GE_CMD_JUMP, "jump", GECmdFormat::JUMP },
	{ GE_CMD_BJUMP, "bjump", GECmdFormat::JUMP },
	{ GE_CMD_CALL, "call", GECmdFormat::JUMP },
	{ GE_CMD_RET, "ret", GECmdFormat::NONE },
	{ GE_CMD_END, "end", GECmdFormat::DATA16 },
	{ GE_CMD_UNKNOWN_0D, "unknown0d", GECmdFormat::NONE },
	{ GE_CMD_SIGNAL, "signal", GECmdFormat::SIGNAL },
	{ GE_CMD_FINISH, "finish", GECmdFormat::NONE },
	{ GE_CMD_BASE, "base", GECmdFormat::HIGH_ADDR_ONLY },
	{ GE_CMD_UNKNOWN_11, "unknown11", GECmdFormat::NONE },
	{ GE_CMD_VERTEXTYPE, "vtype", GECmdFormat::VERTEX_TYPE },
	{ GE_CMD_OFFSETADDR, "setoffset", GECmdFormat::OFFSET_ADDR },
	{ GE_CMD_ORIGIN, "origin", GECmdFormat::NONE },
	{ GE_CMD_REGION1, "regionrate", GECmdFormat::X10_Y10 },
	{ GE_CMD_REGION2, "regionstop", GECmdFormat::X10_Y10 },
	{ GE_CMD_LIGHTINGENABLE, "lighting_on", GECmdFormat::FLAG },
	{ GE_CMD_LIGHTENABLE0, "light0_on", GECmdFormat::FLAG },
	{ GE_CMD_LIGHTENABLE1, "light1_on", GECmdFormat::FLAG },
	{ GE_CMD_LIGHTENABLE2, "light2_on", GECmdFormat::FLAG },
	{ GE_CMD_LIGHTENABLE3, "light3_on", GECmdFormat::FLAG },
	{ GE_CMD_DEPTHCLAMPENABLE, "zclamp_on", GECmdFormat::FLAG },
	{ GE_CMD_CULLFACEENABLE, "cull_on", GECmdFormat::FLAG },
	{ GE_CMD_TEXTUREMAPENABLE, "tex_on", GECmdFormat::FLAG },
	{ GE_CMD_FOGENABLE, "fog_on", GECmdFormat::FLAG },
	{ GE_CMD_DITHERENABLE, "dither_on", GECmdFormat::FLAG },
	{ GE_CMD_ALPHABLENDENABLE, "ablend_on", GECmdFormat::FLAG },
	{ GE_CMD_ALPHABLENDENABLE, "atest_on", GECmdFormat::FLAG },
	{ GE_CMD_ZTESTENABLE, "ztest_on", GECmdFormat::FLAG },
	{ GE_CMD_STENCILTESTENABLE, "stest_on", GECmdFormat::FLAG },
	{ GE_CMD_ANTIALIASENABLE, "antialias_on", GECmdFormat::FLAG },
	{ GE_CMD_PATCHCULLENABLE, "patchcull_on", GECmdFormat::FLAG },
	{ GE_CMD_COLORTESTENABLE, "ctest_on", GECmdFormat::FLAG },
	{ GE_CMD_LOGICOPENABLE, "logicop_on", GECmdFormat::FLAG },
	{ GE_CMD_UNKNOWN_29, "unknown29", GECmdFormat::NONE },
	{ GE_CMD_BONEMATRIXNUMBER, "bonemtxnum", GECmdFormat::BONE_NUM },
	{ GE_CMD_BONEMATRIXDATA, "bonemtxdata", GECmdFormat::FLOAT },
	{ GE_CMD_MORPHWEIGHT0, "morph0", GECmdFormat::FLOAT },
	{ GE_CMD_MORPHWEIGHT1, "morph1", GECmdFormat::FLOAT },
	{ GE_CMD_MORPHWEIGHT2, "morph2", GECmdFormat::FLOAT },
	{ GE_CMD_MORPHWEIGHT3, "morph3", GECmdFormat::FLOAT },
	{ GE_CMD_MORPHWEIGHT4, "morph4", GECmdFormat::FLOAT },
	{ GE_CMD_MORPHWEIGHT5, "morph5", GECmdFormat::FLOAT },
	{ GE_CMD_MORPHWEIGHT6, "morph6", GECmdFormat::FLOAT },
	{ GE_CMD_MORPHWEIGHT7, "morph7", GECmdFormat::FLOAT },
	{ GE_CMD_UNKNOWN_34, "unknown34", GECmdFormat::NONE },
	{ GE_CMD_UNKNOWN_35, "unknown35", GECmdFormat::NONE },
	{ GE_CMD_PATCHDIVISION, "patchdivision", GECmdFormat::PATCH_DIVISION },
	{ GE_CMD_PATCHPRIMITIVE, "patchprim", GECmdFormat::PATCH_PRIM },
	{ GE_CMD_PATCHFACING, "patchreversenormals", GECmdFormat::FLAG },
	{ GE_CMD_UNKNOWN_39, "unknown39", GECmdFormat::NONE },
	{ GE_CMD_WORLDMATRIXNUMBER, "worldmtxnum", GECmdFormat::MATRIX_NUM },
	{ GE_CMD_WORLDMATRIXDATA, "worldmtxdata", GECmdFormat::FLOAT },
	{ GE_CMD_VIEWMATRIXNUMBER, "viewmtxnum", GECmdFormat::MATRIX_NUM },
	{ GE_CMD_VIEWMATRIXDATA, "viewmtxdata", GECmdFormat::FLOAT },
	{ GE_CMD_PROJMATRIXNUMBER, "projmtxnum", GECmdFormat::MATRIX_NUM },
	{ GE_CMD_PROJMATRIXDATA, "projmtxdata", GECmdFormat::FLOAT },
	{ GE_CMD_TGENMATRIXNUMBER, "texgenmtxnum", GECmdFormat::MATRIX_NUM },
	{ GE_CMD_TGENMATRIXDATA, "texgenmtxdata", GECmdFormat::FLOAT },
	{ GE_CMD_VIEWPORTXSCALE, "vpxscale", GECmdFormat::FLOAT },
	{ GE_CMD_VIEWPORTYSCALE, "vpyscale", GECmdFormat::FLOAT },
	{ GE_CMD_VIEWPORTZSCALE, "vpzscale", GECmdFormat::FLOAT },
	{ GE_CMD_VIEWPORTXCENTER, "vpxcenter", GECmdFormat::FLOAT },
	{ GE_CMD_VIEWPORTYCENTER, "vpycenter", GECmdFormat::FLOAT },
	{ GE_CMD_VIEWPORTZCENTER, "vpzcenter", GECmdFormat::FLOAT },
	{ GE_CMD_TEXSCALEU, "texscaleu", GECmdFormat::FLOAT },
	{ GE_CMD_TEXSCALEV, "texscalev", GECmdFormat::FLOAT },
	{ GE_CMD_TEXOFFSETU, "texoffsetu", GECmdFormat::FLOAT },
	{ GE_CMD_TEXOFFSETV, "texoffsetv", GECmdFormat::FLOAT },
	{ GE_CMD_OFFSETX, "offsetx", GECmdFormat::SUBPIXEL_COORD },
	{ GE_CMD_OFFSETY, "offsety", GECmdFormat::SUBPIXEL_COORD },
	{ GE_CMD_UNKNOWN_4E, "unknown4e", GECmdFormat::NONE },
	{ GE_CMD_UNKNOWN_4F, "unknown4f", GECmdFormat::NONE },
	// Really shade mode, but using gouraud as the default so it can be 1/0.
	{ GE_CMD_SHADEMODE, "gouraud", GECmdFormat::FLAG },
	{ GE_CMD_REVERSENORMAL, "reversenormals", GECmdFormat::FLAG },
	{ GE_CMD_UNKNOWN_52, "unknown52", GECmdFormat::NONE },
	{ GE_CMD_MATERIALUPDATE, "materialupdate", GECmdFormat::MATERIAL_UPDATE },
	{ GE_CMD_MATERIALEMISSIVE, "materialemissive", GECmdFormat::RGB },
	{ GE_CMD_MATERIALAMBIENT, "materialambient", GECmdFormat::RGB },
	{ GE_CMD_MATERIALDIFFUSE, "materialdiffuse", GECmdFormat::RGB },
	{ GE_CMD_MATERIALSPECULAR, "materialspecular", GECmdFormat::RGB },
	{ GE_CMD_MATERIALALPHA, "materialambienta", GECmdFormat::DATA8 },
	{ GE_CMD_UNKNOWN_59, "unknown59", GECmdFormat::NONE },
	{ GE_CMD_UNKNOWN_5A, "unknown5a", GECmdFormat::NONE },
	{ GE_CMD_MATERIALSPECULARCOEF, "specularcoef", GECmdFormat::FLOAT },
	{ GE_CMD_AMBIENTCOLOR, "ambient", GECmdFormat::RGB },
	{ GE_CMD_AMBIENTALPHA, "ambienta", GECmdFormat::DATA8 },
	{ GE_CMD_LIGHTMODE, "lightseparate", GECmdFormat::FLAG },
	{ GE_CMD_LIGHTTYPE0, "ltype0", GECmdFormat::LIGHT_TYPE },
	{ GE_CMD_LIGHTTYPE1, "ltype1", GECmdFormat::LIGHT_TYPE },
	{ GE_CMD_LIGHTTYPE2, "ltype2", GECmdFormat::LIGHT_TYPE },
	{ GE_CMD_LIGHTTYPE3, "ltype3", GECmdFormat::LIGHT_TYPE },
	{ GE_CMD_LX0, "light0posx", GECmdFormat::FLOAT },
	{ GE_CMD_LY0, "light0posy", GECmdFormat::FLOAT },
	{ GE_CMD_LZ0, "light0posz", GECmdFormat::FLOAT },
	{ GE_CMD_LX1, "light1posx", GECmdFormat::FLOAT },
	{ GE_CMD_LY1, "light1posy", GECmdFormat::FLOAT },
	{ GE_CMD_LZ1, "light1posz", GECmdFormat::FLOAT },
	{ GE_CMD_LX2, "light2posx", GECmdFormat::FLOAT },
	{ GE_CMD_LY2, "light2posy", GECmdFormat::FLOAT },
	{ GE_CMD_LZ2, "light2posz", GECmdFormat::FLOAT },
	{ GE_CMD_LX3, "light3posx", GECmdFormat::FLOAT },
	{ GE_CMD_LY3, "light3posy", GECmdFormat::FLOAT },
	{ GE_CMD_LZ3, "light3posz", GECmdFormat::FLOAT },
	{ GE_CMD_LDX0, "light0dirx", GECmdFormat::FLOAT },
	{ GE_CMD_LDY0, "light0diry", GECmdFormat::FLOAT },
	{ GE_CMD_LDZ0, "light0dirz", GECmdFormat::FLOAT },
	{ GE_CMD_LDX1, "light1dirx", GECmdFormat::FLOAT },
	{ GE_CMD_LDY1, "light1diry", GECmdFormat::FLOAT },
	{ GE_CMD_LDZ1, "light1dirz", GECmdFormat::FLOAT },
	{ GE_CMD_LDX2, "light2dirx", GECmdFormat::FLOAT },
	{ GE_CMD_LDY2, "light2diry", GECmdFormat::FLOAT },
	{ GE_CMD_LDZ2, "light2dirz", GECmdFormat::FLOAT },
	{ GE_CMD_LDX3, "light3dirx", GECmdFormat::FLOAT },
	{ GE_CMD_LDY3, "light3diry", GECmdFormat::FLOAT },
	{ GE_CMD_LDZ3, "light3dirz", GECmdFormat::FLOAT },
	{ GE_CMD_LKA0, "light0attpow0", GECmdFormat::FLOAT },
	{ GE_CMD_LKB0, "light0attpow1", GECmdFormat::FLOAT },
	{ GE_CMD_LKC0, "light0attpow2", GECmdFormat::FLOAT },
	{ GE_CMD_LKA1, "light1attpow0", GECmdFormat::FLOAT },
	{ GE_CMD_LKB1, "light1attpow1", GECmdFormat::FLOAT },
	{ GE_CMD_LKC1, "light1attpow2", GECmdFormat::FLOAT },
	{ GE_CMD_LKA2, "light2attpow0", GECmdFormat::FLOAT },
	{ GE_CMD_LKB2, "light2attpow1", GECmdFormat::FLOAT },
	{ GE_CMD_LKC2, "light2attpow2", GECmdFormat::FLOAT },
	{ GE_CMD_LKA3, "light3attpow0", GECmdFormat::FLOAT },
	{ GE_CMD_LKB3, "light3attpow1", GECmdFormat::FLOAT },
	{ GE_CMD_LKC3, "light3attpow2", GECmdFormat::FLOAT },
	{ GE_CMD_LKS0, "light0spotexp", GECmdFormat::FLOAT },
	{ GE_CMD_LKS1, "light1spotexp", GECmdFormat::FLOAT },
	{ GE_CMD_LKS2, "light2spotexp", GECmdFormat::FLOAT },
	{ GE_CMD_LKS3, "light3spotexp", GECmdFormat::FLOAT },
	{ GE_CMD_LKO0, "light0spotcutoff", GECmdFormat::FLOAT },
	{ GE_CMD_LKO1, "light1spotcutoff", GECmdFormat::FLOAT },
	{ GE_CMD_LKO2, "light2spotcutoff", GECmdFormat::FLOAT },
	{ GE_CMD_LKO3, "light3spotcutoff", GECmdFormat::FLOAT },
	{ GE_CMD_LAC0, "light0ambient", GECmdFormat::RGB },
	{ GE_CMD_LDC0, "light0diffuse", GECmdFormat::RGB },
	{ GE_CMD_LSC0, "light0specular", GECmdFormat::RGB },
	{ GE_CMD_LAC1, "light1ambient", GECmdFormat::RGB },
	{ GE_CMD_LDC1, "light1diffuse", GECmdFormat::RGB },
	{ GE_CMD_LSC1, "light1specular", GECmdFormat::RGB },
	{ GE_CMD_LAC2, "light2ambient", GECmdFormat::RGB },
	{ GE_CMD_LDC2, "light2diffuse", GECmdFormat::RGB },
	{ GE_CMD_LSC2, "light2specular", GECmdFormat::RGB },
	{ GE_CMD_LAC3, "light3ambient", GECmdFormat::RGB },
	{ GE_CMD_LDC3, "light3diffuse", GECmdFormat::RGB },
	{ GE_CMD_LSC3, "light3specular", GECmdFormat::RGB },
	{ GE_CMD_CULL, "cullccw", GECmdFormat::FLAG },
	{ GE_CMD_FRAMEBUFPTR, "fbptr", GECmdFormat::LOW_ADDR_ONLY },
	{ GE_CMD_FRAMEBUFWIDTH, "fbstride", GECmdFormat::STRIDE },
	{ GE_CMD_ZBUFPTR, "zbptr", GECmdFormat::LOW_ADDR_ONLY },
	{ GE_CMD_ZBUFWIDTH, "zbstride", GECmdFormat::STRIDE },
	{ GE_CMD_TEXADDR0, "texaddr0", GECmdFormat::LOW_ADDR },
	{ GE_CMD_TEXADDR1, "texaddr1", GECmdFormat::LOW_ADDR },
	{ GE_CMD_TEXADDR2, "texaddr2", GECmdFormat::LOW_ADDR },
	{ GE_CMD_TEXADDR3, "texaddr3", GECmdFormat::LOW_ADDR },
	{ GE_CMD_TEXADDR4, "texaddr4", GECmdFormat::LOW_ADDR },
	{ GE_CMD_TEXADDR5, "texaddr5", GECmdFormat::LOW_ADDR },
	{ GE_CMD_TEXADDR6, "texaddr6", GECmdFormat::LOW_ADDR },
	{ GE_CMD_TEXADDR7, "texaddr7", GECmdFormat::LOW_ADDR },
	{ GE_CMD_TEXBUFWIDTH0, "texbufw0", GECmdFormat::STRIDE_HIGH_ADDR },
	{ GE_CMD_TEXBUFWIDTH1, "texbufw1", GECmdFormat::STRIDE_HIGH_ADDR },
	{ GE_CMD_TEXBUFWIDTH2, "texbufw2", GECmdFormat::STRIDE_HIGH_ADDR },
	{ GE_CMD_TEXBUFWIDTH3, "texbufw3", GECmdFormat::STRIDE_HIGH_ADDR },
	{ GE_CMD_TEXBUFWIDTH4, "texbufw4", GECmdFormat::STRIDE_HIGH_ADDR },
	{ GE_CMD_TEXBUFWIDTH5, "texbufw5", GECmdFormat::STRIDE_HIGH_ADDR },
	{ GE_CMD_TEXBUFWIDTH6, "texbufw6", GECmdFormat::STRIDE_HIGH_ADDR },
	{ GE_CMD_TEXBUFWIDTH7, "texbufw7", GECmdFormat::STRIDE_HIGH_ADDR },
	{ GE_CMD_CLUTADDR, "clutaddr", GECmdFormat::LOW_ADDR },
	{ GE_CMD_CLUTADDRUPPER, "clutaddrhigh", GECmdFormat::HIGH_ADDR },
	{ GE_CMD_TRANSFERSRC, "transfersrc", GECmdFormat::LOW_ADDR },
	{ GE_CMD_TRANSFERSRCW, "transfersrcstride", GECmdFormat::STRIDE_HIGH_ADDR },
	{ GE_CMD_TRANSFERDST, "transferdst", GECmdFormat::LOW_ADDR },
	{ GE_CMD_TRANSFERDSTW, "transferdststride", GECmdFormat::STRIDE_HIGH_ADDR },
	{ GE_CMD_UNKNOWN_B6, "unknownb6", GECmdFormat::NONE },
	{ GE_CMD_UNKNOWN_B7, "unknownb7", GECmdFormat::NONE },
	{ GE_CMD_TEXSIZE0, "texsize0", GECmdFormat::TEX_SIZE },
	{ GE_CMD_TEXSIZE1, "texsize1", GECmdFormat::TEX_SIZE },
	{ GE_CMD_TEXSIZE2, "texsize2", GECmdFormat::TEX_SIZE },
	{ GE_CMD_TEXSIZE3, "texsize3", GECmdFormat::TEX_SIZE },
	{ GE_CMD_TEXSIZE4, "texsize4", GECmdFormat::TEX_SIZE },
	{ GE_CMD_TEXSIZE5, "texsize5", GECmdFormat::TEX_SIZE },
	{ GE_CMD_TEXSIZE6, "texsize6", GECmdFormat::TEX_SIZE },
	{ GE_CMD_TEXSIZE7, "texsize7", GECmdFormat::TEX_SIZE },
	{ GE_CMD_TEXMAPMODE, "texmapmode", GECmdFormat::TEX_MAP_MODE },
	{ GE_CMD_TEXSHADELS, "texlightsrc", GECmdFormat::TEX_LIGHT_SRC },
	{ GE_CMD_TEXMODE, "texmode", GECmdFormat::TEX_MODE },
	{ GE_CMD_TEXFORMAT, "texformat", GECmdFormat::TEX_FORMAT },
	{ GE_CMD_LOADCLUT, "loadclut", GECmdFormat::CLUT_BLOCKS },
	{ GE_CMD_CLUTFORMAT, "clutformat", GECmdFormat::CLUT_FORMAT },
	{ GE_CMD_TEXFILTER, "texfilter", GECmdFormat::TEX_FILTER },
	{ GE_CMD_TEXWRAP, "texclamp", GECmdFormat::TEX_CLAMP },
	{ GE_CMD_TEXLEVEL, "texlevelmode", GECmdFormat::TEX_LEVEL_MODE },
	{ GE_CMD_TEXFUNC, "texfunc", GECmdFormat::TEX_FUNC },
	{ GE_CMD_TEXENVCOLOR, "texenv", GECmdFormat::RGB },
	{ GE_CMD_TEXFLUSH, "texflush", GECmdFormat::NONE },
	{ GE_CMD_TEXSYNC, "texsync", GECmdFormat::NONE },
	{ GE_CMD_FOG1, "fogend", GECmdFormat::FLOAT },
	{ GE_CMD_FOG2, "fogslope", GECmdFormat::FLOAT },
	{ GE_CMD_FOGCOLOR, "fogcolor", GECmdFormat::RGB },
	{ GE_CMD_TEXLODSLOPE, "texlodslope", GECmdFormat::FLOAT },
	{ GE_CMD_UNKNOWN_D1, "unknownd1", GECmdFormat::NONE },
	{ GE_CMD_FRAMEBUFPIXFORMAT, "fbformat", GECmdFormat::TEX_FORMAT },
	{ GE_CMD_CLEARMODE, "clearmode", GECmdFormat::CLEAR_MODE },
	{ GE_CMD_SCISSOR1, "scissor1", GECmdFormat::X10_Y10 },
	{ GE_CMD_SCISSOR2, "scissor2", GECmdFormat::X10_Y10 },
	{ GE_CMD_MINZ, "minz", GECmdFormat::DATA16 },
	{ GE_CMD_MAXZ, "maxz", GECmdFormat::DATA16 },
	{ GE_CMD_COLORTEST, "ctestfunc", GECmdFormat::COLOR_TEST_FUNC },
	{ GE_CMD_COLORREF, "ctestref", GECmdFormat::RGB },
	{ GE_CMD_COLORTESTMASK, "ctestmask", GECmdFormat::RGB },
	{ GE_CMD_ALPHATEST, "atest", GECmdFormat::ALPHA_TEST },
	{ GE_CMD_STENCILTEST, "stest", GECmdFormat::ALPHA_TEST },
	{ GE_CMD_STENCILOP, "stencilop", GECmdFormat::STENCIL_OP },
	{ GE_CMD_ZTEST, "ztest", GECmdFormat::DEPTH_TEST_FUNC },
	{ GE_CMD_BLENDMODE, "blendmode", GECmdFormat::BLEND_MODE },
	{ GE_CMD_BLENDFIXEDA, "blendfixa", GECmdFormat::RGB },
	{ GE_CMD_BLENDFIXEDB, "blendfixb", GECmdFormat::RGB },
	{ GE_CMD_DITH0, "dither0", GECmdFormat::DITHER_ROW },
	{ GE_CMD_DITH1, "dither1", GECmdFormat::DITHER_ROW },
	{ GE_CMD_DITH2, "dither2", GECmdFormat::DITHER_ROW },
	{ GE_CMD_DITH3, "dither3", GECmdFormat::DITHER_ROW },
	{ GE_CMD_LOGICOP, "logicop", GECmdFormat::LOGIC_OP },
	{ GE_CMD_ZWRITEDISABLE, "zwrite_off", GECmdFormat::FLAG },
	{ GE_CMD_MASKRGB, "rgbmask_block", GECmdFormat::RGB },
	{ GE_CMD_MASKALPHA, "swritemask_block", GECmdFormat::DATA8 },
	{ GE_CMD_TRANSFERSTART, "transferstart_bpp", GECmdFormat::FLAG },
	{ GE_CMD_TRANSFERSRCPOS, "transfersrcpos", GECmdFormat::X10_Y10 },
	{ GE_CMD_TRANSFERDSTPOS, "transferdstpos", GECmdFormat::X10_Y10 },
	{ GE_CMD_UNKNOWN_ED, "unknowned", GECmdFormat::NONE },
	{ GE_CMD_TRANSFERSIZE, "transfersize", GECmdFormat::X10_Y10 },
	{ GE_CMD_UNKNOWN_EF, "unknownef", GECmdFormat::NONE },
	{ GE_CMD_VSCX, "immx", GECmdFormat::SUBPIXEL_COORD },
	{ GE_CMD_VSCY, "immy", GECmdFormat::SUBPIXEL_COORD },
	{ GE_CMD_VSCZ, "immz", GECmdFormat::DATA16 },
	{ GE_CMD_VTCS, "imms", GECmdFormat::FLOAT },
	{ GE_CMD_VTCT, "immt", GECmdFormat::FLOAT },
	{ GE_CMD_VTCQ, "immq", GECmdFormat::FLOAT },
	{ GE_CMD_VCV, "immrgb", GECmdFormat::RGB },
	{ GE_CMD_VAP, "imma_prim", GECmdFormat::ALPHA_PRIM },
	{ GE_CMD_VFC, "immfog", GECmdFormat::DATA8 },
	{ GE_CMD_VSCV, "immrgb1", GECmdFormat::RGB },
	{ GE_CMD_UNKNOWN_FA, "unknownfa", GECmdFormat::NONE },
	{ GE_CMD_UNKNOWN_FB, "unknownfb", GECmdFormat::NONE },
	{ GE_CMD_UNKNOWN_FC, "unknownfc", GECmdFormat::NONE },
	{ GE_CMD_UNKNOWN_FD, "unknownfd", GECmdFormat::NONE },
	{ GE_CMD_UNKNOWN_FE, "unknownfe", GECmdFormat::NONE },
	{ GE_CMD_NOP_FF, "nopff", GECmdFormat::NONE },
};

static constexpr GECmdAlias geCmdAliases[] = {
	{ GE_CMD_VADDR, { "vertexaddr" } },
	{ GE_CMD_IADDR, { "indexaddr" } },
	{ GE_CMD_BOUNDINGBOX, { "boundingbox", "boundtest" } },
	{ GE_CMD_BJUMP, { "boundjump" } },
	{ GE_CMD_BASE, { "baseaddr" } },
	{ GE_CMD_VERTEXTYPE, { "vertextype" } },
	{ GE_CMD_OFFSETADDR, { "offsetaddr" } },
	{ GE_CMD_REGION2, { "region2" } },
	{ GE_CMD_LIGHTINGENABLE, { "lightingenable", "lighting" } },
	{ GE_CMD_LIGHTENABLE0, { "light0enable" } },
	{ GE_CMD_LIGHTENABLE1, { "light1enable" } },
	{ GE_CMD_LIGHTENABLE2, { "light2enable" } },
	{ GE_CMD_LIGHTENABLE3, { "light3enable" } },
	{ GE_CMD_DEPTHCLAMPENABLE, { "zclampenable", "depthclamp_on", "depthclampenable" } },
	{ GE_CMD_CULLFACEENABLE, { "cullenable", "cullface_on", "cullfaceenable" } },
	{ GE_CMD_TEXTUREMAPENABLE, { "texenable", "texture_on", "textureenable" } },
	{ GE_CMD_FOGENABLE, { "fogenable" } },
	{ GE_CMD_DITHERENABLE, { "ditherenable" } },
	{ GE_CMD_ALPHABLENDENABLE, { "ablendenable", "alphablend_on", "alphablendenable" } },
	{ GE_CMD_ALPHABLENDENABLE, { "atestenable", "alphatest_on", "alphatestenable" } },
	{ GE_CMD_ZTESTENABLE, { "ztestenable", "depthtest_on", "depthtest_enable" } },
	{ GE_CMD_STENCILTESTENABLE, { "stestenable", "stenciltest_on", "stenciltestenable" } },
	{ GE_CMD_ANTIALIASENABLE, { "antialiasenable", "antialias" } },
	{ GE_CMD_PATCHCULLENABLE, { "patchcullenable" } },
	{ GE_CMD_COLORTESTENABLE, { "ctestenable", "colortest_on", "colortestenable" } },
	{ GE_CMD_LOGICOPENABLE, { "logicopenable" } },
	{ GE_CMD_BONEMATRIXNUMBER, { "bonematrixnum" } },
	{ GE_CMD_BONEMATRIXDATA, { "bonematrixdata" } },
	{ GE_CMD_MORPHWEIGHT0, { "morphweight0" } },
	{ GE_CMD_MORPHWEIGHT1, { "morphweight1" } },
	{ GE_CMD_MORPHWEIGHT2, { "morphweight2" } },
	{ GE_CMD_MORPHWEIGHT3, { "morphweight3" } },
	{ GE_CMD_MORPHWEIGHT4, { "morphweight4" } },
	{ GE_CMD_MORPHWEIGHT5, { "morphweight5" } },
	{ GE_CMD_MORPHWEIGHT6, { "morphweight6" } },
	{ GE_CMD_MORPHWEIGHT7, { "morphweight7" } },
	{ GE_CMD_PATCHDIVISION, { "patchdiv" } },
	{ GE_CMD_PATCHFACING, { "patchreversenormal" } },
	{ GE_CMD_WORLDMATRIXNUMBER, { "worldmatrixnum" } },
	{ GE_CMD_WORLDMATRIXDATA, { "worldmatrixdata" } },
	{ GE_CMD_VIEWMATRIXNUMBER, { "viewmatrixnum" } },
	{ GE_CMD_VIEWMATRIXDATA, { "viewmatrixdata" } },
	{ GE_CMD_PROJMATRIXNUMBER, { "projmatrixnum" } },
	{ GE_CMD_PROJMATRIXDATA, { "projmatrixdata" } },
	{ GE_CMD_TGENMATRIXNUMBER, { "texgenmatrixnum", "tgenmtxnum", "tgenmatrixnum" } },
	{ GE_CMD_TGENMATRIXDATA, { "texgenmatrixdata", "tgenmtxdata", "tgenmatrixdata" } },
	{ GE_CMD_VIEWPORTXSCALE, { "viewportxscale" } },
	{ GE_CMD_VIEWPORTYSCALE, { "viewportyscale" } },
	{ GE_CMD_VIEWPORTZSCALE, { "viewportzscale" } },
	{ GE_CMD_VIEWPORTXCENTER, { "viewportxcenter" } },
	{ GE_CMD_VIEWPORTYCENTER, { "viewportycenter" } },
	{ GE_CMD_VIEWPORTZCENTER, { "viewportzcenter" } },
	{ GE_CMD_SHADEMODE, { "shademode", "shading" } },
	{ GE_CMD_REVERSENORMAL, { "reversenormal" } },
	{ GE_CMD_MATERIALAMBIENT, { "materialambientrgb" } },
	{ GE_CMD_MATERIALALPHA, { "materialambientalpha" } },
	{ GE_CMD_MATERIALSPECULARCOEF, { "materialspecularcoef" } },
	{ GE_CMD_AMBIENTCOLOR, { "ambientrgb" } },
	{ GE_CMD_AMBIENTALPHA, { "ambientalpha" } },
	{ GE_CMD_LIGHTMODE, { "lmode", "secondarycolor" } },
	{ GE_CMD_LIGHTTYPE0, { "lighttype0" } },
	{ GE_CMD_LIGHTTYPE1, { "lighttype1" } },
	{ GE_CMD_LIGHTTYPE2, { "lighttype2" } },
	{ GE_CMD_LIGHTTYPE3, { "lighttype3" } },
	{ GE_CMD_FRAMEBUFPTR, { "framebufptr" } },
	{ GE_CMD_FRAMEBUFWIDTH, { "fbwidth", "framebufstride", "framebufwidth" } },
	{ GE_CMD_ZBUFPTR, { "depthbufptr" } },
	{ GE_CMD_ZBUFWIDTH, { "zbwidth", "depthbufstride", "depthbufwidth" } },
	{ GE_CMD_TEXBUFWIDTH0, { "texbufwidth0", "texstride0" } },
	{ GE_CMD_TEXBUFWIDTH1, { "texbufwidth1", "texstride1" } },
	{ GE_CMD_TEXBUFWIDTH2, { "texbufwidth2", "texstride2" } },
	{ GE_CMD_TEXBUFWIDTH3, { "texbufwidth3", "texstride3" } },
	{ GE_CMD_TEXBUFWIDTH4, { "texbufwidth4", "texstride4" } },
	{ GE_CMD_TEXBUFWIDTH5, { "texbufwidth5", "texstride5" } },
	{ GE_CMD_TEXBUFWIDTH6, { "texbufwidth6", "texstride6" } },
	{ GE_CMD_TEXBUFWIDTH7, { "texbufwidth7", "texstride7" } },
	{ GE_CMD_CLUTADDRUPPER, { "clutaddrupper" } },
	{ GE_CMD_TEXSHADELS, { "texshadels" } },
	{ GE_CMD_TEXWRAP, { "texwrap" } },
	{ GE_CMD_FOGCOLOR, { "fogrgb" } },
	{ GE_CMD_FRAMEBUFPIXFORMAT, { "framebufformat" } },
	{ GE_CMD_CLEARMODE, { "clear" } },
	{ GE_CMD_SCISSOR1, { "scissortl" } },
	{ GE_CMD_SCISSOR2, { "scissorbr" } },
	{ GE_CMD_COLORTEST, { "colortestfunc" } },
	{ GE_CMD_COLORREF, { "colortestref" } },
	{ GE_CMD_COLORTESTMASK, { "colortestmask" } },
	{ GE_CMD_ALPHATEST, { "alphatest" } },
	{ GE_CMD_STENCILTEST, { "stenciltest" } },
	{ GE_CMD_ZTEST, { "depthtest" } },
	{ GE_CMD_BLENDFIXEDA, { "blendfixsrc" } },
	{ GE_CMD_BLENDFIXEDB, { "blendfixdst" } },
	{ GE_CMD_ZWRITEDISABLE, { "depthwrite_off", "zwritedisable", "depthwritedisable" } },
	{ GE_CMD_MASKRGB, { "rgbmask" } },
	{ GE_CMD_MASKALPHA, { "swritemask", "amask", "amask_block" } },
	{ GE_CMD_TRANSFERSTART, { "transferstart" } },
	{ GE_CMD_VCV, { "immrgb0" } },
	{ GE_CMD_VSCV, { "immsecondaryrgb" } },
};

bool GECmdInfoByName(const char *name, GECmdInfo &result) {
	for (const GECmdInfo &info : geCmdInfo) {
		if (strcasecmp(info.name, name) == 0) {
			result = info;
			return true;
		}
	}

	for (const GECmdAlias &entry : geCmdAliases) {
		for (const char *alias : entry.aliases) {
			if (alias && strcasecmp(alias, name) == 0) {
				result = GECmdInfoByCmd(entry.reg);
				return true;
			}
		}
	}

	return false;
}

GECmdInfo GECmdInfoByCmd(GECommand reg) {
	_assert_msg_((reg & 0xFF) == reg, "Invalid reg");
	return geCmdInfo[reg & 0xFF];
}
