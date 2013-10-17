/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_Uniform.h"

#include <E_Geometry/E_Vec2.h>
#include <E_Geometry/E_Vec3.h>
#include <E_Geometry/E_Vec4.h>
#include <E_Geometry/E_Matrix3x3.h>
#include <E_Geometry/E_Matrix4x4.h>
#include <E_Util/Graphics/E_Color.h>
#include <Util/IO/FileName.h>
#include <Util/Macros.h>
#include "../E_RenderingContext.h"
#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

using namespace Rendering;
using namespace EScript;

namespace E_Rendering {

//! (static)
EScript::Type * E_Uniform::getTypeObject() {
	// E_Uniform ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static,internal) internal helper
template<typename value_t,int bucketSize>
void getUniformNumbersAsArray(Array * targetArray,const Uniform & u){
	const value_t *v = reinterpret_cast<const value_t*>(u.getData());
	const size_t numSingleValues = u.getDataSize() / sizeof(value_t);
	ERef<Array> arr;
	for(size_t i=0;i<numSingleValues;++i){
		if( (i%bucketSize) == 0){
			arr=Array::create();
			targetArray->pushBack(arr.get());
		}
		arr->pushBack(Number::create(v[i]));
	}
}

//! (static,internal) used by getData(...)
static Array * getUniformData(const Uniform & u){
	Array * a=Array::create();
	switch (u.getType()) {
		case Uniform::UNIFORM_BOOL: {
			const int *v = reinterpret_cast<const int*>(u.getData());
			for (size_t  i=0;i<u.getNumValues();i++)
				a->pushBack(Bool::create(v[i]!=0));
			break;
		}
		case Uniform::UNIFORM_VEC2B: {
			const int32_t *v = reinterpret_cast<const int32_t*>(u.getData());
			const size_t numSingleValues = u.getDataSize() / sizeof(int32_t);
			ERef<Array> a2;
			for(size_t i=0;i<numSingleValues;++i){
				if( (i%2) == 0){
					a2=Array::create();
					a->pushBack(a2.get());
				}
				a2->pushBack(Bool::create(v[i]!=0));
			}
			break;
		}
		case Uniform::UNIFORM_VEC3B: {
			const int32_t *v = reinterpret_cast<const int32_t*>(u.getData());
			const size_t numSingleValues = u.getDataSize() / sizeof(int32_t);
			ERef<Array> a2;
			for(size_t i=0;i<numSingleValues;++i){
				if( (i%3) == 0){
					a2=Array::create();
					a->pushBack(a2.get());
				}
				a2->pushBack(Bool::create(v[i]!=0));
			}
			break;
		}
		case Uniform::UNIFORM_VEC4B: {
			const int32_t *v = reinterpret_cast<const int32_t*>(u.getData());
			const size_t numSingleValues = u.getDataSize() / sizeof(int32_t);
			ERef<Array> a2;
			for(size_t i=0;i<numSingleValues;++i){
				if( (i%4) == 0){
					a2=Array::create();
					a->pushBack(a2.get());
				}
				a2->pushBack(Bool::create(v[i]!=0));
			}
			break;
		}
		case Uniform::UNIFORM_FLOAT: {
			const float *v = reinterpret_cast<const float*>(u.getData());
			for (size_t i=0;i<u.getNumValues();++i)
				a->pushBack(Number::create(v[i]));
			break;
		}
		case Uniform::UNIFORM_VEC2F: {
			getUniformNumbersAsArray<float,2>(a,u);
			break;
		}
		case Uniform::UNIFORM_VEC3F: {
			getUniformNumbersAsArray<float,3>(a,u);
			break;
		}
		case Uniform::UNIFORM_VEC4F: {
			getUniformNumbersAsArray<float,4>(a,u);
			break;
		}
		case Uniform::UNIFORM_INT: {
			const int *v = reinterpret_cast<const int*>(u.getData());
			for (size_t i=0;i<u.getNumValues();i++)
				a->pushBack(Number::create(v[i]));
			break;
		}
		case Uniform::UNIFORM_VEC2I: {
			getUniformNumbersAsArray<int32_t,2>(a,u);
			break;
		}
		case Uniform::UNIFORM_VEC3I: {
			getUniformNumbersAsArray<int32_t,3>(a,u);
			break;
		}
		case Uniform::UNIFORM_VEC4I: {
			getUniformNumbersAsArray<int32_t,4>(a,u);
			break;
		}
		case Uniform::UNIFORM_MATRIX_2X2F: {
			getUniformNumbersAsArray<float,4>(a,u);
			break;
		}
		case Uniform::UNIFORM_MATRIX_3X3F: {
			getUniformNumbersAsArray<float,9>(a,u);
			break;
		}
		case Uniform::UNIFORM_MATRIX_4X4F: {
			getUniformNumbersAsArray<float,16>(a,u);
			break;
		}
		default:
			WARN("unexpected case in switch statement");
	}
	return a;
}


// ----------


//! initMembers
void E_Uniform::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);


