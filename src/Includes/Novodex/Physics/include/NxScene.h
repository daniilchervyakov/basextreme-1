#ifndef NX_PHYSICS_NX_SCENE
#define NX_PHYSICS_NX_SCENE
/*----------------------------------------------------------------------------*\
|
|						Public Interface to NovodeX Technology
|
|							     www.novodex.com
|
\*----------------------------------------------------------------------------*/

#include "Nxp.h"
#include "NxUserRaycastReport.h"
#include "NxUserEntityReport.h"
#include "NxSceneDesc.h"
#include "NxSceneStats.h"
#include "NxArray.h"

#if NX_USE_FLUID_API
#include "fluids/NxFluid.h"
class NxUserFluidContactReport;
#endif

class NxActor;
class NxActorDescBase;
class NxJoint;
class NxJointDesc;
class NxEffector;
class NxShape;
class NxUserNotify;
class NxUserTriggerReport;
class NxUserContactReport;
class NxSpringAndDamperEffector;
class NxSpringAndDamperEffectorDesc;
class NxRay;
class NxBounds3;
class NxPlane;
class NxSphere;
class NxThread;
class NxTriangle;
class NxPhysicsSDK;
class NxMaterialDesc;
class NxMaterial;

/**
Struct used by NxScene::getPairFlagArray().
The high bit of each 32 bit	flag field denotes whether a pair is a shape or an actor pair.
The flags are defined by the enum NxContactPairFlag.
*/
class NxPairFlag
	{
	public:
	void*	objects[2];
	NxU32	flags;

	NX_INLINE NxU32 isActorPair() const { return flags & 0x80000000;	}
	};


enum NxStandardFences
	{
	NX_FENCE_RUN_FINISHED,
	/*NX_SYNC_RAYCAST_FINISHED,*/
	NX_NUM_STANDARD_FENCES,
	};

/**
 enum to check if a certain part of the SDK has finished.
 used in:
 bool checkResults(NxSimulationStatus, bool block = false)
 bool fetchResults(NxSimulationStatus, bool block = false)
*/
enum NxSimulationStatus
	{
	NX_RIGID_BODY_FINISHED	= (1<<0),
	};

