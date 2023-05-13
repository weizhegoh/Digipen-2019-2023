///////////////////////////////////////////////////////////////////////////////
///
/// \file MathFunctions.hpp
/// Central location for all the math used by the Zero engine.
/// 
/// Authors: Benjamin Strukus
/// Copyright 2010, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Reals.hpp"
#include "SIMD.h"
#include "Matrix.h"
#include "Quaternion.hpp"
#include "EulerAngles.hpp"

namespace Xivi::Math
{
    using Vector2 = Vec2f;
    using Vector3 = Vec3f;
    using Vec3Ptr = Vector3*;
    using Vector4 = Vec4f;
    using Vec4Ptr = Vector4*;
    using Matrix3 = Mat3x3;
    using Mat3Ptr = Matrix3*;
    using Matrix4 = Mat4x4;
    using Mat4Ptr = Matrix4*;
    ///Creates a skew symmetric matrix from the given 3D vector. Multiplying a 
    ///vector by this matrix is equivalent to the cross product using the input 
    ///vector.
    Matrix3 SkewSymmetric(Vec3Param vec3);

    ///Projects the normal out of the vector. Dot(normal, result) will be 0.
    Vector3 ProjectOut(Vec3Param vector, Vec3Param normal);

    ///Converts a quaternion to an axis-angle pair (in radians). Axis is stored in 
    ///the Vector4's xyz and the angle is stored in the w.
    Vector4 ToAxisAngle(QuatParam quaternion);
    void ToAxisAngle(QuatParam quaternion, Vec4Ptr axisAngle);

    ///Converts a quaternion to an axis-angle pair (in radians).
    void ToAxisAngle(QuatParam quaternion, Vec3Ptr axis, float* radians);

    ///Convert a 3x3 matrix to a set of Euler angles (in radians). The desired order
    ///of the rotations is expected to be in the given Euler angle structure.
    EulerAngles ToEulerAngles(Mat3Param matrix,
        EulerOrders::Enum order = EulerOrders::XYZs);
    void ToEulerAngles(Mat3Param matrix, EulerAnglesPtr eulerAngles);

    ///Convert a 4x4 matrix to a set of Euler angles in radians. The desired order
    ///of the rotations is expected to be in the given Euler angle structure.
    EulerAngles ToEulerAngles(Mat4Param matrix,
        EulerOrders::Enum order = EulerOrders::XYZs);
    void ToEulerAngles(Mat4Param matrix, EulerAnglesPtr eulerAngles);

    ///Convert a quaternion to a set of Euler angles (in radians). The desired order
    ///of the rotations is expected to be in the given Euler angle structure.
    EulerAngles ToEulerAngles(QuatParam quaternion,
        EulerOrders::Enum order = EulerOrders::XYZs);
    void ToEulerAngles(QuatParam quaternion, EulerAnglesPtr eulerAngles);

    /// Converts from Vector3 to Vector2, removing the z component of the Vector3.
    Vector2 ToVector2(Vec3Param v3);

    /// Converts from Vector2 to Vector3, adding the given z component.
    Vector3 ToVector3(Vec2Param v2, float z = 0.0f);

    ///Converts an axis-angle pair to a 3x3 (in radians). Axis is stored in the
    ///Vector4's xyz and the angle is stored in the w. Axis is assumed to be 
    ///normalized.
    Matrix3 ToMatrix3(Vec4Param axisAngle);
    void ToMatrix3(Vec4Param axisAngle, Mat3Ptr matrix);

    ///Converts an axis-angle pair to a 3x3 matrix (in radians). Axis is assumed to
    ///be normalized.
    Matrix3 ToMatrix3(Vec3Param axis, float radians);
    void ToMatrix3(Vec3Param axis, float radians, Mat3Ptr matrix);

    ///Convert a set of Euler angles to a 3x3 matrix (in radians).
    Matrix3 ToMatrix3(EulerAnglesParam eulerAngles);
    void ToMatrix3(EulerAnglesParam eulerAngles, Mat3Ptr matrix);

    ///Convert a 4x4 matrix to a 3x3 matrix. Simply copies the 4x4 matrix's upper 
    ///3x3 matrix (rotation & scale) to the 3x3 matrix.
    Matrix3 ToMatrix3(Mat4Param matrix4);
    void ToMatrix3(Mat4Param matrix4, Mat3Ptr matrix3);

    ///Converts a quaternion to a 3x3 rotation matrix (in radians).
    Matrix3 ToMatrix3(QuatParam quaternion);
    void ToMatrix3(QuatParam quaternion, Mat3Ptr matrix);

    ///Convert a set of Euler angles to a 4x4 matrix (in radians).
    Matrix4 ToMatrix4(EulerAnglesParam eulerAngles);
    void ToMatrix4(EulerAnglesParam eulerAngles, Mat4Ptr matrix);

    ///Convert a 3x3 matrix to a 4x4 matrix. Simply copies the 3x3 matrix's values
    ///into the rotational part of the 4x4 matrix.
    Matrix4 ToMatrix4(Mat3Param matrix3);
    void ToMatrix4(Mat3Param matrix3, Mat4Ptr matrix4);

