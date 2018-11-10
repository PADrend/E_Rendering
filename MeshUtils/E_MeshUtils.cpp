/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_MeshUtils.h"
#include <sstream>

#include "../Mesh/E_Mesh.h"
#include "../Mesh/E_VertexDescription.h"
#include <Geometry/Tools.h>
#include <Rendering/Serialization/Serialization.h>
#include <Rendering/Mesh/VertexAttributeIds.h>
#include <Rendering/MeshUtils/MarchingCubesMeshBuilder.h>
#include <Rendering/MeshUtils/MeshUtils.h>
#include <Rendering/MeshUtils/QuadtreeMeshBuilder.h>
#include <Rendering/MeshUtils/Simplification.h>
#include <Util/Graphics/PixelAccessor.h>
#include <Util/IO/FileName.h>
#include <E_Geometry/E_Matrix4x4.h>
#include <E_Geometry/E_Vec3.h>
#include <E_Geometry/E_Ray3.h>
#include <E_Util/Graphics/E_Color.h>
#include <E_Util/Graphics/E_PixelAccessor.h>
#include <E_Util/Graphics/E_Bitmap.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>
#include <memory>
#include <set>

namespace E_Rendering{

using namespace EScript;
using namespace Rendering;
using namespace Rendering::MeshUtils;
using namespace E_Geometry;
using namespace Geometry;


void initMeshUtils(EScript::Namespace * lib) {

	// -----------------------------------------------------------------------------------------------------
	// -----------------------------------------------------------------------------------------------------
	// MeshUtils

	//! [ESF] void Rendering.calculateNormals(Mesh)
	ES_FUN(lib,"calculateNormals", 1, 1, (Rendering::MeshUtils::calculateNormals(parameter[0].to<Rendering::Mesh*>(rt)), EScript::create(nullptr)))

	//! [ESF] Mesh Rendering.combineMeshes(Array of Meshes)
	//! [ESF] Mesh Rendering.combineMeshes(Array of Meshes [,Array of Transformation Matrixes])
	ES_FUNCTION(lib,"combineMeshes",1,2,{
		Array * a = parameter[0].to<EScript::Array*>(rt);
		std::deque<Mesh *> meshes;

		if(a->empty())
			return EScript::create(new Mesh);

		for(ERef<Iterator> i = a->getIterator();!i->end() ; i->next()){
			ObjRef value=i->value();
			meshes.push_back(value.to<Mesh*>(rt));
			if (meshes.front()->getDrawMode() != meshes.back()->getDrawMode())
				rt.setException("Invalid Argument. Cannot combine different types of meshes");
		}
		if(parameter.count()==1){
			Mesh * result = MeshUtils::combineMeshes(meshes);
			result->setDrawMode(meshes.front()->getDrawMode());
			result->setUseIndexData(meshes.front()->isUsingIndexData());
			return EScript::create(result);
		}
		Array * t = parameter[1].to<EScript::Array*>(rt);
		std::deque<Matrix4x4> matrices;
		for( ERef<Iterator> i=t->getIterator();!i->end() ; i->next()){
			ObjRef value=i->value();
			matrices.push_back(value.to<const Geometry::Matrix4x4&>(rt));
		}
		Mesh * result = MeshUtils::combineMeshes(meshes, matrices);
		result->setDrawMode(meshes.front()->getDrawMode());
		result->setUseIndexData(meshes.front()->isUsingIndexData());
		return EScript::create(result);
	})

	//! [ESF] [Array|Void] Rendering.splitPointMeshIntoSmallerMeshes(Mesh, Points per Submesh)
	ES_FUNCTION(lib, "splitPointMeshIntoSmallerMeshes", 2, 2, {
			Mesh * mesh = parameter[0].to<Rendering::Mesh*>(rt);
			uint32_t chunkSize = parameter[1].to<uint32_t>(rt);

			if (mesh->getDrawMode() != Mesh::DRAW_POINTS){
				return EScript::create(nullptr);
			}

			std::deque<MeshVertexData> submeshes = MeshUtils::splitVertexData(mesh, chunkSize);

			Array * a = Array::create();

			for (std::deque<MeshVertexData>::iterator it = submeshes.begin(); it != submeshes.end(); ++it){
				Mesh * currentMesh = new Mesh();
				currentMesh->setUseIndexData(false);
				currentMesh->setDrawMode(Mesh::DRAW_POINTS);
				MeshVertexData & currentVertices = currentMesh->openVertexData();
				currentVertices.swap(*(&(*it)));
				currentVertices._setBoundingBox(mesh->getBoundingBox());
				a->pushBack(EScript::create(currentMesh));
			}

			return a;
	})

	//! [ESF] [Mesh|Void] Rendering.extractPointsFromMesh(Mesh, First Point, Number of Points)
	ES_FUNCTION(lib, "extractPointsFromMesh", 3, 3, {
			Mesh * mesh = parameter[0].to<Rendering::Mesh*>(rt);
			uint32_t begin = parameter[1].to<uint32_t>(rt);
			uint32_t length = parameter[2].to<uint32_t>(rt);

			if (mesh->getDrawMode() != Mesh::DRAW_POINTS){
				return EScript::create(nullptr);
			}

			MeshVertexData * vertices = MeshUtils::extractVertexData(mesh, begin, length);

			if (vertices == nullptr){
				return EScript::create(nullptr);
			}

			Mesh * result = new Mesh();
			result->setUseIndexData(false);
			result->setDrawMode(Mesh::DRAW_POINTS);
			MeshVertexData & resultVertices = result->openVertexData();
			resultVertices.swap(*vertices);
			resultVertices._setBoundingBox(mesh->getBoundingBox());

			return EScript::create(result);
	})

	//! [ESF] void Rendering.calculateTangentVectors(Mesh, String uvAttrName, String tanAttrName)
	ES_FUN(lib,"calculateTangentVectors", 3, 3, (Rendering::MeshUtils::calculateTangentVectors(
			parameter[0].to<Rendering::Mesh*>(rt),parameter[1].toString(),parameter[2].toString()), EScript::create(nullptr)))

	//! [ESF] void Rendering.calculateTextureCoordinates_projection(Mesh, String AttrName, Matrix4x4 projection)
	ES_FUN(lib,"calculateTextureCoordinates_projection", 3, 3, (Rendering::MeshUtils::calculateTextureCoordinates_projection(
			parameter[0].to<Rendering::Mesh*>(rt),parameter[1].toString(),
				parameter[2].to<const Geometry::Matrix4x4&>(rt)), EScript::create(nullptr)))

	//! [ESF] Void Rendering.convertVertices(Mesh, VertexDescription)
	ES_FUNCTION(lib, "convertVertices", 2, 2, {
		Rendering::Mesh * mesh = parameter[0].to<Rendering::Mesh*>(rt);
		Rendering::VertexDescription vertexDesc = EScript::assertType<E_VertexDescription>(rt, parameter[1])->ref();

		Rendering::MeshVertexData & oldData = mesh->openVertexData();
		std::unique_ptr<Rendering::MeshVertexData> newData(Rendering::MeshUtils::convertVertices(oldData, vertexDesc));
		oldData.swap(*newData);
		return EScript::create(nullptr);
	})

	//! [ESF] void Rendering.copyVertexAttribute(mesh, String AttrNameFrom, String AttrNameTo)
	ES_FUN(lib,"copyVertexAttribute", 3, 3, (Rendering::MeshUtils::copyVertexAttribute(
		parameter[0].to<Rendering::Mesh*>(rt),
		parameter[1].toString(),
		parameter[2].toString()
	), EScript::create(nullptr)))

	//! [ESF] void Rendering.eliminateDuplicateVertices(Mesh)
	ES_FUN(lib,"eliminateDuplicateVertices",1,1,(
		Rendering::MeshUtils::eliminateDuplicateVertices(parameter[0].to<Rendering::Mesh*>(rt)),
		EScript::create(nullptr)
	))

	//! [ESF] void Rendering.eliminateLongTriangles(Mesh,ratio)
	ES_FUNCTION(lib,"eliminateLongTriangles",2,2,{
		Mesh * m=parameter[0].to<Rendering::Mesh*>(rt);
		Util::Reference<Mesh> m2(Rendering::MeshUtils::eliminateLongTriangles(m,parameter[1].toFloat()));
		m->_getIndexData().swap( m2->_getIndexData() );
		m->_getVertexData().swap( m2->_getVertexData() );
		return EScript::create(nullptr);
	})

	//! [ESF] void Rendering.eliminateTrianglesBehindPlane(Mesh, Vec3, Vec3)
	ES_FUNCTION(lib, "eliminateTrianglesBehindPlane", 3, 3, {
		Mesh * mesh = parameter[0].to<Rendering::Mesh*>(rt);
		const Geometry::Vec3f & position = parameter[1].to<Geometry::Vec3>(rt);
		const Geometry::Vec3f & normal = parameter[2].to<Geometry::Vec3>(rt);
		Util::Reference<Mesh> newMesh(Rendering::MeshUtils::eliminateTrianglesBehindPlane(mesh, Geometry::Plane(position, normal)));
		mesh->_getIndexData().swap(newMesh->_getIndexData());
		mesh->_getVertexData().swap(newMesh->_getVertexData());
		return EScript::create(nullptr);
	})

	//! [ESF] void Rendering.eliminateZeroAreaTriangles(Mesh)
	ES_FUNCTION(lib, "eliminateZeroAreaTriangles", 1, 1, {
		Mesh * mesh = parameter[0].to<Rendering::Mesh*>(rt);
		Util::Reference<Mesh> newMesh(Rendering::MeshUtils::eliminateZeroAreaTriangles(mesh));
		mesh->_getIndexData().swap(newMesh->_getIndexData());
		mesh->_getVertexData().swap(newMesh->_getVertexData());
		return EScript::create(nullptr);
	})

	//! [ESF] void Rendering.removeSkinsWithHoleCovering(Mesh, Number, Number)
	ES_FUNCTION(lib, "removeSkinsWithHoleCovering", 3, 3, {
		Mesh * mesh = parameter[0].to<Rendering::Mesh*>(rt);
		Util::Reference<Mesh> newMesh(Rendering::MeshUtils::removeSkinsWithHoleCovering(mesh, parameter[1].toFloat(), parameter[2].toFloat()));
		mesh->_getIndexData().swap(newMesh->_getIndexData());
		mesh->_getVertexData().swap(newMesh->_getVertexData());
		return EScript::create(nullptr);
	})

	//! [ESF] void Rendering.eliminateUnusedVertices(Mesh)
	ES_FUNCTION(lib,"eliminateUnusedVertices",1,1,{
		Mesh * m=parameter[0].to<Rendering::Mesh*>(rt);
		Util::Reference<Mesh> m2(Rendering::MeshUtils::eliminateUnusedVertices(m));
		m->_getIndexData().swap( m2->_getIndexData() );
		m->_getVertexData().swap( m2->_getVertexData() );
		return EScript::create(nullptr);
	})

	//! [ESF] Mesh loadMesh(String)
	ES_FUN(lib, "loadMesh", 1, 1, EScript::create(Rendering::Serialization::loadMesh(Util::FileName(parameter[0].toString()))))

	//! [ESF] Void Rendering.optimizeIndices(Mesh)
	ES_FUNCTION(lib, "optimizeIndices", 1, 2, {
		Mesh * mesh = parameter[0].to<Rendering::Mesh*>(rt);
		Rendering::MeshUtils::optimizeIndices(mesh, parameter[1].toInt(24));
		return EScript::create(nullptr);
	})

	//! [ESF] Void Rendering.shrinkMesh(Mesh, [bool])
	ES_FUNCTION(lib, "shrinkMesh", 1, 2, {
		Mesh * mesh = parameter[0].to<Rendering::Mesh*>(rt);
		MeshUtils::shrinkMesh(mesh, parameter[1].toBool(false));
		return EScript::create(nullptr);
	})

	//! [ESF] void Rendering.reverseMeshWinding(Mesh)
	ES_FUN(lib,"reverseMeshWinding", 1, 1, (Rendering::MeshUtils::reverseWinding(parameter[0].to<Rendering::Mesh*>(rt)), EScript::create(nullptr)))

	//! [ESF] void saveMesh(Mesh *, string s)
	ES_FUNCTION(lib,"saveMesh",2,2,{
		Mesh * m= parameter[0].to<Rendering::Mesh*>(rt);
		Util::FileName file(parameter[1].toString());
		return Bool::create(Rendering::Serialization::saveMesh(m, file));
	})

	//! [ESF] void Rendering.setColor(Mesh, Color)
	ES_FUN(lib,"setColor",2,2,(Rendering::MeshUtils::setColor(
				parameter[0].to<Rendering::Mesh*>(rt),
				parameter[1].to<Util::Color4f>(rt) ),EScript::create(nullptr)))

	//! [ESF] void Rendering.setMaterial(Mesh, Color ambient, Color diffuse, Color specular, Number shininess)
	ES_FUN(lib,"setMaterial",5,5,(Rendering::MeshUtils::setMaterial(
				parameter[0].to<Rendering::Mesh*>(rt),
				parameter[1].to<Util::Color4f>(rt),
				parameter[2].to<Util::Color4f>(rt),
				parameter[3].to<Util::Color4f>(rt),
				parameter[4].to<float>(rt) ),EScript::create(nullptr)))

	//! [ESF] void Rendering.simplifyMesh(Mesh,numberOfPolygons)
	ES_FUNCTION(lib,"simplifyMesh",6,6,{
		Mesh *m=parameter[0].to<Rendering::Mesh*>(rt);

		Rendering::MeshUtils::Simplification::weights_t weights;
		Array * eWeights = EScript::assertType<Array>(rt, parameter[5]);
		if(eWeights->size() != weights.size()) {
			rt.throwException("Array parameter that contains the weights has wrong size.");
		}
		auto eWeightIt = eWeights->begin();
		for(auto & weight : weights) {
			weight = (*eWeightIt)->toFloat();
			++eWeightIt;
		}
		// normalize vertex position weight
		weights[Rendering::MeshUtils::Simplification::VERTEX_OFFSET] /= m->getBoundingBox().getExtentMax();

		m = Rendering::MeshUtils::Simplification::simplifyMesh(m, parameter[1].to<uint32_t>(rt), parameter[2].toFloat(), parameter[3].toBool(), parameter[4].toFloat(), weights);
		if(m!=nullptr)
			return EScript::create(m);
		return EScript::create(nullptr);
	})

	//! [ESF] Mesh Rendering.createMeshByQuadTree(Util.PixelAccessor depthTexture[, Util.PixelAccessor colorTexture[, Util.PixelAccessor normalTexture[, Util.PixelAccessor stencilTexture[,disruption=0.0075f]]]])
	ES_FUNCTION(lib, "createMeshByQuadTree", 1, 5, {
		Util::PixelAccessor * depthTexture = parameter[0].to<Util::PixelAccessor*>(rt);
		Util::PixelAccessor * colorTexture = nullptr;
		if(parameter.count() > 1 && !parameter[1].toType<Void>()) {
			colorTexture = parameter[1].to<Util::PixelAccessor*>(rt);
		}
		Util::PixelAccessor * normalTexture = nullptr;
		if(parameter.count() > 2 && !parameter[2].toType<Void>()) {
			normalTexture = parameter[2].to<Util::PixelAccessor*>(rt);
		}
		Util::PixelAccessor * stencilTexture = nullptr;
		if(parameter.count() > 3 && !parameter[3].toType<Void>()) {
			stencilTexture = parameter[3].to<Util::PixelAccessor*>(rt);
		}

		Rendering::VertexDescription vertexDescription;
		vertexDescription.appendPosition3D();
		if(colorTexture != nullptr) {
			vertexDescription.appendColorRGBAByte();
		}
		if(normalTexture != nullptr) {
			vertexDescription.appendNormalByte();
		}
		vertexDescription.appendTexCoord();

		QuadtreeMeshBuilder::DepthSplitFunction splitFunction(depthTexture, parameter[4].toFloat(0.0075f));
		return EScript::create(QuadtreeMeshBuilder::createMesh(vertexDescription, depthTexture, colorTexture, normalTexture, stencilTexture, splitFunction));
	})

	//! [ESF] Mesh Rendering.createMeshByMarchingCubesFromTiledImage(Util.PixelAccessor values,Number sizeX,Number sizeY,Number sizeZ)
	ES_FUN(lib, "createMeshByMarchingCubesFromTiledImage", 4, 4, EScript::create(
			MarchingCubesMeshBuilder::createMeshFromTiledImage(
					parameter[0].to<Util::PixelAccessor&>(rt),
					parameter[1].to<uint32_t>(rt),parameter[2].to<uint32_t>(rt),parameter[3].to<uint32_t>(rt))))


	//! [EMSF] float Rendering.getLongestSideLength(Mesh)
	ES_FUN(lib,"getLongestSideLength",1,1,(Rendering::MeshUtils::getLongestSideLength(
				parameter[0].to<Rendering::Mesh*>(rt) )))

	//! [ESF] void Rendering.splitLargeTriangles(Mesh,size)
	ES_FUN(lib,"splitLargeTriangles",2,2,(Rendering::MeshUtils::splitLargeTriangles(
				parameter[0].to<Rendering::Mesh*>(rt), parameter[1].toFloat() ),EScript::create(nullptr)))

	//! [ESF] void Rendering.transformMesh(Mesh, Matrix)
	ES_FUN(lib,"transformMesh",2,2,(Rendering::MeshUtils::transform(
				parameter[0].to<Rendering::Mesh*>(rt)->openVertexData(),
				parameter[1].to<const Geometry::Matrix4x4&>(rt) ),EScript::create(nullptr)))

	/*! ***temporary***
		[ESF] void Rendering._transformMeshCoordinates(Mesh, String VertexAttrName, Matrix)    */
	ES_FUNCTION(lib,"_transformMeshCoordinates",3,3,{
		Mesh * m = parameter[0].to<Rendering::Mesh*>(rt);
		MeshVertexData & vd= m->openVertexData();
		Rendering::MeshUtils::transformCoordinates(vd, parameter[1].toString(),
											parameter[2].to<const Geometry::Matrix4x4&>(rt),
											0, m->getVertexCount());
		vd.updateBoundingBox();
		return EScript::create(nullptr);
	})

	//! [ESF] void Rendering.cutMesh(Mesh, Vec3, Vec3, [Array], [Number])
	ES_FUNCTION(lib, "cutMesh", 3, 5, {
		Mesh * mesh = parameter[0].to<Rendering::Mesh*>(rt);
		const Geometry::Vec3f & position = parameter[1].to<Geometry::Vec3>(rt);
		const Geometry::Vec3f & normal = parameter[2].to<Geometry::Vec3>(rt);

		EScript::Array* arr = parameter[3].toType<EScript::Array>();
		if(arr) {
			std::set<uint32_t> uniqueIndices;
			for(auto val : *arr)
				uniqueIndices.insert(val.toUInt());
			Rendering::MeshUtils::cutMesh(mesh, Geometry::Plane(position, normal), uniqueIndices, parameter[4].toFloat(0.00001));
		} else {
			Rendering::MeshUtils::cutMesh(mesh, Geometry::Plane(position, normal), {}, parameter[3].toFloat(0.00001));
		}
		return EScript::create(nullptr);
	})

	//! [ESF] void Rendering.extrudeTriangles(Mesh, Vec3, Array)
	ES_FUNCTION(lib, "extrudeTriangles", 3, 3, {
		Mesh * mesh = parameter[0].to<Rendering::Mesh*>(rt);
		const Geometry::Vec3f & dir = parameter[1].to<Geometry::Vec3>(rt);
		EScript::Array* arr = parameter[2].to<EScript::Array*>(rt);
		std::set<uint32_t> uniqueIndices;
		uint32_t count = mesh->getPrimitiveCount();
		for(auto val : *arr)
			if(val.toUInt() < count)
				uniqueIndices.insert(val.toUInt());
		Rendering::MeshUtils::extrudeTriangles(mesh, dir, uniqueIndices);
		return EScript::create(nullptr);
	})

	//! [ESF] Number Rendering.getFirstTriangleIntersectingRay(Mesh, Ray)
	ES_FUN(lib,"getFirstTriangleIntersectingRay",2,2,(Rendering::MeshUtils::getFirstTriangleIntersectingRay(
				parameter[0].to<Rendering::Mesh*>(rt), parameter[1].to<Geometry::Ray3>(rt) )))


	//! [ESF] void Rendering.eliminateDuplicateVertices(Mesh,[Number])
	ES_FUN(lib,"mergeCloseVertices",1,2,
		Rendering::MeshUtils::mergeCloseVertices(parameter[0].to<Rendering::Mesh*>(rt), parameter[1].toFloat(0.0001))
	)
	

	//! [ESF] [Array|Void] Rendering.splitIntoConnectedComponents(Mesh, [Number])
	ES_FUNCTION(lib, "splitIntoConnectedComponents", 1, 2, {
			auto result = Rendering::MeshUtils::splitIntoConnectedComponents(parameter[0].to<Rendering::Mesh*>(rt), parameter[1].toFloat(0.001));
			Array * a = Array::create();
			for(auto* mesh : result) {
				a->pushBack(EScript::create(mesh));
			}
			return a;
	})

	//! [ESF] void Rendering.applyDisplacementMap(Mesh,PixelAccessor,Number scale,Bool clampToEdge)
	ES_FUN(lib,"applyDisplacementMap",2,4, (
		Rendering::MeshUtils::applyDisplacementMap(parameter[0].to<Rendering::Mesh*>(rt), 
		parameter[1].to<Util::PixelAccessor*>(rt), parameter[2].toFloat(1), parameter[3].toBool(false)), EScript::create(nullptr)))

	//! [ESF] void Rendering.applyNoise(Mesh,Number noiseScale, Matrix4x4 transform,Number seed)
	ES_FUN(lib,"applyNoise",1,4, (
		Rendering::MeshUtils::applyNoise(parameter[0].to<Rendering::Mesh*>(rt), 
		parameter[1].toFloat(1), parameter[2].to(rt, Geometry::Matrix4x4()), parameter[3].toUInt(0)), EScript::create(nullptr)))
	
	//! [ESF] void Rendering.flattenMesh(Mesh,Vec3 pos,Number radius,Number falloff)
	ES_FUN(lib,"flattenMesh",4,4, (
		Rendering::MeshUtils::flattenMesh(parameter[0].to<Rendering::Mesh*>(rt), 
		parameter[1].to<Geometry::Vec3>(rt), parameter[2].toFloat(), parameter[3].toFloat()), EScript::create(nullptr)))
	

	//! [ESF] Number Rendering.computeSurfaceArea(Mesh)
	ES_FUN(lib,"computeSurfaceArea",1,1,(Rendering::MeshUtils::computeSurfaceArea(
				parameter[0].to<Rendering::Mesh*>(rt))))

	//! [ESF] void Rendering.copyVertices(Mesh source, Mesh target, Number sourceOffset, Number targetOffset, Number count)
	ES_FUN(lib,"copyVertices",5,5,(Rendering::MeshUtils::copyVertices(
				parameter[0].to<Rendering::Mesh*>(rt), parameter[1].to<Rendering::Mesh*>(rt), parameter[2].toUInt(), parameter[3].toUInt(), parameter[4].toUInt()), EScript::create(nullptr)))
}
}
