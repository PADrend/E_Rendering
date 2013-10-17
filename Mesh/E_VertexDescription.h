/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef RENDERING_E_VERTEX_DESCRIPTION_H
#define RENDERING_E_VERTEX_DESCRIPTION_H

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/Mesh/VertexDescription.h>
#include <string>

namespace EScript {
class Namespace;
class Runtime;
class Type;
}
namespace E_Rendering {

/**
 * [E_VertexDescription] ---|> [ReferenceObject] ---|> [Object]
 */
class E_VertexDescription : public EScript::ReferenceObject<Rendering::VertexDescription> {
		ES_PROVIDES_TYPE_NAME(VertexDescription)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_VertexDescription(const Rendering::VertexDescription & description) : ReferenceObject_t(description, getTypeObject()) {}
		virtual ~E_VertexDescription() {}

		E_VertexDescription * clone() const override	{	return new E_VertexDescription(ref());	}
		std::string toString() const override			{	return ref().toString();	}
};

}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_VertexDescription,		Rendering::VertexDescription*, 		&**eObj)
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_VertexDescription,		Rendering::VertexDescription&, 		**eObj)
ES_CONV_OBJ_TO_EOBJ(const Rendering::VertexDescription&,	E_Rendering::E_VertexDescription,	new E_Rendering::E_VertexDescription(obj))

#endif // RENDERING_E_VERTEX_DESCRIPTION_H