/** 
 Scene interface class. A scene is a collection of bodies, constraints, and
 effectors which can interact. The scene simulates the behavior of these objects
 over time. Several scenes may exist at the same time, but each body, constraint,
 or effector object is specific to a scene -- they may not be shared.

 For example, attempting to create a joint in one scene and then using it to attach
 bodies from a different scene results in undefined behavior.
*/
class NxScene
	{
	protected:
										NxScene()	{}
	virtual								~NxScene()	{}

	public:

	/**
	Sets a constant gravity for the entire scene.
	*/
	virtual void						setGravity(const NxVec3&) = 0;

	/**
	Retrieves the current gravity setting.
	*/
	virtual void						getGravity(NxVec3&) = 0;

	/**
	Creates an actor in this scene. NxActorDesc::isValid() must return true.
	*/
	virtual NxActor*					createActor(const NxActorDescBase&) = 0;

	/**
	Deletes the specified actor. The actor must be in this scene.
	Do not keep a reference to the deleted instance.

	Note: deleting a static actor will not wake up any sleeping objects that were
	sitting on it. Use a kinematic actor instead to get this behavior.
	*/
	virtual void						releaseActor(NxActor&) = 0;

	/**
	Creates a joint. The joint type depends on the type of joint desc passed in.
	*/
	virtual NxJoint *					createJoint(const NxJointDesc &) = 0;

	/**
	Deletes the specified joint. The joint must be in this scene.
	Do not keep a reference to the deleted instance.
	*/
	virtual void						releaseJoint(NxJoint &) = 0;

	/**
	Creates an instance of the returned class.
	*/
	virtual NxSpringAndDamperEffector*	createSpringAndDamperEffector(const NxSpringAndDamperEffectorDesc&) = 0;

	/**
	Deletes the effector passed.
	Do not keep a reference to the deleted instance.
	*/
	virtual void						releaseEffector(NxEffector&) = 0;

	/**
	Adds a new material to the material list, and returns its index.

	The material library consists of an array of material objects. Each
	material has a well defined index that can be used to refer to it.
	If an object (shape or triangle) references an undefined material,
	the default material with index 0 is used instead.
	*/
	virtual NxMaterial *				createMaterial(const NxMaterialDesc &) = 0;

	/**
	Deletes the specified material. The material must be in this scene.
	Don not keep a reference to the deleted instance.
	If you release a material while shapes or meshes are referencing its material index,
	the material assignment of these objects becomes undefined.
	*/
	virtual void						releaseMaterial(NxMaterial &) = 0;

	/**
	Sets the pair flags for the given pair of actors. The pair flags are a combination of bits
	defined by ::NxContactPairFlag. Calling this on an actor that has no shape(s) has no effect.
	The two actor references must not reference the same actor.
	*/
	virtual void						setActorPairFlags(NxActor&, NxActor&, NxU32 nxContactPairFlag) = 0;

	/**
	Retrieves the pair flags for the given pair of actors. The pair flags are a combination of bits
	defined by ::NxContactPairFlag. If no pair record is found, zero is returned.
	The two actor references must not reference the same actor.
	*/
	virtual NxU32						getActorPairFlags(NxActor&, NxActor&) const = 0;

	/**
	similar to setPairFlags(), but for a pair of shapes.
	The two shape references must not reference the same shape.
	*/
	virtual	void						setShapePairFlags(NxShape&, NxShape&, NxU32 nxContactPairFlag) = 0;

	/**
	similar to getPairFlags(), but for a pair of shapes.
	The two shape references must not reference the same shape.
	*/
	virtual	NxU32						getShapePairFlags(NxShape&, NxShape&) const = 0;

	/**
	Returns the number of pairs for which pairFlags are defined. (This includes actor and shape pairs.)
	*/
	virtual NxU32						getNbPairs() const = 0;

	/**
	Outputs the pair data. The high bit of each 32 bit flag field denotes whether a pair is a shape or
	an actor pair. numPairs is the number of pairs the buffer can hold. The user is responsible for
	allocating and deallocating the buffer. Call ::getNbPairs() to check what the number of pairs should be.
	Returns the number of pairs written.
	*/
	virtual NxU32						getPairFlagArray(NxPairFlag* userArray, NxU32 numPairs) const = 0;

	/**
	Returns the number of actors.
	*/
	virtual	NxU32						getNbActors()		const	= 0;

	/**
	Returns an array of actor pointers with size getNbActors().
	*/
	virtual	NxActor**					getActors()					= 0;

	/**
	Returns the number of static shapes.
	*/
	virtual	NxU32						getNbStaticShapes()		const	= 0;

	/**
	Returns the number of dynamic shapes.
	*/
	virtual	NxU32						getNbDynamicShapes()	const	= 0;

	/**
	Returns the number of joints in this scene.  This includes all joints, even broken ones.
	*/
	virtual NxU32						getNbJoints()		const	= 0;

	/**
	Restarts the joint iterator so that the next call to ::getNextJoint() returns the first joint in the scene.
	Creating or deleting joints resets the joint iterator.
	*/
	virtual void						resetJointIterator()	 = 0;

	/**
	Retrieves the next joint.  First call resetJointIterator(), then call this method repeatedly until it returns
	zero.  After a call to resetJointIterator(), repeated calls to getNextJoint() should return a sequence of getNbJoints()
	joint pointers.  The getNbJoints()+1th call will return 0.
	Creating or deleting joints resets the joint iterator.
	*/
	virtual NxJoint *					getNextJoint()	 = 0;

	/**
	Returns the number of effectors in this scene.
	*/
	virtual NxU32						getNbEffectors()		const	= 0;

	/**
	Restarts the effector iterator so that the next call to ::getNextEffector() returns the first effector in the scene.
	Creating or deleting effectors resets the joint iterator.
	*/
	virtual void						resetEffectorIterator()	 = 0;

	/**
	Retrieves the next effector.  First call resetEffectorIterator(), then call this method repeatedly until it returns
	zero.  After a call to resetEffectorIterator(), repeated calls to getNextEffector() should return a sequence of getNbEffectors()
	effector pointers.  The getNbEffectors()+1th call will return 0.
	Creating or deleting effectors resets the joint iterator.
	*/
	virtual NxEffector *				getNextEffector() = 0;

	/**
	Returns the size of the internal material array.
	Note that the returned value is not related to material indices (NxMaterialIndex).
	Those may not be allocated continuously, and its values may be higher than getNbMaterials().
	This will also include the default material which exists without having to be created.
	*/
	virtual NxU32						getNbMaterials() const = 0;

	/**
	Writes the scene's array of material pointers to a user buffer.  bufferSize is the number of pointers (not bytes) that the buffer can hold.
	usersIterator is an iterator that the user should initialize to 0 to start copying the array from the beginning.
	Once the first call succeeds, the SDK will have changed the value of the iterator (in some data structure specific way) such that the
	next get*Array() call will return the next batch of values.  This way a large internal array can be read out with several calls into 
	a smaller user side buffer.
	
	Returns the number of pointers written, this should be less or equal to bufferSize.  This will also return the default material which exists without having to be created.

	The ordering of the materials in the array is not specified.

	Usage example:
	NxMaterial * ptrs[3];
	NxU32 iterator = 0;
	while (3 == s->getMaterialArray(ptrs, 3, iterator))
		process3Materials(ptrs);
	*/
	virtual	NxU32						getMaterialArray(NxMaterial ** userBuffer, NxU32 bufferSize, NxU32 & usersIterator) = 0;

	/**
	Returns current highest valid material index.  Note that not all indices below this are valid if some of them belong to meshes that
	have been freed.
	*/
	virtual NxMaterialIndex				getHighestMaterialIndex() const = 0;

	/**
	retrieves the material with the given material index.  If the material index is out of range or
	belongs to a material that was released, 0 is returned, but no error is reported.
	You can get a pointer to the default material (the material that all invalid material indices are mapped to) by using index 0.
	You can then change this default material.  Releasing the default material has no effect.
	*/
	virtual	NxMaterial *				getMaterialFromIndex(NxMaterialIndex) = 0;

	/**
	Flush the scene's command queue for processing.
	Flushes any eventually buffered commands so that they get executed.
	Ensures that commands buffered in the system will continue to make forward progress until completion.
	*/
	virtual void						flushStream() = 0;

	/**
 	Sets simulation timing parameters used in simulate(elapsedTime).

	If method is NX_TIMESTEP_FIXED, elapsedTime is internally subdivided into up to
	maxIter substeps no larger than maxTimestep. The timestep method of
	TIMESTEP_FIXED is strongly preferred for stable, reproducible simulation.

	Alternatively NX_TIMESTEP_VARIABLE can be used, in this case the first two parameters
	are not used.	See also ::NxTimeStepMethod.
	*/
	virtual void						setTiming(NxReal maxTimestep=1.0f/60.0f, NxU32 maxIter=8, NxTimeStepMethod method=NX_TIMESTEP_FIXED) = 0;

	/**
	Retrieves simulation timing parameters.
	*/
	virtual void						getTiming(NxReal& maxTimestep, NxU32& maxIter, NxTimeStepMethod& method, NxU32* numSubSteps=NULL) const = 0;

	/**
	Call this if you want this scene to generate visualization data. You can have this data get rendered into a buffer
	using PhysicsSDK::visualize(buffer).
	*/
	virtual	void						visualize() = 0;

	/**
	Call this method to retrieve the Physics SDK.
	*/
	virtual	NxPhysicsSDK&				getPhysicsSDK() = 0;

	/**
	Call this method to retrieve statistics about the current scene.
	*/
	virtual	void						getStats(NxSceneStats& stats) const = 0;

	/**
	Call to retrieve the expected object count limits set in the scene descriptor.
	*/
	virtual	void						getLimits(NxSceneLimits& limits) const = 0;

	/**
	Sets a user notify object which receives special simulation events when they occur.
	*/
	virtual void						setUserNotify(NxUserNotify* callback) = 0;

	/**
	Retrieves the userNotify pointer set with setUserNotify().
	*/
	virtual NxUserNotify*				getUserNotify() const = 0;

	/**
	Sets a trigger report object which receives collision trigger events.
	*/
	virtual	void						setUserTriggerReport(NxUserTriggerReport* callback) = 0;

	/**
	Retrieves the callback pointer set with setUserTriggerReport().
	*/
	virtual	NxUserTriggerReport*		getUserTriggerReport() const = 0;

	/**
	Sets a contact report object which receives collision contact events.
	*/
	virtual	void						setUserContactReport(NxUserContactReport* callback) = 0;

	/**
	Retrieves the callback pointer set with setUserContactReport().
	*/
	virtual	NxUserContactReport*		getUserContactReport() const = 0;

#if NX_USE_FLUID_API

	/**
	Not available in the current release.
	The fluid equivalent for registering a specialized version of the user contact report.
	*/
	virtual	void						setUserFluidContactReport(NxUserFluidContactReport* callback) = 0;
	
	/**
	Not available in the current release.
	Retrieves the callback pointer set with setUserFluidContactReport().
	*/
	virtual	NxUserFluidContactReport*	getUserFluidContactReport() const = 0;

#endif

	//! Raycasting

	/**
	Returns true if any axis aligned bounding box enclosing a shape of type shapeType is intersected by the ray.

	Note: Make certain that the direction vector of NxRay is normalized.
	*/
	virtual bool						raycastAnyBounds		(const NxRay& worldRay, NxShapesType shapesType, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, const NxGroupsMask* groupsMask=NULL) const = 0;

	/**
	Returns true if any shape of type ShapeType is intersected by the ray.

	Note: Make certain that the direction vector of NxRay is normalized.
	*/
	virtual bool						raycastAnyShape			(const NxRay& worldRay, NxShapesType shapesType, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, const NxGroupsMask* groupsMask=NULL) const = 0;

	/**
	Calls the report's hitCallback() method for all the axis aligned bounding boxes enclosing shapes
	of type ShapeType intersected by the ray. The point of impact is provided as a parameter to hitCallback().
	hintFlags is a combination of ::NxRaycastBit flags.
	Returns the number of shapes hit.

	Note: Make certain that the direction vector of NxRay is normalized.
	*/
	virtual NxU32						raycastAllBounds		(const NxRay& worldRay, NxUserRaycastReport& report, NxShapesType shapesType, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, NxU32 hintFlags=0xffffffff, const NxGroupsMask* groupsMask=NULL) const = 0;

	/**
	Calls the report's hitCallback() method for all the shapes of type ShapeType intersected by the ray.
	hintFlags is a combination of ::NxRaycastBit flags.
	Returns the number of shapes hit. The point of impact is provided as a parameter to hitCallback().

	Note: Make certain that the direction vector of NxRay is normalized.
	*/
	virtual NxU32						raycastAllShapes		(const NxRay& worldRay, NxUserRaycastReport& report, NxShapesType shapesType, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, NxU32 hintFlags=0xffffffff, const NxGroupsMask* groupsMask=NULL) const = 0;

	/**
	Returns the first axis aligned bounding box enclosing a shape of type shapeType that is hit along the ray. The world space intersectin point,
	and the distance along the ray are also provided.
	hintFlags is a combination of ::NxRaycastBit flags.

	Note: Make certain that the direction vector of NxRay is normalized.
	*/
	virtual NxShape*					raycastClosestBounds	(const NxRay& worldRay, NxShapesType shapeType, NxRaycastHit& hit, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, NxU32 hintFlags=0xffffffff, const NxGroupsMask* groupsMask=NULL) const = 0;

	/**
	Returns the first shape of type shapeType that is hit along the ray. The world space intersectin point,
	and the distance along the ray are also provided.
	hintFlags is a combination of ::NxRaycastBit flags.

	Note: Make certain that the direction vector of NxRay is normalized.
	*/
	virtual NxShape*					raycastClosestShape		(const NxRay& worldRay, NxShapesType shapeType, NxRaycastHit& hit, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, NxU32 hintFlags=0xffffffff, const NxGroupsMask* groupsMask=NULL) const = 0;

	/**
	Returns the set of shapes overlapped by the world-space sphere. You can test against static and/or dynamic objects
	by adjusting 'shapeType'. Shapes are written to the static array 'shapes', which should be big enough to hold 'nbShapes'.
	An alternative is to use the ::NxUserEntityReport callback mechanism.

	The function returns the total number of collided shapes.

	NOTE: this functions only tests against the objects' AABBs!
	*/
	virtual	NxU32						overlapSphereShapes		(const NxSphere& worldSphere, NxShapesType shapeType, NxU32 nbShapes, NxShape** shapes, NxUserEntityReport<NxShape*>* callback, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL) = 0;

	/**
	Returns the set of shapes overlapped by the world-space AABB. You can test against static and/or dynamic objects
	by adjusting 'shapeType'. Shapes are written to the static array 'shapes', which should be big enough to hold 'nbShapes'.
	An alternative is to use the ::NxUserEntityReport callback mechanism.

	The function returns the total number of collided shapes.

	NOTE: this functions only tests against the objects' AABBs!
	*/
	virtual	NxU32						overlapAABBShapes		(const NxBounds3& worldBounds, NxShapesType shapeType, NxU32 nbShapes, NxShape** shapes, NxUserEntityReport<NxShape*>* callback, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL) = 0;

	/**
	Returns the set of shapes overlapped by the world-space planes. You can test against static and/or dynamic objects
	by adjusting 'shapeType'. Shapes are written to the static array 'shapes', which should be big enough to hold 'nbShapes'.
	An alternative is to use the ::NxUserEntityReport callback mechanism.

	The function returns the total number of collided shapes.

	This can be used for view-frustum culling by passing the 6 camera planes to the function.

	NOTE: this functions only tests against the objects' AABBs!
	*/
	virtual	NxU32						cullShapes				(NxU32 nbPlanes, const NxPlane* worldPlanes, NxShapesType shapeType, NxU32 nbShapes, NxShape** shapes, NxUserEntityReport<NxShape*>* callback, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL) = 0;

	/**
	Checks whether a world-space sphere overlaps something or not.
	*/
	virtual bool						checkOverlapSphere		(const NxSphere& worldSphere, NxShapesType shapeType=NX_ALL_SHAPES, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL)	= 0;

	/**
	Checks whether a world-space AABB overlaps something or not.
	*/
	virtual bool						checkOverlapAABB		(const NxBounds3& worldBounds, NxShapesType shapeType=NX_ALL_SHAPES, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL)	= 0;

	//virtual	NxU32						overlapAABBTriangles	(const NxBounds3& worldBounds, NxArraySDK<NxTriangle>& worldTriangles) = 0;

#if NX_USE_FLUID_API
	
	/**
	Not available in the current release.
	Creates a fluid in this scene. NxFluidDesc::isValid() must return true.
	*/
	virtual NxFluid*					createFluid(const NxFluidDesc&) = 0;

	/**
	Not available in the current release.
	Deletes the specified fluid. The fluid must be in this scene.
	Do not keep a reference to the deleted instance.
	*/
	virtual void						releaseFluid(NxFluid&)			= 0;

	/**
	Not available in the current release.
	Returns the number of fluids.
	*/
	virtual	NxU32						getNbFluids()		const		= 0;

	/**
	Not available in the current release.
	Returns an array of fluid pointers with size getNbFluids().
	*/
	virtual	NxFluid**					getFluids()						= 0;

	/**
	Not available in the current release.
	Creates an implicit mesh in this scene. NxImplicitMeshDesc::isValid() must return true.
	*/
	virtual NxImplicitMesh*				createImplicitMesh(const NxImplicitMeshDesc&)	= 0;

	/**
	Not available in the current release.
	Deletes the specified implicit mesh. The implicit mesh must be in this scene.
	Do not keep a reference to the deleted instance.
	*/
	virtual void						releaseImplicitMesh(NxImplicitMesh&)			= 0;

	/**
	Not available in the current release.
	Returns the number of implicit meshes.
	*/
	virtual	NxU32						getNbImplicitMeshes() const						= 0;

	/**
	Not available in the current release.
	Returns an array of implicit mesh pointers with size getNbImplicitMeshes().
	*/
	virtual	NxImplicitMesh**			getImplicitMeshes()								= 0;

#endif

	/**
	This is a query to see if the scene is in a state that allows the application to 
	update scene state.
	*/
	virtual	bool						isWritable()	= 0;

	/**
 	Advances the simulation by an elapsedTime time. If elapsedTime is large, it is internally
 	subdivided according to parameters provided with the setTiming() method.
 
 	Calls to simulate() should pair with calls to fetchResults():
 	Each fetchResults() invocation corresponds to exactly one simulate()
 	invocation; calling fetchResults() twice without an intervening fetchResults()
 	or fetchResults() twice without an intervening simulate() causes an error
 	condition.
 
 	scene->simulate();
 	...do some processing until physics is computed...
 	scene->fetchResults();
 	...now results of run may be retrieved.
 
 	Applications should not modify physics objects between calls to
 	simulate() and fetchResults();
 
  	This method replaces startRun().
	*/
	virtual	void						simulate(NxReal elapsedTime)		= 0;
	
	/**
	This method replaces wait()
	This checks to see if the part of the simulation run whose results you are interested in has completed.
	This does not cause the data available for reading to be updated with the results of the simulation, it is simply a status check.
	The bool will allow it to either return immediately or block waiting for the condition to be met so that it can return true
	*/
	virtual	bool						checkResults(NxSimulationStatus, bool block = false)	= 0;

	/**
	This is the big brother to checkResults() it basically does the following:
	if ( checkResults(enum, block) )
	{
		fire appropriate callbacks
		swap buffers
		if (CheckResults(all_enums, false))
			make IsWritable() true
		return true
	}
	else
		return false
	*/
	virtual	bool						fetchResults(NxSimulationStatus, bool block = false)	= 0;

	void*	userData;	//!< user can assign this to whatever, usually to create a 1:1 relationship with a user object.
	};

#endif