	/*!	[ESMF] new Rendering.Uniform( Uniform | String name, Number type, Array values)
		Depending on type, the type of values in the Array may differ:

		Uniform::UNIFORM_BOOL:	Bool
		Uniform::UNIFORM_VEC2B:	[x,y] (bool)
		Uniform::UNIFORM_VEC3B:	[x,y,z] (bool)
		Uniform::UNIFORM_VEC4B:	[x,y,z,w] (bool)

		Uniform::UNIFORM_FLOAT:	Number (float)
		Uniform::UNIFORM_VEC2F:	[x,y], Vec2 (float)
		Uniform::UNIFORM_VEC3F:	[x,y,z], Vec3 (float)
		Uniform::UNIFORM_VEC4F:	[x,y,z,w], Vec4 (float), Color4f

		Uniform::UNIFORM_INT:	Number (int)
		Uniform::UNIFORM_VEC2I:	[x,y], Vec2 (int)
		Uniform::UNIFORM_VEC3I:	[x,y,z], Vec3 (int)
		Uniform::UNIFORM_VEC4I:	[x,y,z,w], Vec4 (int)

		Uniform::UNIFORM_MATRIX_2X2F:	[x*4] (float)
		Uniform::UNIFORM_MATRIX_3X3F:	[x*9] (float), Matrix3x3
		Uniform::UNIFORM_MATRIX_4X4F:	[x*16] (float), Matrix4x4
	*/
	ES_CONSTRUCTOR(typeObject, 1, 3, {
		if(parameter.count()==1){
			return EScript::create(Uniform(parameter[0].to<const Rendering::Uniform&>(rt)));
		}
		EScript::assertParamCount(rt,3,3);
		const std::string name = parameter[0].toString();
		const Uniform::dataType_t type = static_cast<const Uniform::dataType_t>(parameter[1].to<uint32_t>(rt));
		Array * values = parameter[2].to<EScript::Array*>(rt);

		switch(type) {
			// bool
			case Uniform::UNIFORM_BOOL:{
				std::deque<bool> v;
				for(const auto & value : *values) {
					v.push_back(value.toBool());
				}
				return EScript::create(Uniform(name, v));
			}
			case Uniform::UNIFORM_VEC2B:{
				std::deque<bool> v;
				for(auto & value : *values) {
					// [x, y]
					Array * arr = value.to<Array*>(rt);
					if(arr->size()!=2){
						rt.setException("Vec2b expects 2 values");
						return nullptr;
					}
					v.push_back( arr->get(0)->toBool() );
					v.push_back( arr->get(1)->toBool() );
				}
				return EScript::create(Uniform(name, type, v));
			}
			case Uniform::UNIFORM_VEC3B:{
				std::deque<bool> v;
				for(auto & value : *values) {
					// [x, y, z]
					Array * arr = value.to<Array*>(rt);
					if(arr->size()!=3){
						rt.setException("Vec3b expects 3 values");
						return nullptr;
					}
					v.push_back( arr->get(0)->toBool() );
					v.push_back( arr->get(1)->toBool() );
					v.push_back( arr->get(2)->toBool() );
				}
				return EScript::create(Uniform(name, type, v));
			}
			case Uniform::UNIFORM_VEC4B:{
				std::deque<bool> v;
				for(auto & value : *values) {
					// [x,y, z, w]
					Array * arr = value.to<Array*>(rt);
					if(arr->size()!=4){
						rt.setException("Vec4b expects 4 values");
						return nullptr;
					}
					v.push_back( arr->get(0)->toBool() );
					v.push_back( arr->get(1)->toBool() );
					v.push_back( arr->get(2)->toBool() );
					v.push_back( arr->get(3)->toBool() );
				}
				return EScript::create(Uniform(name, type, v));
			}
			// float
			case Uniform::UNIFORM_FLOAT:{
				std::vector<float> v;
				for(auto & value : *values) {
					v.push_back(value.toFloat());
				}
				return EScript::create(Uniform(name, v));
			}
			case Uniform::UNIFORM_VEC2F:{
				std::vector<Geometry::Vec2> v;
				for(auto & value : *values) {
					// Vec2
					if(E_Geometry::E_Vec2 * ev = value.toType<E_Geometry::E_Vec2>()){
						v.push_back(ev->ref());
					}else{ // [x, y]
						Array * arr = value.to<Array*>(rt);
						if(arr->size()!=2){
							rt.setException("Vec2 expects 2 values");
							return nullptr;
						}
						v.emplace_back(arr->get(0)->toFloat(),arr->get(1)->toFloat());
					}
				}
				return EScript::create(Uniform(name, v));
			}
			case Uniform::UNIFORM_VEC3F:{
				std::vector<Geometry::Vec3> v;
				for(auto & value : *values) {
					// Vec3
					if(E_Geometry::E_Vec3 * ev = value.toType<E_Geometry::E_Vec3>()){
						v.push_back(ev->ref());
					}else{ // [x, y, z]
						Array * arr = value.to<Array*>(rt);
						if(arr->size()!=3){
							rt.setException("Vec3 expects 3 values");
							return nullptr;
						}
						v.emplace_back(arr->get(0)->toFloat(),arr->get(1)->toFloat(),arr->get(2)->toFloat());
					}
				}
				return EScript::create(Uniform(name, v));
			}
			case Uniform::UNIFORM_VEC4F:{
				std::vector<Geometry::Vec4> v;
				for(auto & value : *values) {
					// Vec4
					if(E_Geometry::E_Vec4 * ev = value.toType<E_Geometry::E_Vec4>()){
						v.push_back(ev->ref());
					} // Color4f
					else if(E_Util::E_Color4f * ec = value.toType<E_Util::E_Color4f>()){
						v.emplace_back(ec->ref().getR(),ec->ref().getG(),ec->ref().getB(),ec->ref().getA());
					}else{ // [x, y, z, w]
						Array * arr = value.to<Array*>(rt);
						if(arr->size()!=4){
							rt.setException("Vec4 expects 4 values");
							return nullptr;
						}
						v.emplace_back(arr->get(0)->toFloat(),arr->get(1)->toFloat(),arr->get(2)->toFloat(),arr->get(3)->toFloat());
					}
				}
				return EScript::create(Uniform(name, v));
			}
			// int
			case Uniform::UNIFORM_INT:{
				std::vector<int32_t> v;
				for(auto & value : *values) {
					v.push_back(value.to<uint32_t>(rt));
				}
				return EScript::create(Uniform(name, v));
			}
			case Uniform::UNIFORM_VEC2I:{
				std::vector<Geometry::Vec2i> v;
				for(auto & value : *values) {
					// Vec2
					if(E_Geometry::E_Vec2 * ev = value.toType<E_Geometry::E_Vec2>()){
						v.emplace_back(static_cast<int>(ev->ref().getX()),static_cast<int>(ev->ref().getY()));
					}else{ // [x, y]
						Array * arr = value.to<Array*>(rt);
						if(arr->size()!=2){
							rt.setException("Vec2i expects 2 values");
							return nullptr;
						}
						v.emplace_back(arr->get(0)->toInt(),arr->get(1)->toInt());
					}
				}
				return EScript::create(Uniform(name, v));
			}
			case Uniform::UNIFORM_VEC3I:{
				std::vector<Geometry::Vec3i> v;
				for(auto & value : *values) {
					// Vec3
					if(E_Geometry::E_Vec3 * ev = value.toType<E_Geometry::E_Vec3>()){
						v.emplace_back(static_cast<int>(ev->ref().getX()), static_cast<int>(ev->ref().getY()),
								static_cast<int>(ev->ref().getZ()));
					}else{ // [x, y, z]
						Array * arr = value.to<Array*>(rt);
						if(arr->size()!=3){
							rt.setException("Vec3i expects 3 values");
							return nullptr;
						}
						v.emplace_back(arr->get(0)->toInt(),arr->get(1)->toInt(),arr->get(2)->toInt());
					}
				}
				return EScript::create(Uniform(name, v));
			}
			case Uniform::UNIFORM_VEC4I:{
				std::vector<Geometry::Vec4i> v;
				for(auto & value : *values) {
					// Vec4
					if(E_Geometry::E_Vec4 * ev = value.toType<E_Geometry::E_Vec4>()){
						v.emplace_back(static_cast<int>(ev->ref().getX()),static_cast<int>(ev->ref().getY()),
								static_cast<int>(ev->ref().getZ()),static_cast<int>(ev->ref().getW()));
					}else{ // [x, y, z, w]
						Array * arr = value.to<Array*>(rt);
						if(arr->size()!=4){
							rt.setException("Vec4i expects 4 values");
							return nullptr;
						}
						v.emplace_back(arr->get(0)->toInt(),arr->get(1)->toInt(),arr->get(2)->toInt(),arr->get(3)->toInt());
					}
				}
				return EScript::create(Uniform(name, v));
			}
			// matrices
			case Uniform::UNIFORM_MATRIX_2X2F:{
				std::vector<float> v;
				for(auto & value : *values) {
					// [f1,f2,f3,f4]
					Array * arr = value.to<Array*>(rt);
					if(arr->size()!=4){
						rt.setException("Matrix2x2 expects 4 values");
						return nullptr;
					}
					for(const auto & matrixEntry : *arr) {
						v.push_back(matrixEntry.toFloat());
					}
				}
				return EScript::create(Uniform(name, type, v));
			}

			case Uniform::UNIFORM_MATRIX_3X3F:{
				std::vector<Geometry::Matrix3x3> v;
				for(auto & value : *values) {
					// Matrix3x3
					if(E_Geometry::E_Matrix3x3 * em = value.toType<E_Geometry::E_Matrix3x3>()){
						v.push_back(em->ref());
					}else {// [f*9]
						Array * arr = EScript::assertType<Array>( rt,value.get() );
						if(arr->size()!=9){
							rt.setException("Matrix3x3 expects 9 values");
							return nullptr;
						}
						v.emplace_back(arr->get(0)->toFloat(), arr->get(1)->toFloat(), arr->get(2)->toFloat(),
									   arr->get(3)->toFloat(), arr->get(4)->toFloat(), arr->get(5)->toFloat(),
									   arr->get(6)->toFloat(), arr->get(7)->toFloat(), arr->get(8)->toFloat());
					}
				}
				return EScript::create(Uniform(name, v));
			}
			case Uniform::UNIFORM_MATRIX_4X4F:{
				std::vector<Geometry::Matrix4x4> v;
				for(auto & value : *values) {
					// Matrix4x4
					if(E_Geometry::E_Matrix4x4 * em = value.toType<E_Geometry::E_Matrix4x4>()){
						v.push_back(em->ref());
					}else {// [f*16]
						Array * arr = EScript::assertType<Array>( rt,value.get() );
						if(arr->size()!=16){
							rt.setException("Matrix4x4 expects 16 values");
							return nullptr;
						}
						std::vector<float> matrixEntries;
						for(const auto & matrixEntry : *arr) {
							matrixEntries.push_back(matrixEntry.toFloat());
						}
						v.emplace_back(matrixEntries.data());
					}
				}
				return EScript::create(Uniform(name,v));
			}
			default:
				WARN("unexpected case in switch statement");
				break;
		}
		rt.setException("Invalid arguments for construction of E_Uniform");
		return nullptr;
	})

