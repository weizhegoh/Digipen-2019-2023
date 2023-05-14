/*****************************************************************
*\file         FamilyTransformSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "FamilyTransformSystem.h"
namespace Xivi::Graphics
{
void FamilyTransformSystem::operator()(
  Entity &entity,
  Position &pos,
  Transform &transform,
  Child &child ) noexcept
{
  Math::Matrix4 model( 1.f );
  Math::Matrix4 scale( 1.f );
  Math::Matrix4 rotate( 1.f );
  Math::Vector3 offset = ( pos.shaky ) ? Math::Helper::RandFunction::RandNumberBetween( Math::Vector3( 1.f, 1.f, 1.f ), Math::Vector3( -1.f, -1.f, -1.f ) ) : Math::Vector3( 0.f, 0.f, 0.f );
  Math::Vector3 posTemp = pos.m_position + offset;
  model = model.translate( posTemp.getX(), posTemp.getY(), posTemp.getZ() );
  Vec3f axisRad = Vec3f(
    Math::DegToRad( transform.axis.getX() ),
    Math::DegToRad( transform.axis.getY() ),
    Math::DegToRad( transform.axis.getZ() ) );
  rotate = Math::Helper::MathFunction::ToRotationMatrix( axisRad );
  scale = scale.scale( transform.scale.getX(), transform.scale.getY(), transform.scale.getZ() );
  transform.LocalToWorld = ( model * rotate * scale );
  pos.m_localPosition = Vec3f( 0.f, 0.f, 0.f );
  if ( HaveComponent<Child>( entity ) )
    Traverse( child );
}

void FamilyTransformSystem::Traverse( Child &child )
{
  for ( const auto &x : child.m_entities )
  {
    //if (x.Zombie())
    //	continue;
    auto &transform = GetComponent<Transform>( x );
    auto &pos = GetComponent<Position>( x );

    Math::Matrix4 model( 1.f );
    Math::Matrix4 scale( 1.f );
    Math::Matrix4 rotate( 1.f );
    Math::Vector3 offset = ( pos.shaky ) ? Math::Helper::RandFunction::RandNumberBetween( Math::Vector3( 1.f, 1.f, 1.f ), Math::Vector3( -1.f, -1.f, -1.f ) ) : Math::Vector3( 0.f, 0.f, 0.f );
    Math::Vector3 posTemp = pos.m_position + offset;
    model = model.translate( posTemp.getX(), posTemp.getY(), posTemp.getZ() );
    Vec3f axisRad = Vec3f(
      Math::DegToRad( transform.axis.getX() ),
      Math::DegToRad( transform.axis.getY() ),
      Math::DegToRad( transform.axis.getZ() ) );
    rotate = Math::Helper::MathFunction::ToRotationMatrix( axisRad );
    scale = scale.scale( transform.scale.getX(), transform.scale.getY(), transform.scale.getZ() );
    transform.LocalToWorld = ( model * rotate * scale );
    if ( HaveComponent<Child>( x ) )
      Traverse( GetComponent<Child>( x ) );
    //pos.m_localPosition = Vec3f(0.f, 0.f, 0.f);
  }
  //for (const auto& x : child.m_entities)
  //{
  //	auto& transform = GetComponent<Transform>(x);
  //	auto& pos       = GetComponent<Position>(x);
  //	auto& parent    = GetComponent<Parent>(x);
  //	auto& parent_transfom = GetComponent<Transform>(parent.m_entity);
  //	auto& parent_position= GetComponent<Position>(parent.m_entity);
  //
  //	auto& localToParent_transform = GetComponent<LocalToParent>(x);

  //	//localToParent_transfom.m_position =  localToParent_transfom.m_position - parent_position.m_position;
  //	//localToParent_transform.m_position = parent_position.m_localPosition - pos.m_position;

  //	Math::Matrix4 model(1.f);
  //	Math::Matrix4 scale(1.f);
  //	Math::Matrix4 rotate(1.f);
  //	model = model.translate(
  //		localToParent_transform.m_position .getX(),
  //		localToParent_transform.m_position .getY(),
  //		localToParent_transform.m_position .getZ());
  //	Vec3f axisRad = Vec3f(
  //		Math::DegToRad(transform.axis.getX()),
  //		Math::DegToRad(transform.axis.getY()),
  //		Math::DegToRad(transform.axis.getZ()));
  //	rotate = Math::Helper::MathFunction::ToRotationMatrix(axisRad);
  //	scale = scale.scale(
  //		localToParent_transform.scale.getX(),
  //		localToParent_transform.scale.getY(),
  //		localToParent_transform.scale.getZ());

  //	localToParent_transform.localToParent = (model * rotate * scale);
  //	transform.LocalToWorld =  parent_transfom.LocalToWorld * localToParent_transform.localToParent;
  //	//pos.m_position.set(transform.model[0][3], transform.model[1][3], transform.model[2][3]);

  //	if(HaveComponent<Child>(x))
  //		Traverse(GetComponent<Child>(x));
  //}
}
}