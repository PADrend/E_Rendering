/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef E_ParameterStructs_H
#define E_ParameterStructs_H

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/RenderingContext/RenderingParameters.h>
#include <Util/References.h>

namespace E_Rendering{

namespace E_ParameterStructs{

void init(EScript::Namespace & lib);

/*! EScript wrapper for Rendering::AlphaTestParameters.
	[E_AlphaTestParameters] ---|> [ReferenceObject] ---|> [Object]  */
class E_AlphaTestParameters : public EScript::ReferenceObject<Rendering::AlphaTestParameters> {
		ES_PROVIDES_TYPE_NAME(AlphaTestParameters)
	public:
		static EScript::Type* getTypeObject();
		E_AlphaTestParameters(const Rendering::AlphaTestParameters & p) : EScript::ReferenceObject<Rendering::AlphaTestParameters>(p,getTypeObject()){
		}

		//! ---|> Object
		E_AlphaTestParameters * clone() const override		{	return new E_AlphaTestParameters(ref());	}
};

/*! EScript wrapper for Rendering::BlendingParameters.
	[E_BlendingParameters] ---|> [ReferenceObject] ---|> [Object]  */
class E_BlendingParameters : public EScript::ReferenceObject<Rendering::BlendingParameters> {
		ES_PROVIDES_TYPE_NAME(BlendingParameters)
	public:
		static EScript::Type* getTypeObject();
		E_BlendingParameters(const Rendering::BlendingParameters & p) : EScript::ReferenceObject<Rendering::BlendingParameters>(p,getTypeObject()){
		}

		//! ---|> Object
		E_BlendingParameters * clone() const override		{	return new E_BlendingParameters(ref());	}
};

/*! EScript wrapper for Rendering::CullFaceParameters.
	[E_CullFaceParameters] ---|> [ReferenceObject] ---|> [Object]  */
class E_CullFaceParameters : public EScript::ReferenceObject<Rendering::CullFaceParameters> {
		ES_PROVIDES_TYPE_NAME(CullFaceParameters)
	public:
		static EScript::Type* getTypeObject();
		E_CullFaceParameters(const Rendering::CullFaceParameters & p) : EScript::ReferenceObject<Rendering::CullFaceParameters>(p, getTypeObject()) {
		}

		//! ---|> Object
		E_CullFaceParameters * clone() const override      {   return new E_CullFaceParameters(ref());  }
};

/*! EScript wrapper for Rendering::ImageBindParameters.
	[E_ImageBindParameters] ---|> [ReferenceObject] ---|> [Object]  */
class E_ImageBindParameters : public EScript::ReferenceObject<Rendering::ImageBindParameters> {
		ES_PROVIDES_TYPE_NAME(ImageBindParameters)
	public:
		static EScript::Type* getTypeObject();
		E_ImageBindParameters(const Rendering::ImageBindParameters & p);
		virtual ~E_ImageBindParameters();

		//! ---|> Object
		E_ImageBindParameters * clone() const override      {   return new E_ImageBindParameters(ref());  }
};

/*! EScript wrapper for Rendering::LightingParameters.
	[E_LightingParameters] ---|> [ReferenceObject] ---|> [Object]  */
class E_LightingParameters : public EScript::ReferenceObject<Rendering::LightingParameters> {
		ES_PROVIDES_TYPE_NAME(LightingParameters)
	public:
		static EScript::Type* getTypeObject();
		E_LightingParameters(const Rendering::LightingParameters & p) : EScript::ReferenceObject<Rendering::LightingParameters>(p, getTypeObject()) {
		}

		//! ---|> Object
		E_LightingParameters * clone() const override      {   return new E_LightingParameters(ref());  }
};

/*! EScript wrapper for Rendering::MaterialParameters.
	[E_MaterialParameters] ---|> [ReferenceObject] ---|> [Object]  */
class E_MaterialParameters : public EScript::ReferenceObject<Rendering::MaterialParameters> {
		ES_PROVIDES_TYPE_NAME(MaterialParameters)
	public:
		static EScript::Type* getTypeObject();
		E_MaterialParameters(const Rendering::MaterialParameters & p) : EScript::ReferenceObject<Rendering::MaterialParameters>(p, getTypeObject()) {
		}

		//! ---|> Object
		E_MaterialParameters * clone() const override      {   return new E_MaterialParameters(ref());  }
};

/*! EScript wrapper for Rendering::PointParameters.
	[E_PointParameters] ---|> [ReferenceObject] ---|> [Object]  */
class E_PointParameters : public EScript::ReferenceObject<Rendering::PointParameters> {
		ES_PROVIDES_TYPE_NAME(PointParameters)
	public:
		static EScript::Type* getTypeObject();
		E_PointParameters(const Rendering::PointParameters & p) : 
					EScript::ReferenceObject<Rendering::PointParameters>(p, getTypeObject()) {}
		virtual ~E_PointParameters(){}
		//! ---|> Object
		E_PointParameters * clone() const override      {   return new E_PointParameters(ref());  }
};

/*! EScript wrapper for Rendering::PolygonModeParameters.
	[E_PolygonModeParameters] ---|> [ReferenceObject] ---|> [Object]  */
class E_PolygonModeParameters : public EScript::ReferenceObject<Rendering::PolygonModeParameters> {
		ES_PROVIDES_TYPE_NAME(PolygonModeParameters)
	public:
		static EScript::Type* getTypeObject();
		E_PolygonModeParameters(const Rendering::PolygonModeParameters & p) : EScript::ReferenceObject<Rendering::PolygonModeParameters>(p,getTypeObject()){
		}

