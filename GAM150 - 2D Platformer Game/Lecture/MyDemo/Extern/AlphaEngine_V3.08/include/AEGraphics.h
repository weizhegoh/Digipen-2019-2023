/******************************************************************************/
/*!
\par        Project: Alpha Engine
\file       AEGraphics.h

\author     Sun Tjen Fam
\date       January 30, 2008

\brief      Header file for the graphics library.

\copyright  Copyright (C) 2013 DigiPen Institute of Technology. Reproduction 
            or disclosure of this file or its contents without the prior 
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#ifndef AE_GRAPHICS_H
#define AE_GRAPHICS_H

// ---------------------------------------------------------------------------
// Defines

/*! AEEngine render option */
typedef	enum
{
	AE_GFX_RM_NONE,         //!< No render
	AE_GFX_RM_COLOR,        //!< Color rendering
	AE_GFX_RM_TEXTURE,      //!< Texture rendering
	
	AE_GFX_RM_NUM
}AEGfxRenderMode;


// ---------------------------------------------------------------------------

/*! AEEngine blend option */
typedef enum
{
	AE_GFX_BM_NONE = 0,     //!< No blending
	AE_GFX_BM_BLEND,        //!< Color blending
	AE_GFX_BM_ADD,          //!< Additive blending

	AE_GFX_BM_NUM
}AEGfxBlendMode;

// ---------------------------------------------------------------------------

typedef enum
{
	AE_GFX_TM_PRECISE = 0,
	AE_GFX_TM_AVERAGE
}AEGfxTextureMode;

// ---------------------------------------------------------------------------

typedef enum
{
	AE_GFX_MDM_POINTS = 0,
	AE_GFX_MDM_LINES,
	AE_GFX_MDM_LINES_STRIP,
	AE_GFX_MDM_TRIANGLES,
	
	// Keep this one last
	AE_GFX_MDM_NUM
}AEGfxMeshDrawMode;

// ---------------------------------------------------------------------------
// Struct/Class definitions

typedef struct AEGfxVertexBuffer AEGfxVertexBuffer;

typedef struct AEGfxVertexList
{
	AEGfxVertexBuffer		*mpVtxBuffer;
	u32						vtxNum;
}AEGfxVertexList;

// ---------------------------------------------------------------------------

typedef struct AEGfxSurface AEGfxSurface;

typedef struct AEGfxTexture
{
	AEGfxSurface *mpSurface;
	s8 mpName[256];
}AEGfxTexture;

// ---------------------------------------------------------------------------
// Extern variables

extern bool gfullScreen;

extern f32 gResolutionRatioX;
extern f32 gResolutionRatioY;

// ---------------------------------------------------------------------------
// Function prototypes
// ---------------------------------------------------------------------------

#ifdef __cplusplus

