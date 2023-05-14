#ifndef PIPELINE_TOOLS_H
#define PIPELINE_TOOLS_H


/*
struct Vertex
{
	Point4 position;
	Vector4 normal;
	Point4 color;
	Point4 texCoord;

	f32 zInCamera;
	f32 zDepth;

	Vertex(Point4 p = Point4(), Vector4 n = Vector4(), Point4 c = Point4(), Point4 t = Point4(), f32 zc = 0.0f, f32 zd = 0.0f)
	{
		position = p;
		normal = n;
		color = c;
		texCoord = t;

		zInCamera = zc;
		zDepth = zd;
	}
};
*/


Matrix4 MtxScale(const f32 &sx, const f32 &sy, const f32 &sz);
Matrix4 MtxScale(const Point4 &s);
Matrix4 MtxScale(const Vector4 &s);

Matrix4 MtxTranslate(const f32 &tx, const f32 &ty, const f32 &tz);
Matrix4 MtxTranslate(const Point4 &t);
Matrix4 MtxTranslate(const Vector4 &t);

Matrix4 MtxRotateX(const f32 &alpha);
Matrix4 MtxRotateY(const f32 &alpha);
Matrix4 MtxRotateZ(const f32 &alpha);

Matrix4 MtxRotateAxisAngle(Vector4 &v, const f32 &alpha);
Matrix4 MtxRotateOrthogonal(Vector4 &u, Vector4 &v, Vector4 &w);

Matrix4 MtxTranspose(const Matrix4 &m);

f32 MtxDeterminant(const Matrix4 &m);



Matrix4 MtxLookAt(const Point4 &position, const Point4 &target, const Vector4 &up);

Matrix4 MtxOrthogonalProjection(const f32 &width, const f32 &height, const f32 &near, const f32 &far);
Matrix4 MtxPerspectiveProjection(const f32 &fovy, const f32 &aspectRatio, const f32 &near, const f32 &far);

Matrix4 MtxNormalMatrix(const Matrix4 &input);


#endif