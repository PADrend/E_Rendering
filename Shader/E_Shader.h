/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef __E_SHADER_H
#define __E_SHADER_H

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/Shader/Shader.h>
#include <Util/References.h>

namespace E_Rendering{

/**
 *   [E_Shader] ---|> [ReferenceObject] ---> Shader
 */
class E_Shader : public EScript::ReferenceObject<Util::Reference<Rendering::Shader> > {
	ES_PROVIDES_TYPE_NAME(Shader)

	public:
		static EScript::Type* getTypeObject();
		static void init(EScript::Namespace & lib);

		E_Shader(Rendering::Shader * shader,EScript::Type * type=nullptr) : ReferenceObject_t(shader,type?type:getTypeObject())	{}
		virtual ~E_Shader()	{}
};
}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_Shader,	Rendering::Shader*,		(**eObj).get())
ES_CONV_OBJ_TO_EOBJ(Rendering::Shader*, 	E_Rendering::E_Shader,	new E_Rendering::E_Shader(obj))

#endif // __E_SHADER_H
