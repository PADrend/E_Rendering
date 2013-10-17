/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef __E_UNIFORM_H
#define __E_UNIFORM_H

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/Shader/Uniform.h>
#include <Util/References.h>

namespace E_Rendering{

/**
 *   [E_Uniform] ---|> [ReferenceObject] ---> Uniform
 */
class E_Uniform : public EScript::ReferenceObject<Rendering::Uniform>{
	ES_PROVIDES_TYPE_NAME(Uniform)
	public:
		static EScript::Type* getTypeObject();
		static void init(EScript::Namespace & lib);

		E_Uniform(const Rendering::Uniform & uniform,EScript::Type * type=nullptr) : ReferenceObject_t(uniform,type?type:getTypeObject()) {}
		virtual ~E_Uniform() {}

		E_Uniform * clone() const override					{	return new E_Uniform(ref());	}
		std::string toString() const override				{	return ref().toString();	}
};
}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_Uniform,		Rendering::Uniform&,			**eObj)
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_Uniform,		Rendering::Uniform*,			&**eObj)
ES_CONV_OBJ_TO_EOBJ(const Rendering::Uniform&, 	E_Rendering::E_Uniform,			new E_Rendering::E_Uniform(obj))

#endif // __E_UNIFORM_H
