/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef E_VERTEXATTRIBUTE_H_
#define E_VERTEXATTRIBUTE_H_

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/Mesh/VertexAttribute.h>
#include <string>

namespace EScript {
class Namespace;
class Runtime;
class Type;
}
namespace E_Rendering {

/**
 * [E_VertexAttribute] ---|> [ReferenceObject] ---|> [Object]
 */
class E_VertexAttribute : public EScript::ReferenceObject<Rendering::VertexAttribute> {
		ES_PROVIDES_TYPE_NAME(Attribute)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_VertexAttribute(const Rendering::VertexAttribute & attribute) : ReferenceObject_t(attribute, getTypeObject()) {
		}
		virtual ~E_VertexAttribute() {
		}

		E_VertexAttribute * clone() const override {
			return new E_VertexAttribute(ref());
		}
		std::string toString() const override {
			return ref().toString();
		}
};

}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_VertexAttribute,		Rendering::VertexAttribute&,			**eObj)
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_VertexAttribute,		Rendering::VertexAttribute*,			&**eObj)
ES_CONV_OBJ_TO_EOBJ(const Rendering::VertexAttribute&, 	E_Rendering::E_VertexAttribute,			new E_Rendering::E_VertexAttribute(obj))

#endif /* E_VERTEXATTRIBUTE_H_ */
