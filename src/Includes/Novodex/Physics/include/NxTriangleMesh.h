#ifndef NX_COLLISION_NXTRIANGLEMESH
#define NX_COLLISION_NXTRIANGLEMESH
/*----------------------------------------------------------------------------*\
|
|						Public Interface to NovodeX Technology
|
|							     www.novodex.com
|
\*----------------------------------------------------------------------------*/

#include "Nxp.h"
#include "NxVec3.h"

class NxTriangleMeshShape;
class NxSimpleTriangleMesh;
class NxTriangleMeshDesc;
class NxTriangleMeshShapeDesc;
class NxPMap;
class NxStream;


/**

A triangle mesh, also called a 'polygon soup'. It is 
represented as an indexed triangle list. There are no restrictions on the
triangle data. 

However, you may use some settings to have the data be treated as a height field.
See NxTriangleMeshDesc for details.

To avoid duplicating data when you have several instances of a particular 
mesh positioned differently, you do not use this class to represent a 
mesh object directly. Instead, you create an instance of this mesh via
the NxTriangleMeshShape class.

To create an instance of this class call NxPhysicsSDK::createTriangleMesh(),
and NxPhysicsSDK::releaseTriangleMesh() to delete it. This is only possible
once you have released all of its NxTriangleMeshShape instances.

*/

class NxTriangleMesh
	{
	public:

#ifdef NX_COOKING
	/**
	Sets the triangle mesh data. Note that the mesh data will be copied, then cleaned.

	The internal data structures will be created when this call is made. If you change the triangle mesh
	data, you need to make this call again to recompute the data structures.

	See the documentation of NxTriangleMeshDesc for specifics about the parameters.
	*/
	virtual	bool				loadFromDesc(const NxTriangleMeshDesc&) = 0;
#endif
	virtual	bool				saveToDesc(NxTriangleMeshDesc&)	const	= 0;

	/**
	Gets the number of internal submeshes for this mesh.
	*/
	virtual NxU32				getSubmeshCount()							const	= 0;

	/**
	For a given submesh, retrieves the number of elements of a given internal array.
	*/
	virtual NxU32				getCount(NxSubmeshIndex, NxInternalArray)	const	= 0;

	/**
	For a given submesh, retrieves the format of a given internal array.
	*/
	virtual NxInternalFormat	getFormat(NxSubmeshIndex, NxInternalArray)	const	= 0;

	/**
	For a given submesh, retrieves the base pointer of a given internal array.
	*/
	virtual const void*			getBase(NxSubmeshIndex, NxInternalArray)	const	= 0;

	/**
	For a given submesh, retrieves the stride value of a given internal array.
	The stride value is always a number of bytes. You have to skip this number of bytes
	to go from one element to the other in an array, starting from the base.
	*/
	virtual NxU32				getStride(NxSubmeshIndex, NxInternalArray)	const	= 0;

	/**
	This call lets you supply a pmap if you have not done so at creation time.
	*/
	virtual	bool				loadPMap(const NxPMap&) = 0;

	/**
	Checks the mesh has a pmap or not.
	*/
	virtual	bool				hasPMap()					const	= 0;

	/**
	Gets the size of the pmap.
	*/
	virtual	NxU32				getPMapSize()				const	= 0;

	/**
	Gets pmap data. You must first query expected size with getPmapSize(), then allocate a buffer large
	enough to contain that amount of bytes, then call this function to dump data in preallocated buffer.
	The system checks that pmap.dataSize is equal to expected data size, so you must initialize that
	member as well before the query.
	*/
	virtual	bool				getPMapData(NxPMap& pmap)	const	= 0;

	/**
	Gets the density of the pmap.
	*/
	virtual	NxU32				getPMapDensity()			const	= 0;

	virtual	bool				load(const NxStream& stream)		= 0;
#ifdef NX_COOKING
	virtual	bool				save(NxStream& stream)		const	= 0;
#endif
	};
#endif