    ///Converts a quaternion to a 4x4 rotation matrix (in radians).
    Matrix4 ToMatrix4(QuatParam quaternion);
    void ToMatrix4(QuatParam quaternion, Mat4Ptr matrix);

    ///Converts a 3D vector to a quaternion.
    Quaternion ToQuaternion(Vec3Param vector);
    void ToQuaternion(Vec3Param vector, QuatPtr quaternion);

    ///Converts an axis-angle pair to a quaternion (in radians). Axis is stored in
    ///the Vector4's xyz and the angle is stored in the w. Axis is assumed to be 
    ///normalized.
    Quaternion ToQuaternion(Vec4Param axisAngle);
    void ToQuaternion(Vec4Param axisAngle, QuatPtr quaternion);

    ///Converts an axis-angle pair to a quaternion (in radians). Axis is assumed to
    ///be normalized.
    Quaternion ToQuaternion(Vec3Param axis, float radians);
    void ToQuaternion(Vec3Param axis, float radians, QuatPtr quaternion);

    ///Convert a set of Euler angles to a quaternion (in radians).
    Quaternion ToQuaternion(EulerAnglesParam eulerAngles);
    void ToQuaternion(EulerAnglesParam eulerAngles, QuatPtr quaternion);

    ///Converts a 3x3 matrix to a quaternion (in radians).
    Quaternion ToQuaternion(Mat3Param matrix);
    void ToQuaternion(Mat3Param matrix, QuatPtr quaternion);

    ///Converts a 4x4 matrix to a quaternion (in radians).
    Quaternion ToQuaternion(Mat4Param matrix);
    void ToQuaternion(Mat4Param matrix, QuatPtr quaternion);

    ///Generates a set of orthonormal vectors from the given vectors, modifying u 
    ///and v.
    void GenerateOrthonormalBasis(Vec3Param w, Vec3Ptr u, Vec3Ptr v);

    ///Doesn't blow up on zero vectors
    void DebugGenerateOrthonormalBasis(Vec3Param w, Vec3Ptr u, Vec3Ptr v);

    ///Perform the dot product with the specified basis vector of the 3x3 matrix and
    ///the given vector.
    float BasisNDot(Mat3Param matrix, unsigned basisIndex, Vec3Param vector);

    ///Perform the dot product with the specified basis vector of the 3x3 matrix and
    ///the given vector.
    float BasisNDot(Mat4Param matrix, unsigned basisIndex, Vec4Param vector);

    ///Perform the dot product with the basis vector of the 3x3 matrix that 
    ///represents the x-axis (if the matrix is pure rotation) and the given vector.
    float BasisXDot(Mat3Param matrix, Vec3Param vector);

    ///Perform the dot product with the basis vector of the 3x3 matrix that 
    ///represents the y-axis (if the matrix is pure rotation) and the given vector.
    float BasisYDot(Mat3Param matrix, Vec3Param vector);

    ///Perform the dot product with the basis vector of the 3x3 matrix that 
    ///represents the z-axis (if the matrix is pure rotation) and the given vector.
    float BasisZDot(Mat3Param matrix, Vec3Param vector);

    ///Perform the dot product with the basis vector of the 4x4 matrix that 
    ///represents the x-axis (if the matrix is pure rotation) and the given vector.
    float BasisXDot(Mat4Param matrix, Vec4Param vector);

    ///Perform the dot product with the basis vector of the 4x4 matrix that 
    ///represents the y-axis (if the matrix is pure rotation) and the given vector.
    float BasisYDot(Mat4Param matrix, Vec4Param vector);

    ///Perform the dot product with the basis vector of the 4x4 matrix that 
    ///represents the z-axis (if the matrix is pure rotation) and the given vector.
    float BasisZDot(Mat4Param matrix, Vec4Param vector);

    //----------------------------------------------------------- Rotation Functions
    /// Get angle between two 2D vectors in radians.
    float Angle(Vec2Param a, Vec2Param b);

    /// Get angle between two 3D vectors in radians.
    float Angle(Vec3Param a, Vec3Param b);

    /// Get angle between two quaternions in radians.
    float Angle(QuatParam a, QuatParam b);

    /// Spherical linear interpolation for unit 2D vectors.
    /// Interpolates along the surface of the unit sphere.
    Vector2 Slerp(Vec2Param start, Vec2Param end, float t);

    /// Spherical linear interpolation for unit 3D vectors.
    /// Interpolates along the surface of the unit sphere.
    Vector3 Slerp(Vec3Param start, Vec3Param end, float t);

    /// Rotate a 2D vector towards another 2D vector changing at most maxAngle,
    /// maxAngle is in radians.
    Vector2 RotateTowards(Vec2Param a, Vec2Param b, float maxAngle);

    /// Rotate a 3D vector towards another 3D vector changing at most maxAngle,
    /// maxAngle is in radians.
    Vector3 RotateTowards(Vec3Param a, Vec3Param b, float maxAngle);

    /// Rotate a quaternion towards another quaternion changing at most maxAngle,
    /// maxAngle is in radians.
    Quat RotateTowards(QuatParam a, QuatParam b, float maxAngle);

    // Get the rotation angle between two vectors in radians.
    float SignedAngle(Vec3Param a, Vec3Param b, Vec3Param up);

}// namespace Math
