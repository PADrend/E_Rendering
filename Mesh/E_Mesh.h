/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef E_MESH_H
#define E_MESH_H

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/Mesh/Mesh.h>
#include <Util/References.h>

namespace E_Rendering{

/**
 **   [E_Mesh] ---|> [ReferenceObject] ---|> [Object]
 **/
class E_Mesh : public EScript::ReferenceObject<Util::Reference<Rendering::Mesh> > {
		ES_PROVIDES_TYPE_NAME(Mesh)
	public:
		static EScript::Type* getTypeObject();
		static void init(EScript::Namespace & lib);

		E_Mesh(Rendering::Mesh * t):ReferenceObject<Util::Reference<Rendering::Mesh> >(t,getTypeObject()){}
		E_Mesh(const Util::Reference<Rendering::Mesh>& t):ReferenceObject<Util::Reference<Rendering::Mesh> >(t,getTypeObject()){}
		virtual ~E_Mesh(){}

		E_Mesh * clone() const override {
			return new E_Mesh((**this)->clone());
		}
		std::string toString() const override;
};
}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_Mesh,					Rendering::Mesh*, 		(**eObj).get())
ES_CONV_OBJ_TO_EOBJ(Rendering::Mesh *,						E_Rendering::E_Mesh,	obj?new E_Rendering::E_Mesh(obj) : nullptr)
ES_CONV_OBJ_TO_EOBJ(Util::Reference<Rendering::Mesh>,		E_Rendering::E_Mesh,	obj?new E_Rendering::E_Mesh(obj) : nullptr)

#endif // E_MESH_H
