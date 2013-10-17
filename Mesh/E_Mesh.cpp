/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_Mesh.h"
#include "E_VertexDescription.h"
#include "../E_RenderingContext.h"

#include <E_Geometry/E_Box.h>
#include <E_Util/E_FileName.h>
#include <E_Util/E_Utils.h>
#include <EScript/EScript.h>

using namespace EScript;

namespace E_Rendering {

//! (static)
EScript::Type * E_Mesh::getTypeObject() {
	// E_Mesh ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! initMembers
void E_Mesh::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = getTypeObject();
	initPrintableName(typeObject,getClassName());
	declareConstant(&lib,getClassName(),typeObject);

	using namespace Rendering;
	
	//! [ESMF] VertexDescription Mesh.getVertexDescription()
	ES_MFUN(typeObject,const Mesh,"getVertexDescription",0,0,thisObj->getVertexDescription())

	//! [ESMF] Number Mesh.getVertexCount()
	ES_MFUN(typeObject,const Mesh,"getVertexCount",0,0,thisObj->getVertexCount())

	//! [ESMF] Array Mesh._getIndices()
	ES_MFUNCTION(typeObject,Mesh,"_getIndices",0,0,{
		MeshIndexData & indexData = thisObj->openIndexData();
		auto arr = EScript::Array::create();
		arr->reserve(indexData.getIndexCount());
		for(size_t idx = 0; idx<indexData.getIndexCount(); ++idx)
			arr->pushBack(EScript::create(indexData[idx]));
		return arr;
	})
	
	//! [ESMF] Number Mesh.getIndexCount()
	ES_MFUN(typeObject,const Mesh,"getIndexCount",0,0,thisObj->getIndexCount())

	//! [ESMF] Box Mesh.getBoundingBox()
	ES_MFUN(typeObject,const Mesh,"getBoundingBox",0,0,thisObj->getBoundingBox())

	//! [ESMF] FileName Mesh.getFileName()
	ES_MFUN(typeObject,const Mesh,"getFileName",0,0,	new E_Util::E_FileName(thisObj->getFileName()))
	
	//! [ESMF] FileName Mesh.getMainMemoryUsage()
	ES_MFUN(typeObject,const Mesh,"getMainMemoryUsage",0,0,	static_cast<double>(thisObj->getMainMemoryUsage()))
	
	//! [ESMF] FileName Mesh.getGraphicsMemoryUsage()
	ES_MFUN(typeObject,const Mesh,"getGraphicsMemoryUsage",0,0,	static_cast<double>(thisObj->getGraphicsMemoryUsage()))
		
	//! [ESMF] Number Mesh.getPrimitiveCount()
	ES_MFUN(typeObject,const Mesh,"getPrimitiveCount",0,0,	static_cast<double>(thisObj->getPrimitiveCount()))

	//! [ESMF] Bool Mesh.isUsingIndexData()
	ES_MFUN(typeObject,const Mesh,"isUsingIndexData",0,0,thisObj->isUsingIndexData())

	//! [ESMF] thisEObj Mesh.releaseLocalData()
	ES_MFUNCTION(typeObject, Mesh, "releaseLocalData", 0, 0, {
		thisObj->openVertexData().releaseLocalData();
		thisObj->openIndexData().releaseLocalData();
		return thisEObj;
	})

	//! [ESMF] thisEObj Mesh.releaseLocalData()
	ES_MFUNCTION(typeObject, Mesh, "assureLocalData", 0, 0, {
		thisObj->openVertexData();
		thisObj->openIndexData();
		return thisEObj;
	})
	
	//! [ESMF] thisEObj Mesh.setDrawLines()
	ES_MFUN(typeObject,Mesh,"setDrawLines",0,0,(thisObj->setDrawMode(Rendering::Mesh::DRAW_LINES),thisEObj))
		
	//! [ESMF] thisEObj Mesh.setDrawPoints()
	ES_MFUN(typeObject,Mesh,"setDrawPoints",0,0,(thisObj->setDrawMode(Rendering::Mesh::DRAW_POINTS),thisEObj))

	//! [ESMF] thisEObj Mesh.setDrawTriangles()
	ES_MFUN(typeObject,Mesh,"setDrawTriangles",0,0,(thisObj->setDrawMode(Rendering::Mesh::DRAW_TRIANGLES),thisEObj))

	//! [ESMF] thisEObj Mesh._setIndices(Array)
	ES_MFUNCTION(typeObject,Mesh,"_setIndices",1,1,{
		auto arr = parameter[0].to<EScript::Array*>(rt);
		MeshIndexData & indices = thisObj->_getIndexData();
		indices.allocate(arr->size());
		for(size_t i=0;i<arr->size();++i)
			indices[i] = arr->at(i).to<uint32_t>(rt);
		indices.updateIndexRange();
		return thisEObj;
	})
	
	//! [ESMF] thisEObj Mesh.setFileName([[FileName, String] name])
	ES_MFUNCTION(typeObject,Mesh,"setFileName",0,1,{
		if(parameter.count() == 0){
			thisObj->setFileName(Util::FileName());
		}else{
			E_Util::E_FileName * t = dynamic_cast<E_Util::E_FileName *>(parameter[0].get());
			if(t){
				thisObj->setFileName(t->ref());
			}else{
				thisObj->setFileName(Util::FileName(parameter[0].toString()));
			}
		}
		return thisEObj;
	})

	//! [ESMF] thisEObj Mesh.setUseIndexData(Bool)
	ES_MFUN(typeObject,Mesh,"setUseIndexData",1,1,(thisObj->setUseIndexData(parameter[0].toBool()),thisEObj))

	//! [ESMF] thisEObj Mesh.swap(Mesh)
	ES_MFUN(typeObject,Mesh,"swap",0,0,(thisObj->swap(*parameter[0].to<Mesh*>(rt)),thisEObj))

	//! [ESMF] thisEObj Mesh._markAsChanged() (wrapper for VertexData and IndexData.markAsChanged(), as these have no wrapper objects)
	ES_MFUNCTION(typeObject,Mesh,"_markAsChanged",0,0,{
		thisObj->openVertexData().markAsChanged();
		thisObj->openIndexData().markAsChanged();
		return thisEObj;
	})


	E_Util::E_Utils::registerConverter(new E_Util::E_Utils::CountedRefAttrConverter<Rendering::Mesh,E_Mesh>);
}

//---

E_Mesh::E_Mesh(Rendering::Mesh * t):ReferenceObject<Util::Reference<Rendering::Mesh> >(t,getTypeObject()){
}

std::string E_Mesh::toString() const{
	std::stringstream ss;
	ss << "#Mesh:" << (**this).get();
	return ss.str();
}

}