	//! [ESMF] String Uniform.getName()
	ES_MFUN(typeObject,Uniform,"getName",0,0,thisObj->getName())

	//! [ESMF] Number Uniform.getType()
	ES_MFUN(typeObject,Uniform,"getType",0,0,static_cast<uint32_t>(thisObj->getType()))

	//! [ESMF] Number Uniform.getNumValues()
	ES_MFUN(typeObject,Uniform,"getNumValues",0,0,static_cast<uint32_t>(thisObj->getNumValues()))

	//! [ESMF] Array Uniform.getData()
	ES_MFUN(typeObject,Uniform,"getData",0,0,getUniformData(*thisObj))

	//! [ESMF] Bool Uniform.isNull()
	ES_MFUN(typeObject,Uniform,"isNull",0,0,thisObj->isNull())

	declareConstant(typeObject,"BOOL",	static_cast<uint32_t>(Uniform::UNIFORM_BOOL));
	declareConstant(typeObject,"VEC2B",	static_cast<uint32_t>(Uniform::UNIFORM_VEC2B));
	declareConstant(typeObject,"VEC3B",	static_cast<uint32_t>(Uniform::UNIFORM_VEC3B));
	declareConstant(typeObject,"VEC4B",	static_cast<uint32_t>(Uniform::UNIFORM_VEC4B));
	declareConstant(typeObject,"FLOAT",	static_cast<uint32_t>(Uniform::UNIFORM_FLOAT));
	declareConstant(typeObject,"VEC2F",	static_cast<uint32_t>(Uniform::UNIFORM_VEC2F));
	declareConstant(typeObject,"VEC3F",	static_cast<uint32_t>(Uniform::UNIFORM_VEC3F));
	declareConstant(typeObject,"VEC4F",	static_cast<uint32_t>(Uniform::UNIFORM_VEC4F));
	declareConstant(typeObject,"INT",	static_cast<uint32_t>(Uniform::UNIFORM_INT));
	declareConstant(typeObject,"VEC2I",	static_cast<uint32_t>(Uniform::UNIFORM_VEC2I));
	declareConstant(typeObject,"VEC3I",	static_cast<uint32_t>(Uniform::UNIFORM_VEC3I));
	declareConstant(typeObject,"VEC4I",	static_cast<uint32_t>(Uniform::UNIFORM_VEC4I));
	declareConstant(typeObject,"MATRIX_2X2F",	static_cast<uint32_t>(Uniform::UNIFORM_MATRIX_2X2F));
	declareConstant(typeObject,"MATRIX_3X3F",	static_cast<uint32_t>(Uniform::UNIFORM_MATRIX_3X3F));
	declareConstant(typeObject,"MATRIX_4X4F",	static_cast<uint32_t>(Uniform::UNIFORM_MATRIX_4X4F));

}



}
