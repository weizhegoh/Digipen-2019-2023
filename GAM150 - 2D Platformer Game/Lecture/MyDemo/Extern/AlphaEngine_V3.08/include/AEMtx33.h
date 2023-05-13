/******************************************************************************/
/*!
\par        Project: Alpha Engine
\file       AEMtx33.h

\author     Sun Tjen Fam
\date       January 31, 2008

\brief      Header file for the 3x3 matrix library.

\copyright  Copyright (C) 2013 DigiPen Institute of Technology. Reproduction 
            or disclosure of this file or its contents without the prior 
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#ifndef AE_MTX33_H
#define AE_MTX33_H

// ---------------------------------------------------------------------------
// Defines

#define AEMtx33RowCol(pMtx, row, col) (pMtx)->m[(row)][(col)]

// ---------------------------------------------------------------------------
// Struct/Class definitions

/*! 
    Matrix is stored in column major format,
    ie. the translation term is in the right most column.

    m[0][0] m[0][1] m[0][2] \n
    m[1][0] m[1][1] m[1][2] \n     
    m[2][0] m[2][1] m[2][2]
*/
typedef struct AEMtx33
{
	f32	m[3][3];
}AEMtx33;

// ---------------------------------------------------------------------------

#ifdef __cplusplus 
extern "C"
{
#endif

// ---------------------------------------------------------------------------

/******************************************************************************/
/*!
\fn         void AEMtx33Identity (AEMtx33* pResult)

\brief      Set pResult to identity matrix.

\param      [out] pResult
            Pointer to AEMtx33 to be set.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void 		AEMtx33Identity			(AEMtx33* pResult);

/******************************************************************************/
/*!
\fn         void AEMtx33Transpose(AEMtx33* pResult, 
                                  AEMtx33* pMtx)

\brief      Set pResult to the transpose of pMtx.

\param      [out] pResult
            Pointer to AEMtx33 to be set.

\param      [in] pMtx
            Pointer to AEMtx33 for input.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void 		AEMtx33Transpose		(AEMtx33* pResult, AEMtx33* pMtx);

/******************************************************************************/
/*!
\fn         f32 AEMtx33Inverse(AEMtx33* pResult, 
                               AEMtx33* pMtx)

\brief      Set pResult to the inverse of pMtx.

\param      [out] pResult
            Pointer to AEMtx33 to be set.

\param      [in] pMtx
            Pointer to AEMtx33 for input.

\retval     f32
            Returns the determinant of pMtx.
*/
/******************************************************************************/
AE_API f32			AEMtx33Inverse			(AEMtx33* pResult, AEMtx33* pMtx);

/******************************************************************************/
/*!
\fn         void AEMtx33InvTranspose(AEMtx33* pResult, 
                                     AEMtx33* pMtx)

\brief      Set pResult to the transpose of the inverse of pMtx.

\param      [out] pResult
            Pointer to AEMtx33 to be set.

\param      [in] pMtx
            Pointer to AEMtx33 for input.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void 		AEMtx33InvTranspose		(AEMtx33* pResult, AEMtx33* pMtx);

/******************************************************************************/
/*!
\fn         void AEMtx33Concat(AEMtx33* pResult, 
                               AEMtx33* pMtx0, 
                               AEMtx33* pMtx1)

\brief      Set pResult to the multiplication of pMtx0 with pMtx1.

\param      [out] pResult
            Pointer to AEMtx33 to be set.

\param      [in] pMtx0
            Pointer to AEMtx33 for input.

\param      [in] pMtx1
            Pointer to AEMtx33 for input.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void 		AEMtx33Concat			(AEMtx33* pResult, AEMtx33* pMtx0, AEMtx33* pMtx1);

/******************************************************************************/
/*!
\fn         void AEMtx33Orthogonalize(AEMtx33* pResult, 
                                      AEMtx33* pMtx)

\brief      Set pResult to the orthogonalization of pMtx.

\warning    Function not implemented.

\param      [out] pResult
            Pointer to AEMtx33 to be set.

\param      [in] pMtx
            Pointer to AEMtx33 for input.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void 		AEMtx33Orthogonalize	(AEMtx33* pResult, AEMtx33* pMtx);

/******************************************************************************/
/*!
\fn         f32 AEMtx33Determinant(AEMtx33* pMtx)

\brief      Calculate the determinant of pMtx.

\param      [in] pMtx
            Pointer to AEMtx33 for input.

\retval     f32
            Returns the determinant of pMtx.
*/
/******************************************************************************/
AE_API f32			AEMtx33Determinant		(AEMtx33* pMtx);

// ---------------------------------------------------------------------------

/******************************************************************************/
/*!
\fn         void AEMtx33SetCol(AEMtx33* pResult, 
                               u32 col,  
                               AEVec2* pVec)

\brief      Set the first and second element of the selected column of pResult
            to the x and y values of pVec respectively. The last element of the
            column will be set automatically.

\param      [in,out] pResult
            Pointer to AEMtx33 to be set.

\param      [in] col
            The selected column of pResult.

\param      [in] pVec
            Pointer to AEVec2 for input.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void 		AEMtx33SetCol			(AEMtx33* pResult, u32 col,  AEVec2* pVec);

/******************************************************************************/
/*!
\fn         void AEMtx33SetRow(AEMtx33* pResult, 
                               u32 row,  
                               AEVec2* pVec)

\brief      Set the first and second element of the selected row of pResult
            to the x and y values of pVec respectively. The last element of the
            row will be set automatically.

\param      [in,out] pResult
            Pointer to AEMtx33 to be set.

\param      [in] row
            The selected row of pResult.

\param      [in] pVec
            Pointer to AEVec2 for input.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void 		AEMtx33SetRow			(AEMtx33* pResult, u32 row,  AEVec2* pVec);

/******************************************************************************/
/*!
\fn         void AEMtx33GetCol(AEVec2* pResult,
                               AEMtx33* pMtx,
                               u32 col)

\brief      Set the x and y values of pResult with the first and second element of 
            the selected column of pMtx respectively.

\param      [out] pResult
            Pointer to AEVec2 to be set.

\param      [in] pMtx
            Pointer to AEMtx33 for input.

\param      [in] col
            The selected column of pResult.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void 		AEMtx33GetCol			(AEVec2* pResult, AEMtx33* pMtx, u32 col);

/******************************************************************************/
/*!
\fn         void AEMtx33GetRow(AEVec2* pResult,
                               AEMtx33* pMtx
                               u32 row)

\brief      Set the x and y values of pResult with the first and second element of 
            the selected row of pMtx respectively.

\param      [out] pResult
            Pointer to AEVec2 to be set.

\param      [in] pMtx
            Pointer to AEMtx33 for input.

\param      [in] row
            The selected row of pResult.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void			AEMtx33GetRow			(AEVec2* pResult, AEMtx33* pMtx, u32 row);

// ---------------------------------------------------------------------------

/******************************************************************************/
/*!
\fn         void AEMtx33Trans(AEMtx33* pResult, 
                              f32 x, 
                              f32 y)

\brief      Set pResult to the translation matrix of x and y.

\param      [out] pResult
            Pointer to AEMtx33 to be set.

\param      [in] x
            Translation along the x-axis.

\param      [in] y
            Translation along the y-axis.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void 		AEMtx33Trans			(AEMtx33* pResult, f32 x, f32 y);

/******************************************************************************/
/*!
\fn         void AEMtx33TransApply(AEMtx33* pResult, 
                                   AEMtx33* pMtx, 
                                   f32 x, 
                                   f32 y)

\brief      Set pResult to the multiplication of translation matrix of x and y
            with pMtx.

\param      [out] pResult
            Pointer to AEMtx33 to be set.

\param      [in] pMtx
            Pointer to AEMtx33 for input.

\param      [in] x
            Translation along the x-axis.

\param      [in] y
            Translation along the y-axis.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void 		AEMtx33TransApply		(AEMtx33* pResult, AEMtx33* pMtx, f32 x, f32 y);

/******************************************************************************/
/*!
\fn         void AEMtx33Scale(AEMtx33* pResult, 
                              f32 x, 
                              f32 y)

\brief      Set pResult to the scaling matrix of x and y.

\param      [out] pResult
            Pointer to AEMtx33 to be set.

\param      [in] x
            Scaling along the x-axis.

\param      [in] y
            Scaling along the y-axis.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void 		AEMtx33Scale			(AEMtx33* pResult, f32 x, f32 y);

/******************************************************************************/
/*!
\fn         void AEMtx33ScaleApply(AEMtx33* pResult, 
                                   AEMtx33* pMtx, 
                                   f32 x, 
                                   f32 y)

\brief      Set pResult to the multiplication of scaling matrix of x and y
            with pMtx.

\param      [out] pResult
            Pointer to AEMtx33 to be set.

\param      [in] pMtx
            Pointer to AEMtx33 for input.

\param      [in] x
            Scaling along the x-axis.

\param      [in] y
            Scaling along the y-axis.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void 		AEMtx33ScaleApply		(AEMtx33* pResult, AEMtx33* pMtx, f32 x, f32 y);

/******************************************************************************/
/*!
\fn         void AEMtx33Rot(AEMtx33* pResult, 
                            f32 angle)

\brief      Set pResult to the rotation matrix of angle in radians
            rotating counter-clockwise.

\param      [out] pResult
            Pointer to AEMtx33 to be set.

\param      [in] angle
            Angle in radians rotating counter-clockwise.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void 		AEMtx33Rot				(AEMtx33* pResult, f32 angle);

/******************************************************************************/
/*!
\fn         void AEMtx33RotDeg(AEMtx33* pResult, 
                               f32 angle)

\brief      Set pResult to the rotation matrix of angle in degrees
            rotating counter-clockwise.

\param      [out] pResult
            Pointer to AEMtx33 to be set.

\param      [in] angle
            Angle in degress rotating counter-clockwise.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void 		AEMtx33RotDeg			(AEMtx33* pResult, f32 angle);

// ---------------------------------------------------------------------------

/******************************************************************************/
/*!
\fn         void AEMtx33MultVec(AEVec2* pResult, 
                                AEMtx33* pMtx, 
                                AEVec2* pVec)

\brief      Set pResult to the multiplication of pMtx with pVec.

\param      [out] pResult
            Pointer to AEMtx33 to be set.

\param      [in] pMtx
            Pointer to AEMtx33 for input.

\param      [in] pVec
            Pointer to AEVec2 for input.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void 		AEMtx33MultVec			(AEVec2* pResult, AEMtx33* pMtx, AEVec2* pVec);

/******************************************************************************/
/*!
\fn         void AEMtx33MultVecArray(AEVec2* pResult, 
                                     AEMtx33* pMtx, 
                                     AEVec2* pVec, 
                                     u32 count)

\brief      Set an array of vectors (pResult) to the multiplication 
            of pMtx with an array of vectors (pVec) of size (count).

\warning    Size of pResult should be not less than count.

\param      [out] pResult
            Pointer to an array of AEMtx33 to be set.

\param      [in] pMtx
            Pointer to AEMtx33 for input.

\param      [in] pVec
            Pointer to an array of AEVec2 for input.

\param      [in] count
            Number of elements in pVec to be multiplied.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void 		AEMtx33MultVecArray		(AEVec2* pResult, AEMtx33* pMtx, AEVec2* pVec, u32 count);


AE_API void 		AEMtx33MultVecSR		(AEVec2* pResult, AEMtx33* pMtx, AEVec2* pVec);
AE_API void 		AEMtx33MultVecArraySR	(AEVec2* pResult, AEMtx33* pMtx, AEVec2* pVec, u32 count);
	
// ---------------------------------------------------------------------------




#ifdef __cplusplus 
}
#endif

#endif // MTX_H