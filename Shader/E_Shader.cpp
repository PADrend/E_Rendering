/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_Shader.h"

#include <Util/IO/FileName.h>
#include <Util/Macros.h>
#include "../E_RenderingContext.h"
#include "E_Uniform.h"

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

using namespace Rendering;
using namespace EScript;

namespace E_Rendering {

//! (static)
EScript::Type * E_Shader::getTypeObject() {
	// E_Shader ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! initMembers
void E_Shader::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	declareConstant(typeObject, "USE_GL", Number::create(Shader::USE_GL));
	declareConstant(typeObject, "USE_UNIFORMS", Number::create(Shader::USE_UNIFORMS));



	//! [ESF] (static) Shader Shader.loadShader(string vsFile, string fsFile, flag usage)
	ES_FUNCTION(typeObject,"loadShader",2,3,{
		const Util::FileName vsFile(parameter[0].toString());
		const Util::FileName fsFile(parameter[1].toString());
		const Shader::flag_t flag = parameter[2].toInt(Shader::USE_GL | Shader::USE_UNIFORMS);
		Shader * s=Shader::loadShader(vsFile, fsFile, flag);
		if(s)
			return new E_Shader(s);
		else
			return EScript::create(nullptr);
	})

	//! [ESF] (static) Shader Shader.createShader( [usage] | string vs, string fs[, flag usage])
	ES_FUNCTION(typeObject,"createShader",0,3,{
		if(parameter.size()>1){
			return EScript::create(
						Shader::createShader(parameter[0].toString().c_str(), parameter[1].toString().c_str(), parameter[2].to<uint32_t>(rt,Shader::USE_GL | Shader::USE_UNIFORMS)));
		}else{
			return EScript::create(
				Shader::createShader(parameter[0].to<uint32_t>(rt,Shader::USE_GL | Shader::USE_UNIFORMS)));
		}
	})

	// -------------
	//! [ESF] thisEObj Shader.attachFS(string code)
	ES_MFUN(typeObject,Shader,"attachFS",1,1,
		(thisObj->attachShaderObject(Rendering::ShaderObjectInfo::createFragment(parameter[0].toString())),thisEObj))

	//! [ESF] thisEObj Shader.attachFSFile(string file)
	ES_MFUN(typeObject,Shader,"attachFSFile",1,1,
		(thisObj->attachShaderObject(Rendering::ShaderObjectInfo::loadFragment(Util::FileName(parameter[0].toString()))),thisEObj))

	//! [ESF] thisEObj Shader.attachGS(string code)
	ES_MFUN(typeObject,Shader,"attachGS",1,1,
		(thisObj->attachShaderObject(Rendering::ShaderObjectInfo::createGeometry(parameter[0].toString())),thisEObj))

	//! [ESF] thisEObj Shader.attachGSFile(string file)
	ES_MFUN(typeObject,Shader,"attachGSFile",1,1,
		(thisObj->attachShaderObject(Rendering::ShaderObjectInfo::loadGeometry(Util::FileName(parameter[0].toString()))),thisEObj))

	//! [ESF] thisEObj Shader.attachVS(string code)
	ES_MFUN(typeObject,Shader,"attachVS",1,1,
		(thisObj->attachShaderObject(Rendering::ShaderObjectInfo::createVertex(parameter[0].toString())),thisEObj))

	//! [ESF] thisEObj Shader.attachVSFile(string file)
	ES_MFUN(typeObject,Shader,"attachVSFile",1,1,
		(thisObj->attachShaderObject(Rendering::ShaderObjectInfo::loadVertex(Util::FileName(parameter[0].toString()))),thisEObj))

	//! [ESMF] thisEObj Shader.defineVertexAttribute(name,index)
	ES_MFUN(typeObject,Shader,"defineVertexAttribute",2,2,
		(thisObj->defineVertexAttribute( parameter[0].toString() , parameter[1].to<uint32_t>(rt)),thisEObj))

	//! [ESF] Unfiform* Shader Shader.getActiveUniforms()
	ES_MFUNCTION(typeObject,Shader, "getActiveUniforms", 0, 0, {
		std::vector<Uniform> u;
		thisObj->getActiveUniforms(u);
		Array * a = Array::create();
		for(const auto & uniform : u) {
			a->pushBack(EScript::create(uniform));
		}
		return a;
	})

	//! [ESMF] Uniform|Void Shader.getUniform( Name )
	ES_MFUNCTION(typeObject,Shader,"getUniform",1,1,{
		const Uniform & u = thisObj->getUniform(parameter[0].toString());
		if(u.isNull())
			return EScript::create(nullptr);
		return EScript::create(u);
	})
	
	//! [ESF] Bool Shader.isInvalid()
	ES_MFUN(typeObject,Shader,"isInvalid",0,0, thisObj->getStatus()==Shader::INVALID);

	//! [ESMF] thisEObj Shader.setUniform( RenderingContext rc,Uniform ,bool warnIfUnused = true, bool forced = false )
	ES_MFUN(typeObject,Shader,"setUniform",2,4,
		(thisObj->setUniform(parameter[0].to<RenderingContext&>(rt),
											parameter[1].to<const Rendering::Uniform&>(rt),
											parameter[2].toBool(true),parameter[3].toBool(false)),thisEObj))

}

}