extern "C"
{
#endif

// ---------------------------------------------------------------------------

/******************************************************************************/
/*!
\fn		    s32 AEGfxInit(s32 Width, 
                          s32 Height)

\brief      Initialize AEGraphics.

\warning    This function is already called in AESysInit.

\param      [in] Width
            Set the width of the window.

\param      [in] Height
            Set the height of the window. 

\retval     s32
            Return 1 if initialization is successful.
            Else return 0.
*/
/******************************************************************************/
AE_API s32 AEGfxInit(s32 Width, s32 Height);

/******************************************************************************/
/*!
\fn         void SetVSync(bool vsync)

\brief      Enable/Disable VSync

\warning    

\retval     void
No return.
*/
/******************************************************************************/
AE_API void SetVSync(bool vsync);

/******************************************************************************/
/*!
\fn         void AEGfxReset()

\brief      Reset AEGraphics.

\warning    This function is already called in AESysReset.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxReset();

/******************************************************************************/
/*!
\fn         void AEGfxExit()

\brief      Free AEGraphics.

\warning    This function is already called in AESysExit.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxExit();

/******************************************************************************/
/*!
\fn         void AEGfxStart()

\brief      Tell AEGraphics that a new frame is starting.

\warning    This function is already called in AESysFrameStart.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxStart();

/******************************************************************************/
/*!
\fn         void AEGfxEnd()

\brief      Tell AEGraphics that the current frame is ending.

\warning    This function is already called in AESysFrameEnd.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxEnd();

/******************************************************************************/
/*!
\fn         void AEGfxSetBackgroundColor(f32 Red, 
                                         f32 Green, 
                                         f32 Blue)

\brief      Set the background colour to selected RGB values.

\param      [in] Red
            Percentage of red. Range from 0.0f to 1.0f.

\param      [in] Green
            Percentage of green. Range from 0.0f to 1.0f.

\param      [in] Blue
            Percentage of blue. Range from 0.0f to 1.0f.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxSetBackgroundColor(f32 Red, f32 Green, f32 Blue);

/******************************************************************************/
/*!
\fn         void AEGfxSetRenderMode(AEGfxRenderMode RenderMode)

\brief      Set the render mode.

\warning    This function should be called at least once per frame,
            before any rendering is done.

\param      [in] RenderMode
            The AEGfxRenderMode to set.
            

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxSetRenderMode(AEGfxRenderMode RenderMode);

/******************************************************************************/
/*!
\fn         void AEGfxSetBlendMode(AEGfxBlendMode BlendMode)

\brief      Set the blend mode.

\param      [in] BlendMode
            The AEGfxBlendMode to set.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void	AEGfxSetBlendMode(AEGfxBlendMode BlendMode);


AE_API int AEGetWindowWidth();
AE_API int AEGetWindowHeight();



/******************************************************************************/
/*!
\fn         f32 AEGfxGetWinMinX(void)

\brief      Get the minimum X world coordinate.

\retval     f32
            Returns the minimum X world coordinate.
*/
/******************************************************************************/
AE_API f32 AEGfxGetWinMinX(void);

/******************************************************************************/
/*!
\fn         f32 AEGfxGetWinMaxX(void)

\brief      Get the maximum X world coordinate.

\retval     f32
            Returns the maximum X world coordinate.
*/
/******************************************************************************/
AE_API f32 AEGfxGetWinMaxX(void);

/******************************************************************************/
/*!
\fn         f32 AEGfxGetWinMinY(void)

\brief      Get the minimum Y world coordinate.

\retval     f32
            Returns the minimum Y world coordinate.
*/
/******************************************************************************/
AE_API f32 AEGfxGetWinMinY(void);

/******************************************************************************/
/*!
\fn         f32 AEGfxGetWinMaxY(void)

\brief      Get the maximum Y world coordinate.

\retval     f32
            Returns the maximum Y world coordinate.
*/
/******************************************************************************/
AE_API f32 AEGfxGetWinMaxY(void);

/******************************************************************************/
/*!
\fn         void AEGfxSetCamPosition(f32 X, f32 Y)

\brief      Set the camera's X and Y position.

\param      [in] X
            X position to set camera to.

\param      [in] Y
            Y position to set camera to.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxSetCamPosition(f32 X, f32 Y);

/******************************************************************************/
/*!
\fn         void AEGfxGetCamPosition(f32 *pX, f32 *pY)

\brief      Get the camera's X and Y position.

\param      [out] pX
            Pointer to f32 to store the X position in.

\param      [out] pY
            Pointer to f32 to store the Y position in.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxGetCamPosition(f32 *pX, f32 *pY);

// Sets/Gets the camera distance, used to zoom in/out
//AE_API void AEGfxSetCamZoom(f32 Distance);
//AE_API f32 AEGfxGetCamZoom();

/******************************************************************************/
/*!
\fn         void AEGfxSetPosition(f32 X, f32 Y)

\brief      Set the position (translation) to render from.

\param      [in] X
            X position to render from.

\param      [out] Y
            Y position to render from.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxSetPosition(f32 X, f32 Y);

/******************************************************************************/
/*!
\fn		    void AEGfxSetTransform(f32 pTransform[3][3])

\brief      Set the new global transformation matrix.

\details    The new matrix will be applied to all object drawn
            after this function was called.

\param      [in] pTransform
            Pointer to a 3x3 matrix to set with.
            User may pass in a AEMtx33.m. 

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxSetTransform(f32 pTransform[3][3]);

/******************************************************************************/
/*!
\fn		    void AEGfxSetTransform3D(f32 pTransform[4][4])

\brief      Set the new global transformation matrix.

\details    The new matrix will be applied to all object drawn
            after this function was called.

\param      [in] pTransform
            Pointer to a 4x4 matrix to set with.
            
\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxSetTransform3D(f32 pTransform[4][4]);

/******************************************************************************/
/*!
\fn		    void AEGfxSetTransparency(float Alpha)

\brief      Set the new global transparency value.

\details    The new transparency value will be applied to all object 
            drawn after this function was called.

\param      [in] Alpha
            Percentage of alpha (transparency). 
            Range from 0.0f (clear) to 1.0f (opaque).

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxSetTransparency(f32 Alpha);

/******************************************************************************/
/*!
\fn		    void AEGfxSetBlendColor(f32 Red, 
                                    f32 Green, 
                                    f32 Blue, 
                                    f32 Alpha)

\brief      Sets a color (RGBA) that will be used to blend with the 
            original material.

\param      [in] Red
            Percentage of red. Range from 0.0f to 1.0f.

\param      [in] Green
            Percentage of green. Range from 0.0f to 1.0f.

\param      [in] Blue
            Percentage of blue. Range from 0.0f to 1.0f.

\param      [in] Alpha
            Percentage of alpha (transparency). 
            Range from 0.0f (clear) to 1.0f (opaque).

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxSetBlendColor(f32 Red, f32 Green, f32 Blue, f32 Alpha);

/******************************************************************************/
/*!
\fn		    void AEGfxSetTintColor(f32 Red, 
                                   f32 Green, 
                                   f32 Blue, 
                                   f32 Alpha)

\brief      Sets a color (RGBA) that will be used to tint 
            the original material.

\param      [in] Red
            Percentage of red. Range from 0.0f to 1.0f.

\param      [in] Green
            Percentage of green. Range from 0.0f to 1.0f.

\param      [in] Blue
            Percentage of blue. Range from 0.0f to 1.0f.

\param      [in] Alpha
            Percentage of alpha (transparency). 
            Range from 0.0f (clear) to 1.0f (opaque).

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxSetTintColor(float Red, float Green, float Blue, float Alpha);

/******************************************************************************/
/*!
\fn		    void AEGfxMeshStart()

\brief      Instruct AEGraphics to start creating a new mesh.

\details    Call this function once before adding any points 
            to the mesh.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void		    	AEGfxMeshStart	();

/******************************************************************************/
/*!
\fn		    void AEGfxTriAdd(f32 x0, f32 y0, u32 c0, f32 tu0, f32 tv0,
							 f32 x1, f32 y1, u32 c1, f32 tu1, f32 tv1,
							 f32 x2, f32 y2, u32 c2, f32 tu2, f32 tv2)

\brief      Add a new triangle to the mesh. 

\details    The triangle is defined by 3 points: pt0, pt1 and pt2.
            The points are listed in a counter-clockwise order.
            The Z-value of the points are set to default 0.

\param      [in] x0
            X coordinate of pt0.

\param      [in] y0
            Y coordinate of pt0.

\param      [in] c0
            Color value of pt0.

\param      [in] tu0
            Texture translation of pt0.

\param      [in] tv0
            Texture translation of pt0.

\param      [in] x1
            X coordinate of pt1.

\param      [in] y1
            Y coordinate of pt1.

\param      [in] c1
            Color value of pt1.

\param      [in] tu1
            Texture translation of pt1.

\param      [in] tv1
            Texture translation of pt1.

\param      [in] x2
            X coordinate of pt2.

\param      [in] y2
            Y coordinate of pt2.

\param      [in] c2
            Color value of pt2.

\param      [in] tu2
            Texture translation of pt2.

\param      [in] tv2
            Texture translation of pt2.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void			    AEGfxTriAdd		(f32 x0, f32 y0, u32 c0, f32 tu0, f32 tv0,
									     f32 x1, f32 y1, u32 c1, f32 tu1, f32 tv1,
									     f32 x2, f32 y2, u32 c2, f32 tu2, f32 tv2);

/******************************************************************************/
/*!
\fn		    void AEGfxVertexAdd(f32 x0, 
                                f32 y0, 
                                u32 c0, 
                                f32 tu0, 
                                f32 tv0)

\brief      Add a new point to the mesh. 

\details    The Z-value of the point are set to default 0.

\param      [in] x0
            X coordinate of the point.

\param      [in] y0
            Y coordinate of the point.
            
\param      [in] c0
            Color value of the point.

\param      [in] tu0
            Texture translation of the point.

\param      [in] tv0
            Texture translation of the point.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void			    AEGfxVertexAdd	(f32 x0, f32 y0, u32 c0, f32 tu0, f32 tv0);

/******************************************************************************/
/*!
\fn		    AEGfxVertexList* AEGfxMeshEnd()

\brief      Instruct AEGraphics to end creating of new mesh.

\details    Call this function once after all the points have 
            been added to the mesh.

\retval     AEGfxVertexList*
            Returns a AEGfxVertexList pointer to the new mesh.
*/
/******************************************************************************/
AE_API AEGfxVertexList*	AEGfxMeshEnd	();

/******************************************************************************/
/*!
\fn		    void AEGfxMeshDraw(AEGfxVertexList* pVertexList, 
                               AEGfxMeshDrawMode MeshDrawMode)

\brief      Draw the mesh on screen.

\details    Render the mesh onto the screen using the MeshDrawMode
            stated.

\param      [in] pVertexList
            Pointer to the AEGfxVertexList to be rendered.

\param      [in] MeshDrawMode
            The AEGfxMeshDrawMode to use for rendering the mesh.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void				AEGfxMeshDraw	(AEGfxVertexList* pVertexList, AEGfxMeshDrawMode MeshDrawMode);

/******************************************************************************/
/*!
\fn		    void AEGfxMeshFree(AEGfxVertexList* pVertexList)

\brief      Free the mesh.

\details    Release the mesh from AEGraphics memory.
            The mesh is destroyed and can no longer be used.

\param      [in] pVertexList
            Pointer to the AEGfxVertexList to be free.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void				AEGfxMeshFree	(AEGfxVertexList* pVertexList);

/******************************************************************************/
/*!
\fn		    AEGfxTexture* AEGfxTextureLoad(const s8 *pFileName)

\brief      Load a texture file into memory.

\param      [in] pFileName
            Pointer to a null-terminated string containing the
            relative path of the file to be loaded.

\retval     AEGfxTexture*
            Returns a AEGfxTexture pointer to the loaded texture. 
*/
/******************************************************************************/
AE_API AEGfxTexture*	AEGfxTextureLoad(const s8 *pFileName);

/******************************************************************************/
/*!
\fn		    void AEGfxTextureSet(AEGfxTexture *pTexture, 
                                 f32 offset_x, 
                                 f32 offset_y)

\brief      Set a texture to be used for rendering.

\param      [in] pTexture
            Pointer to the AEGfxTexture to be used.
            Set to null if not using texture.

\param      [in] offset_x
            X offset for the texture. Range from 0.0f to 1.0f.

\param      [in] offset_y
            Y offset for the texture. Range from 0.0f to 1.0f.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void				AEGfxTextureSet(AEGfxTexture *pTexture, f32 offset_x, f32 offset_y);

/******************************************************************************/
/*!
\fn		    void AEGfxTextureUnload(AEGfxTexture *pTexture)

\brief      Unload a texture file from memory.

\param      [in] pTexture
            Pointer to AEGfxTexture to be unloaded.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void				AEGfxTextureUnload(AEGfxTexture *pTexture);

/******************************************************************************/
/*!
\fn		    AEGfxTexture* AEGfxTextureLoadFromMemory(u8 *pColors, 
                                                     u32 Width, 
                                                     u32 Height)

\brief      Load a texture from data in memory.

\param      [in] pColors
            Pointer to an array containing the data.

\param      [in] Width
            The width of the texture.

\param      [in] Height
            The height of the texture.

\retval     AEGfxTexture*
            Returns a AEGfxTexture pointer to the loaded texture.
*/
/******************************************************************************/
AE_API AEGfxTexture*	AEGfxTextureLoadFromMemory(u8 *pColors, u32 Width, u32 Height);

/******************************************************************************/
/*!
\fn		    void AEGfxSaveTextureToFile(AEGfxTexture* pTexture, 
                                        s8 *pFileName)

\brief      Save a texture from memory to file.

\param      [in] pTexture
            Pointer to AEGfxTexture to be saved.

\param      [in] pFileName
            Pointer to a null-terminated string containing the
            relative path of the file.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void				AEGfxSaveTextureToFile(AEGfxTexture* pTexture, s8 *pFileName);

/******************************************************************************/
/*!
\fn         void AEGfxSetTextureMode(AEGfxTextureMode TextureMode)

\brief      Set the texture mode.

\param      [in] TextureMode
            The AEGfxTextureMode to set.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void				AEGfxSetTextureMode(AEGfxTextureMode TextureMode);

/******************************************************************************/
/*!
\fn		    void AEGfxPoint(f32 x0, f32 y0, f32 z0, u32 c0)

\brief      Draw a point on screen. 

\details    The point is defined by x0, y0, z0 with color c0.

\warning    Do not call this function when AEGraphics is building mesh.

\warning    Function may be slow. Use with caution.

\param      [in] x0
            X coordinate of point

\param      [in] y0
            Y coordinate of point

\param      [in] z0
            Z coordinate of point

\param      [in] c0
            Color value of point

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void			    AEGfxPoint		(f32 x0, f32 y0, f32 z0, u32 c0);

/******************************************************************************/
/*!
\fn		    void AEGfxLine(f32 x0, f32 y0, f32 z0, f32 r0, f32 g0, f32 b0, f32 a0, 
                           f32 x1, f32 y1, f32 z1, f32 r1, f32 g1, f32 b1, f32 a1)

\brief      Draw a line on screen. 

\details    The line is defined by 2 points: pt0 and pt1.

\warning    Do not call this function when AEGraphics is building mesh.

\warning    Function may be slow. Use with caution.

\param      [in] x0
            X coordinate of pt0

\param      [in] y0
            Y coordinate of pt0

\param      [in] z0
            Z coordinate of pt0

\param      [in] r0
            Percentage of red of pt0. Range from 0.0f to 1.0f.

\param      [in] g0
            Percentage of green of pt0. Range from 0.0f to 1.0f.

\param      [in] b0
            Percentage of blue of pt0. Range from 0.0f to 1.0f.

\param      [in] a0
            Percentage of alpha (transparency) of pt0. 
            Range from 0.0f (clear) to 1.0f (opaque).

\param      [in] x1
            X coordinate of pt1

\param      [in] y1
            Y coordinate of pt1

\param      [in] z1
            Z coordinate of pt1

\param      [in] r1
            Percentage of red of pt1. Range from 0.0f to 1.0f.

\param      [in] g1
            Percentage of green of pt1. Range from 0.0f to 1.0f.

\param      [in] b1
            Percentage of blue of pt1. Range from 0.0f to 1.0f.

\param      [in] a1
            Percentage of alpha (transparency) of pt1. 
            Range from 0.0f (clear) to 1.0f (opaque).

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void			    AEGfxLine		(f32 x0, f32 y0, f32 z0, f32 r0, f32 g0, f32 b0, f32 a0, 
                                         f32 x1, f32 y1, f32 z1, f32 r1, f32 g1, f32 b1, f32 a1);

/******************************************************************************/
/*!
\fn		    void AEGfxTri(f32 x0, f32 y0, f32 z0, u32 c0, 
						  f32 x1, f32 y1, f32 z1, u32 c1, 
						  f32 x2, f32 y2, f32 z2, u32 c2)

\brief      Draw a triangle on screen. 

\details    The triangle is defined by 3 points: pt0, pt1 and pt2.
            The points are listed in a counter-clockwise order.

\warning    Do not call this function when AEGraphics is building mesh.

\warning    Function may be slow. Use with caution.

\param      [in] x0
            X coordinate of pt0

\param      [in] y0
            Y coordinate of pt0

\param      [in] z0
            Z coordinate of pt0

\param      [in] c0
            Color value of pt0

\param      [in] x1
            X coordinate of pt1

\param      [in] y1
            Y coordinate of pt1

\param      [in] z1
            Z coordinate of pt1

\param      [in] c1
            Color value of pt1

\param      [in] x2
            X coordinate of pt2

\param      [in] y2
            Y coordinate of pt2

\param      [in] z2
            Z coordinate of pt2

\param      [in] c2
            Color value of pt2

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void			    AEGfxTri		(f32 x0, f32 y0, f32 z0, u32 c0, 
								         f32 x1, f32 y1, f32 z1, u32 c1, 
								         f32 x2, f32 y2, f32 z2, u32 c2);

/******************************************************************************/
/*!
\fn		    void AEGfxQuad(f32 x0, f32 y0, f32 z0, u32 c0, 
						   f32 x1, f32 y1, f32 z1, u32 c1, 
						   f32 x2, f32 y2, f32 z2, u32 c2, 
						   f32 x3, f32 y3, f32 z3, u32 c3)

\brief      Draw a rectangle on screen. 

\details    The rectangle is defined by 4 points: pt0, pt1, pt2 and pt3.
            The points are listed in a counter-clockwise order.

\warning    Do not call this function when AEGraphics is building mesh.

\warning    Function may be slow. Use with caution.

\param      [in] x0
            X coordinate of pt0

\param      [in] y0
            Y coordinate of pt0

\param      [in] z0
            Z coordinate of pt0

\param      [in] c0
            Color value of pt0

\param      [in] x1
            X coordinate of pt1

\param      [in] y1
            Y coordinate of pt1

\param      [in] z1
            Z coordinate of pt1

\param      [in] c1
            Color value of pt1

\param      [in] x2
            X coordinate of pt2

\param      [in] y2
            Y coordinate of pt2

\param      [in] z2
            Z coordinate of pt2

\param      [in] c2
            Color value of pt2

\param      [in] x3
            X coordinate of pt3

\param      [in] y3
            Y coordinate of pt3

\param      [in] z3
            Z coordinate of pt3

\param      [in] c3
            Color value of pt3

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void			    AEGfxQuad		(f32 x0, f32 y0, f32 z0, u32 c0, 
								         f32 x1, f32 y1, f32 z1, u32 c1, 
								         f32 x2, f32 y2, f32 z2, u32 c2, 
								         f32 x3, f32 y3, f32 z3, u32 c3);

/******************************************************************************/
/*!
\fn		    void AEGfxBox(f32 x0, f32 y0, f32 z0, 
                          f32 sizeX, f32 sizeY, f32 sizeZ, 
                          u32 c0 ,u32 c1)
                          
\brief      Draw a 3D box on screen. 

\details    The box is centered around the point (x0, y0, z0) 
            and sizeX, sizeY and sizeZ are the width, height and 
            depth. c0 and c1 are the box color. Default values are
            white and grey.

\warning    Do not call this function when AEGraphics is building mesh.

\warning    Function may be slow. Use with caution.

\param      [in] x0
            X coordinate of the center of the box

\param      [in] y0
            Y coordinate of the center of the box

\param      [in] z0
            Z coordinate of the center of the box

\param      [in] sizeX
            Width of the box

\param      [in] sizeY
            Height of the box

\param      [in] sizeZ
            Depth of the box

\param      [in] c0
            First color of the box.

\param      [in] c1
            Second color of the box.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void			    AEGfxBox		(f32 x0, f32 y0, f32 z0, f32 sizeX, f32 sizeY, f32 sizeZ, u32 c0, u32 c1);

/******************************************************************************/
/*!
\fn		    void AEGfxSphere(f32 x0, f32 y0, f32 z0, 
                             f32 radius,
                             u32 c0, 
                             u32 c1, 
                             u32 division)
                          
\brief      Draw a 3D sphere on screen. 

\details    The sphere is centered around the point (x0, y0, z0)
            with size radius. c0 and c1 are the sphere color. 
            Default colors are white and grey. Division is the 
            resolution, or how many parts the sphere consists of. 
            Default value is 8.

\warning    Do not call this function when AEGraphics is building mesh.

\warning    Function may be slow. Use with caution.

\param      [in] x0
            X coordinate of the center of the sphere

\param      [in] y0
            Y coordinate of the center of the sphere

\param      [in] z0
            Z coordinate of the center of the sphere

\param      [in] radius
            Radius of the sphere

\param      [in] c0
            First color of the sphere.

\param      [in] c1
            Second color of the sphere.

\param      [in] division
            Number of parts to divide the sphere by. 

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void			    AEGfxSphere		(f32 x0, f32 y0, f32 z0, 
                                         f32 radius, u32 c0, u32 c1, 
                                         u32 division);

/******************************************************************************/
/*!
\fn		    void AEGfxAxis(f32 scale)
                          
\brief      Draw a 3D axis on screen. 

\details    The axis is rendered on the center of the screen
            with size scale.

\warning    Do not call this function when AEGraphics is building mesh.

\warning    Function may be slow. Use with caution.

\param      [in] scale
            Size of the axis

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void			    AEGfxAxis		(f32 scale);

/******************************************************************************/
/*!
\fn         u32	AEGfxColInterp(u32 c0, u32 c1, f32 t)

\brief      Get the interpolation between two colours (c0 and c1)
            at time interval t.

\warning    t should be between 0.0f to 1.0f.

\param      [in] c0
            The first colour.

\param      [in] c1
            The second colour.

\param      [in] t
            The time interval to calculate the interpolation at.
            At t = 0.0f, the result will be c0.
            At t = 1.0f, the result will be c1.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API u32				AEGfxColInterp	(u32 c0, u32 c1, f32 t);




// ---------------------------------------------------------------------------
//                            FONT SYSTEM
	

/******************************************************************************/
/*!
\fn         s8  AEGfxCreateFont(const char * fontName, int sizeF);

\brief      Load a Font in memory, and prepares it for usage in the AlphaEngine

\param      [in] fontName
            Full Path to the font ( accept .ttf, .ttc, .cff, .otf, .otc, .fnt ...)

\param      [in] sizeF
            Initial size of the glyphs in memory.

\retval     s8
            ID of the font, returns -1 if unsuccessful.
*/
/******************************************************************************/
AE_API s8  AEGfxCreateFont(const char * fontName, int sizeF);

/******************************************************************************/
/*!
\fn		    void AEGfxPrint(s8 fontId, s8* pStr,
							f32 screenX, f32 screenY,
							f32 scale,
							f32 red, f32 Green, f32 blue)

\brief      Prints a text on screen

\warning	Be sure to set AEGfxSetBlendMode(AE_GFX_BM_BLEND) mode before
			calling this function.
			
			Uses Screen percent as Coordinates to Handle multi-resolution. You
			still have to manage the proper scaling yourself, according to the
			initial size of the font you have chosen.
			
			Keep scale close to 1.0f to avoid glyph visual artifact.

\param      [in] fontId
            ID of a font created with AEGfxCreateFont.

\param      [in] pStr
            String to Display.

\param      [in] screenX
            X position on screen, in Percentage. Range from -1.0f to 1.0f.

\param      [in] screenY
            Y position on screen, in Percentage. Range from -1.0f to 1.0f.

\param      [in] scale
            Applied scale to the original Font size created with AEGfxCreateFont

\param      [in] red
            Percentage of red. Range from 0.0f to 1.0f.

\param      [in] green
            Percentage of green. Range from 0.0f to 1.0f.

\param      [in] blue
            Percentage of blue. Range from 0.0f to 1.0f.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxPrint(s8 fontId, s8* pStr, f32 screenX, f32 screenY, f32 scale, f32 red, f32 green, f32 blue);


/******************************************************************************/
/*!
\fn		    void AEGfxGetPrintSize(s8 fontId, s8* pStr, f32 scale, f32& width, f32& height)

\brief      Gets the size of a Text in the specified font. 

\param      [in] fontId
            ID of a font created with AEGfxCreateFont.

\param      [in] pStr
            String to Measure.

\param      [in] scale
            Applied scale to the original Font size created with AEGfxCreateFont

\param      [out] width
            width of the text. In Screen Percent ( 0 = empty, to 2 = full screen).

\param      [out] height
            height of the text. In screen Percent ( 0 = empty, to 2 = full screen).

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxGetPrintSize(s8 fontId, s8* pStr, f32 scale, f32& width, f32& height);

	
/******************************************************************************/
/*!
\fn         void AEGfxDestroyFont(s8 fontId);

\brief      Deletes a Font from memory

\param      [in] fontId
            ID returned from the AEGfxCreateFont(...) function

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxDestroyFont(s8 fontId);

/******************************************************************************/
/*!
\fn         void  AEGfxFontSystemStart( );

\brief      Initializes the Font system.

\warning   	This function is already called in AESysInit. Do not call it

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxFontSystemStart();

/******************************************************************************/
/*!
\fn         void  AEGfxFontSystemEnd( );

\brief      Ends the Font system. And clear all Fonts from memory

\warning    This function is already called in AESysExit. Do not call it

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGfxFontSystemEnd();

// ---------------------------------------------------------------------------


#ifdef __cplusplus 
}
#endif

// ---------------------------------------------------------------------------

#endif // AE_GRAPHICS_H