		//! ---|> Object
		E_PolygonModeParameters * clone() const override		{	return new E_PolygonModeParameters(ref());	}
};

/*! EScript wrapper for Rendering::StencilParameters.
	[E_StencilParameters] ---|> [ReferenceObject] ---|> [Object]  */
class E_StencilParameters : public EScript::ReferenceObject<Rendering::StencilParameters> {
		ES_PROVIDES_TYPE_NAME(StencilParameters)
	public:
		static EScript::Type* getTypeObject();
		E_StencilParameters(const Rendering::StencilParameters & p) : EScript::ReferenceObject<Rendering::StencilParameters>(p,getTypeObject()){
		}

		//! ---|> Object
		E_StencilParameters * clone() const override		{	return new E_StencilParameters(ref());	}
};

/*! EScript wrapper for Rendering::ScissorParameters.
	[E_ScissorParameters] ---|> [ReferenceObject] ---|> [Object]  */
class E_ScissorParameters : public EScript::ReferenceObject<Rendering::ScissorParameters> {
		ES_PROVIDES_TYPE_NAME(ScissorParameters)
	public:
		static EScript::Type* getTypeObject();
		E_ScissorParameters(const Rendering::ScissorParameters & p) : EScript::ReferenceObject<Rendering::ScissorParameters>(p, getTypeObject()) {
		}

		//! ---|> Object
		E_ScissorParameters * clone() const override		{	return new E_ScissorParameters(ref());	}
};


}
}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_AlphaTestParameters,		Rendering::AlphaTestParameters*,							&**eObj)
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_AlphaTestParameters,		Rendering::AlphaTestParameters&,							**eObj)
ES_CONV_OBJ_TO_EOBJ(const Rendering::AlphaTestParameters&, 						E_Rendering::E_ParameterStructs::E_AlphaTestParameters,		new E_Rendering::E_ParameterStructs::E_AlphaTestParameters(obj))

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_BlendingParameters,		Rendering::BlendingParameters*,								&**eObj)
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_BlendingParameters,		Rendering::BlendingParameters&,								**eObj)
ES_CONV_OBJ_TO_EOBJ(const Rendering::BlendingParameters&, 						E_Rendering::E_ParameterStructs::E_BlendingParameters,		new E_Rendering::E_ParameterStructs::E_BlendingParameters(obj))

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_CullFaceParameters,		Rendering::CullFaceParameters*,								&**eObj)
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_CullFaceParameters,		Rendering::CullFaceParameters&,								**eObj)
ES_CONV_OBJ_TO_EOBJ(const Rendering::CullFaceParameters&, 						E_Rendering::E_ParameterStructs::E_CullFaceParameters,		new E_Rendering::E_ParameterStructs::E_CullFaceParameters(obj))

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_ImageBindParameters,		Rendering::ImageBindParameters*,							&**eObj)
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_ImageBindParameters,		Rendering::ImageBindParameters&,							**eObj)
ES_CONV_OBJ_TO_EOBJ(const Rendering::ImageBindParameters&, 						E_Rendering::E_ParameterStructs::E_ImageBindParameters,		new E_Rendering::E_ParameterStructs::E_ImageBindParameters(obj))

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_LightingParameters,		Rendering::LightingParameters*,								&**eObj)
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_LightingParameters,		Rendering::LightingParameters&,								**eObj)
ES_CONV_OBJ_TO_EOBJ(const Rendering::LightingParameters&, 						E_Rendering::E_ParameterStructs::E_LightingParameters,		new E_Rendering::E_ParameterStructs::E_LightingParameters(obj))

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_MaterialParameters,		Rendering::MaterialParameters*,								&**eObj)
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_MaterialParameters,		Rendering::MaterialParameters&,								**eObj)
ES_CONV_OBJ_TO_EOBJ(const Rendering::MaterialParameters&, 						E_Rendering::E_ParameterStructs::E_MaterialParameters,		new E_Rendering::E_ParameterStructs::E_MaterialParameters(obj))

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_PointParameters,			Rendering::PointParameters*,								&**eObj)
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_PointParameters,			Rendering::PointParameters&,								**eObj)
ES_CONV_OBJ_TO_EOBJ(const Rendering::PointParameters&, 							E_Rendering::E_ParameterStructs::E_PointParameters,	 		new E_Rendering::E_ParameterStructs::E_PointParameters(obj))

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_PolygonModeParameters,	Rendering::PolygonModeParameters*,							&**eObj)
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_PolygonModeParameters,	Rendering::PolygonModeParameters&,							**eObj)
ES_CONV_OBJ_TO_EOBJ(const Rendering::PolygonModeParameters&, 					E_Rendering::E_ParameterStructs::E_PolygonModeParameters,	new E_Rendering::E_ParameterStructs::E_PolygonModeParameters(obj))

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_ScissorParameters,		Rendering::ScissorParameters*,								&**eObj)
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_ScissorParameters,		Rendering::ScissorParameters&,								**eObj)
ES_CONV_OBJ_TO_EOBJ(const Rendering::ScissorParameters&, 						E_Rendering::E_ParameterStructs::E_ScissorParameters,		new E_Rendering::E_ParameterStructs::E_ScissorParameters(obj))

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_StencilParameters,		Rendering::StencilParameters*,								&**eObj)
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ParameterStructs::E_StencilParameters,		Rendering::StencilParameters&,								**eObj)
ES_CONV_OBJ_TO_EOBJ(const Rendering::StencilParameters&, 						E_Rendering::E_ParameterStructs::E_StencilParameters,		new E_Rendering::E_ParameterStructs::E_StencilParameters(obj))

#endif // E_ParameterStructs_H
