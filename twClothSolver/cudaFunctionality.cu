

//include
//-----------------------------------------------
//cuda
#include "helper_math.h"

//own
#include "cudaFunctionality.h"


//kernels
//-----------------------------------------------

namespace kernels
{
	//add_on_gpu
	__global__ void add_on_gpu(float a, float b, float* p_result)
	{
		//result
		*p_result = a + b;
	}
	
	//verletIntegration
	__global__ void verletIntegration(staticSolverData_t staticSolverData, dynamicSolverData_t dynamicSolverData)
	{
		//thread_index
		int thread_index = blockIdx.x * blockDim.x + threadIdx.x;
		
		//execute if thread_index < vertexCount
		if(thread_index < staticSolverData.vertexCount)
		{
			
			//vecNew
			float3 vecNew = make_float3(0,0,0); //initialize to zero

			//vecPos
			float3 vecPos = make_float3(staticSolverData.pVertexPositionList[thread_index].x,
										staticSolverData.pVertexPositionList[thread_index].y,
										staticSolverData.pVertexPositionList[thread_index].z
										);

			//vecOldpos
			float3 vecOldpos = make_float3(staticSolverData.pVertexOldpositionList[thread_index].x,
										staticSolverData.pVertexOldpositionList[thread_index].y,
										staticSolverData.pVertexOldpositionList[thread_index].z
										);

			//vecForce
			float3 vecForce = make_float3(dynamicSolverData.pVertexForceList[thread_index].x,
										dynamicSolverData.pVertexForceList[thread_index].y,
										dynamicSolverData.pVertexForceList[thread_index].z
										);

			//drag
			float drag = 1.0 - dynamicSolverData.drag;

			//integration
			vecNew = (vecPos + ((vecPos - vecOldpos) * drag)) + vecForce;


			//update solverData

			//pVertexOldpositionList
			staticSolverData.pVertexOldpositionList[thread_index].x = vecPos.x;
			staticSolverData.pVertexOldpositionList[thread_index].y = vecPos.y;
			staticSolverData.pVertexOldpositionList[thread_index].z = vecPos.z;

			//pVertexOldpositionList
			staticSolverData.pVertexPositionList[thread_index].x = vecNew.x;
			staticSolverData.pVertexPositionList[thread_index].y = vecNew.y;
			staticSolverData.pVertexPositionList[thread_index].z = vecNew.z;

		}
	}

	//collisionConstraintGroundplane
	__global__ void collisionConstraintGroundplane(staticSolverData_t staticSolverData, dynamicSolverData_t dynamicSolverData)
	{
		//thread_index
		int thread_index = blockIdx.x * blockDim.x + threadIdx.x;
		
		//execute if thread_index < vertexCount
		if(thread_index < staticSolverData.vertexCount)
		{
			//if point pos y is < than groundplane height set to groundplane height
			if(staticSolverData.pVertexPositionList[thread_index].y < dynamicSolverData.groundplaneHeight)
				staticSolverData.pVertexPositionList[thread_index].y = dynamicSolverData.groundplaneHeight;
		}
	}
	
	//collisionConstraintSpheres
	__global__ void collisionConstraintSpheres(staticSolverData_t staticSolverData, dynamicSolverData_t dynamicSolverData)
	{
		//thread_index
		int thread_index = blockIdx.x * blockDim.x + threadIdx.x;
		
		//execute if thread_index < vertexCount
		if(thread_index < staticSolverData.vertexCount)
		{
			for(int collisionSphereIndex = 0; collisionSphereIndex < dynamicSolverData.collisionConstraintSpheresCount * 2; collisionSphereIndex+=2 )
			{
				
				//New
				//-----------------------------------------------

				//vecDownToUp and radiusSphere

				//vecUp
				float3 vecUp = make_float3(dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList[collisionSphereIndex].x,
											dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList[collisionSphereIndex].y,
											dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList[collisionSphereIndex].z
											);

				//vecDown
				float3 vecDown = make_float3(dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList[collisionSphereIndex+1].x,
											dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList[collisionSphereIndex+1].y,
											dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList[collisionSphereIndex+1].z
											);

				//vecDownToUp
				float3 vecDownToUp = make_float3(0,0,0);
				vecDownToUp = vecUp - vecDown;

				//vecSphereCenter
				float3 vecSphereCenter = make_float3(0,0,0);
				vecSphereCenter = vecDown + (vecDownToUp * 0.5);

				//radiusSphere
				float radiusSphere = length(vecDownToUp * 0.5);

				
				
				
				//perform collision projection

				//vecCurrentPoint
				float3 vecCurrentPoint = make_float3(staticSolverData.pVertexPositionList[thread_index].x,
													staticSolverData.pVertexPositionList[thread_index].y,
													staticSolverData.pVertexPositionList[thread_index].z
													);

				//vecSphereCenterToCurrentPoint
				float3 vecSphereCenterToCurrentPoint = vecCurrentPoint - vecSphereCenter;				
				
				
				//vecSphereCenterToCurrentPointLength
				float vecSphereCenterToCurrentPointLength = length(vecSphereCenterToCurrentPoint);
				
				//check if length < radius
				if(vecSphereCenterToCurrentPointLength < radiusSphere)
				{
					//currentPointProjected
					float3 currentPointProjected = make_float3(0,0,0);
					currentPointProjected = vecSphereCenter + ((vecSphereCenterToCurrentPoint / vecSphereCenterToCurrentPointLength) * radiusSphere);

					//set to solverData
					staticSolverData.pVertexPositionList[thread_index].x = currentPointProjected.x;
					staticSolverData.pVertexPositionList[thread_index].y = currentPointProjected.y;
					staticSolverData.pVertexPositionList[thread_index].z = currentPointProjected.z;

				};

			}
		}
	}
	
	//stickConstraint
	__global__ void stickConstraint(staticSolverData_t staticSolverData, dynamicSolverData_t dynamicSolverData)
	{
		//New

		//iterate edges
		for(int index = 0; index < staticSolverData.edgeCount; index++)
		{
			//restLength
			float restLength = staticSolverData.pEdgeRestlengthList[index];
			//vertexIndices
			int vertexIndex1 = int(staticSolverData.pEdgeVertexIndexList[index].x);
			int vertexIndex2 = int(staticSolverData.pEdgeVertexIndexList[index].y);
			
			//vec1
			float3 vec1 = make_float3(staticSolverData.pVertexPositionList[vertexIndex1].x,
										staticSolverData.pVertexPositionList[vertexIndex1].y,
										staticSolverData.pVertexPositionList[vertexIndex1].z
										);
			
			//vec2
			float3 vec2 = make_float3(staticSolverData.pVertexPositionList[vertexIndex2].x,
										staticSolverData.pVertexPositionList[vertexIndex2].y,
										staticSolverData.pVertexPositionList[vertexIndex2].z
										);

			//Compute new vector positions
			float3 delta = make_float3(0,0,0);
			delta = vec2 - vec1;
			float deltaLength = length(delta);
			float difference = (deltaLength - restLength) / deltaLength;
			vec1 = vec1 + delta * 0.5 * difference;
			vec2 = vec2 - delta * 0.5 * difference;

			//set vertexPositionList
			staticSolverData.pVertexPositionList[vertexIndex1].x = vec1.x;
			staticSolverData.pVertexPositionList[vertexIndex1].y = vec1.y;
			staticSolverData.pVertexPositionList[vertexIndex1].z = vec1.z;
			
			staticSolverData.pVertexPositionList[vertexIndex2].x = vec2.x;
			staticSolverData.pVertexPositionList[vertexIndex2].y = vec2.y;
			staticSolverData.pVertexPositionList[vertexIndex2].z = vec2.z;
		};
	};

	//positionConstraints
	__global__ void positionConstraints(staticSolverData_t staticSolverData, dynamicSolverData_t dynamicSolverData)
	{
		//check if posCons 
		if(dynamicSolverData.positionConstraintCount)
		{
			//iterate posCons
			for(int index = 0; index < dynamicSolverData.positionConstraintCount; index++)
			{
				//check if posCon active
				if(dynamicSolverData.pPositionConstraintActiveList[index])
				{
					//check if vertexIndex in vertexCount
					if(dynamicSolverData.pPositionConstraintVertexIndexList[index] < staticSolverData.vertexCount)
					{
						//set vertexPositionList
						staticSolverData.pVertexPositionList[dynamicSolverData.pPositionConstraintVertexIndexList[index]] = dynamicSolverData.pPositionConstraintCoordinateList[index];
					}
				}
			}
		}
	};
}



//definitions
//-----------------------------------------------

//cudaAvailable
int cuda::cudaAvailable()
{
	int deviceCount = 0;
	cudaError_t cudaErrorId;
	cudaErrorId = cudaGetDeviceCount(&deviceCount);
	return deviceCount;
};

//get_grid_size
int cuda::get_grid_size(const staticSolverData_t& staticSolverData, 
						const dynamicSolverData_t& dynamicSolverData, 
						grid_size_t grid_type = VERTEXCOUNT)
{
	//declare grid_count
	int grid_count;

	//thread_count
	int thread_count = get_thread_count(dynamicSolverData);
	
	//VERTEXCOUNT
	if(grid_type == VERTEXCOUNT)
	{
		//grid_count
		grid_count = (staticSolverData.vertexCount + (thread_count - 1)) / thread_count;
	}
	//EDGECOUNT
	else if(grid_type == EDGECOUNT)
	{
		//grid_count
		grid_count = (staticSolverData.edgeCount + (thread_count - 1)) / thread_count;
	};

	return grid_count;
	
};

//get_thread_count
int cuda::get_thread_count(const dynamicSolverData_t& dynamicSolverData)
{
	return dynamicSolverData.thread_count;
};

//add_on_gpu
float cuda::add_on_gpu(float a, float b)
{
	//size
	size_t size = sizeof(float);

	//d_p_result
	float* d_p_result;
	cudaMalloc(&d_p_result, size);

	//h_p_result
	float* h_p_result = new float[1];
	
	//Call kernel
	dim3 num_blocks(1);
	dim3 threads_per_block(1);
	kernels::add_on_gpu<<<num_blocks, threads_per_block>>>(a, b, d_p_result);

	//copy
	cudaMemcpy(h_p_result, d_p_result, size, cudaMemcpyDeviceToHost);

	//free memory
	cudaFree(d_p_result);
	
	//return
	return *h_p_result;

};


//set_static_solver_data_gpu
void cuda::set_static_solver_data_gpu(staticSolverData_t &staticSolverDataGPU, const staticSolverData_t &staticSolverData)
{
	
	//free mem if not null pointers
	//-----------------------------------------------
	//pVertexPositionList
	if(staticSolverDataGPU.pVertexPositionList)
		cudaFree(staticSolverDataGPU.pVertexPositionList);
	//pVertexOldpositionList
	if(staticSolverDataGPU.pVertexOldpositionList)
		cudaFree(staticSolverDataGPU.pVertexOldpositionList);
	//pVertexInitialpositionList
	if(staticSolverDataGPU.pVertexInitialpositionList)
		cudaFree(staticSolverDataGPU.pVertexInitialpositionList);
	//pEdgeVertexIndexList
	if(staticSolverDataGPU.pEdgeVertexIndexList)
		cudaFree(staticSolverDataGPU.pEdgeVertexIndexList);
	//pEdgeRestlengthList
	if(staticSolverDataGPU.pEdgeRestlengthList)
		cudaFree(staticSolverDataGPU.pEdgeRestlengthList);

	
	
	
	//reallocate mem for pointers and copy data
	//-----------------------------------------------
	
	//size_vertex_vcs_vector
	size_t size_vertex_vcs_vector = staticSolverData.vertexCount * sizeof(vcsVector);
	//size_edge_vcs_vector
	size_t size_edge_vcs_vector = staticSolverData.edgeCount * sizeof(vcsVector);
	//size_float
	size_t size_float = staticSolverData.edgeCount * sizeof(float);

	//allocate
	cudaMalloc(&staticSolverDataGPU.pVertexPositionList, size_vertex_vcs_vector);
	cudaMalloc(&staticSolverDataGPU.pVertexOldpositionList, size_vertex_vcs_vector);
	cudaMalloc(&staticSolverDataGPU.pVertexInitialpositionList, size_vertex_vcs_vector);
	cudaMalloc(&staticSolverDataGPU.pEdgeVertexIndexList, size_edge_vcs_vector);
	cudaMalloc(&staticSolverDataGPU.pEdgeRestlengthList, size_float);

	//copy
	//pVertexPositionList
	cudaMemcpy(staticSolverDataGPU.pVertexPositionList, 
				staticSolverData.pVertexPositionList, 
				size_vertex_vcs_vector, 
				cudaMemcpyHostToDevice);
	//pVertexOldpositionList
	cudaMemcpy(staticSolverDataGPU.pVertexOldpositionList, 
				staticSolverData.pVertexOldpositionList, 
				size_vertex_vcs_vector, 
				cudaMemcpyHostToDevice);
	//pVertexInitialpositionList
	cudaMemcpy(staticSolverDataGPU.pVertexInitialpositionList, 
				staticSolverData.pVertexInitialpositionList, 
				size_vertex_vcs_vector, 
				cudaMemcpyHostToDevice);
	//pEdgeVertexIndexList
	cudaMemcpy(staticSolverDataGPU.pEdgeVertexIndexList, 
				staticSolverData.pEdgeVertexIndexList, 
				size_edge_vcs_vector, 
				cudaMemcpyHostToDevice);
	//pEdgeRestlengthList
	cudaMemcpy(staticSolverDataGPU.pEdgeRestlengthList, 
				staticSolverData.pEdgeRestlengthList, 
				size_float, 
				cudaMemcpyHostToDevice);


	//set non pointer values
	//-----------------------------------------------
	staticSolverDataGPU.edgeCount = staticSolverData.edgeCount;
	staticSolverDataGPU.vertexCount = staticSolverData.vertexCount;

	
	//tmp
	//std::cout <<"Allocated static data on the device" <<std::endl;
};


//set_dynamic_solver_data_gpu
void cuda::set_dynamic_solver_data_gpu(dynamicSolverData_t &dynamicSolverDataGPU, 
										const dynamicSolverData_t &dynamicSolverData, 
										const staticSolverData_t &staticSolverData)
{
	
	
	//free mem if not null pointers
	//-----------------------------------------------
	//pVertexForceList
	if(dynamicSolverDataGPU.pVertexForceList)
		cudaFree(dynamicSolverDataGPU.pVertexForceList);
	//pPositionConstraintActiveList
	if(dynamicSolverDataGPU.pPositionConstraintActiveList)
		cudaFree(dynamicSolverDataGPU.pPositionConstraintActiveList);
	//pPositionConstraintVertexIndexList
	if(dynamicSolverDataGPU.pPositionConstraintVertexIndexList)
		cudaFree(dynamicSolverDataGPU.pPositionConstraintVertexIndexList);
	//pPositionConstraintCoordinateList
	if(dynamicSolverDataGPU.pPositionConstraintCoordinateList)
		cudaFree(dynamicSolverDataGPU.pPositionConstraintCoordinateList);
	//pCollisionConstraintSpheresVecUpVecDownList
	if(dynamicSolverDataGPU.pCollisionConstraintSpheresVecUpVecDownList)
		cudaFree(dynamicSolverDataGPU.pCollisionConstraintSpheresVecUpVecDownList);
	//pCollisionConstraintConvexTriangleCountList
	if(dynamicSolverDataGPU.pCollisionConstraintConvexTriangleCountList)
		cudaFree(dynamicSolverDataGPU.pCollisionConstraintConvexTriangleCountList);
	//pCollisionConstraintConvexTriangleVertexPositionList
	if(dynamicSolverDataGPU.pCollisionConstraintConvexTriangleVertexPositionList)
		cudaFree(dynamicSolverDataGPU.pCollisionConstraintConvexTriangleVertexPositionList);
	

	
	
	
	//reallocate mem for pointers and copy data
	//-----------------------------------------------
	
	//size_vertex_vcs_vector
	size_t size_vertex_vcs_vector = staticSolverData.vertexCount * sizeof(vcsVector);
	//size_position_constraint_int
	size_t size_position_constraint_int = dynamicSolverData.positionConstraintCount * sizeof(int);
	//size_position_constraint_vcs_vector
	size_t size_position_constraint_vcs_vector = dynamicSolverData.positionConstraintCount * sizeof(vcsVector);
	//size_collision_constraint_spheres_vcs_vector
	size_t size_collision_constraint_spheres_vcs_vector = dynamicSolverData.collisionConstraintSpheresCount * 2 * sizeof(vcsVector);
	//size_collision_constraint_convex_int
	size_t size_collision_constraint_convex_int = dynamicSolverData.collisionConstraintConvexCount * sizeof(int);
	//size_collision_constraint_convex_vcs_vector
	size_t size_collision_constraint_convex_vcs_vector = dynamicSolverData.collisionConstraintConvexTriangleVertexCount * sizeof(vcsVector);
	

	//allocate
	cudaMalloc(&dynamicSolverDataGPU.pVertexForceList, size_vertex_vcs_vector);
	
	cudaMalloc(&dynamicSolverDataGPU.pPositionConstraintActiveList, size_position_constraint_int);
	cudaMalloc(&dynamicSolverDataGPU.pPositionConstraintVertexIndexList, size_position_constraint_int);
	cudaMalloc(&dynamicSolverDataGPU.pPositionConstraintCoordinateList, size_position_constraint_vcs_vector);

	cudaMalloc(&dynamicSolverDataGPU.pCollisionConstraintSpheresVecUpVecDownList, size_collision_constraint_spheres_vcs_vector);

	cudaMalloc(&dynamicSolverDataGPU.pCollisionConstraintConvexTriangleCountList, size_collision_constraint_convex_int);
	cudaMalloc(&dynamicSolverDataGPU.pCollisionConstraintConvexTriangleVertexPositionList, size_collision_constraint_convex_vcs_vector);

	

	//copy
	//pVertexForceList
	cudaMemcpy(dynamicSolverDataGPU.pVertexForceList, 
				dynamicSolverData.pVertexForceList, 
				size_vertex_vcs_vector, 
				cudaMemcpyHostToDevice);
	
	//pPositionConstraintActiveList
	cudaMemcpy(dynamicSolverDataGPU.pPositionConstraintActiveList, 
				dynamicSolverData.pPositionConstraintActiveList, 
				size_position_constraint_int, 
				cudaMemcpyHostToDevice);
	//pPositionConstraintVertexIndexList
	cudaMemcpy(dynamicSolverDataGPU.pPositionConstraintVertexIndexList, 
				dynamicSolverData.pPositionConstraintVertexIndexList, 
				size_position_constraint_int, 
				cudaMemcpyHostToDevice);
	//pPositionConstraintCoordinateList
	cudaMemcpy(dynamicSolverDataGPU.pPositionConstraintCoordinateList, 
				dynamicSolverData.pPositionConstraintCoordinateList, 
				size_position_constraint_vcs_vector, 
				cudaMemcpyHostToDevice);

	//pCollisionConstraintSpheresVecUpVecDownList
	cudaMemcpy(dynamicSolverDataGPU.pCollisionConstraintSpheresVecUpVecDownList, 
				dynamicSolverData.pCollisionConstraintSpheresVecUpVecDownList, 
				size_collision_constraint_spheres_vcs_vector, 
				cudaMemcpyHostToDevice);

	//pCollisionConstraintConvexTriangleCountList
	cudaMemcpy(dynamicSolverDataGPU.pCollisionConstraintConvexTriangleCountList, 
				dynamicSolverData.pCollisionConstraintConvexTriangleCountList, 
				size_collision_constraint_convex_int, 
				cudaMemcpyHostToDevice);

	//pCollisionConstraintConvexTriangleVertexPositionList
	cudaMemcpy(dynamicSolverDataGPU.pCollisionConstraintConvexTriangleVertexPositionList, 
				dynamicSolverData.pCollisionConstraintConvexTriangleVertexPositionList, 
				size_collision_constraint_convex_vcs_vector, 
				cudaMemcpyHostToDevice);
	

	
	//set non pointer values
	//-----------------------------------------------
	dynamicSolverDataGPU.drag = dynamicSolverData.drag;
	dynamicSolverDataGPU.collisionGroundplaneActive = dynamicSolverData.collisionGroundplaneActive;
	dynamicSolverDataGPU.groundplaneHeight = dynamicSolverData.groundplaneHeight;
	dynamicSolverDataGPU.repetitions = dynamicSolverData.repetitions;
	dynamicSolverDataGPU.positionConstraintCount = dynamicSolverData.positionConstraintCount;
	dynamicSolverDataGPU.collisionConstraintSpheresCount = dynamicSolverData.collisionConstraintSpheresCount;
	dynamicSolverDataGPU.collisionConstraintConvexCount = dynamicSolverData.collisionConstraintConvexCount;
	dynamicSolverDataGPU.collisionConstraintConvexTriangleVertexCount = dynamicSolverData.collisionConstraintConvexTriangleVertexCount;
	dynamicSolverDataGPU.collisionConstraintConvexOffset = dynamicSolverData.collisionConstraintConvexOffset;
	
	dynamicSolverDataGPU.thread_count = dynamicSolverData.thread_count;
	dynamicSolverDataGPU.omp = dynamicSolverData.omp;


	//tmp
	//std::cout <<"Allocated dynamic data on the device" <<std::endl;
};

//set_static_solver_data_from_gpu
void cuda::set_static_solver_data_from_gpu(staticSolverData_t& staticSolverData, staticSolverData_t& staticSolverDataGPU)
{
	//size_vertex_vcs_vector
	size_t size_vertex_vcs_vector = staticSolverData.vertexCount * sizeof(vcsVector);
	//size_edge_vcs_vector
	size_t size_edge_vcs_vector = staticSolverData.edgeCount * sizeof(vcsVector);
	//size_float
	size_t size_float = staticSolverData.edgeCount * sizeof(float);
	
	
	//copy
	//pVertexPositionList
	cudaMemcpy(staticSolverData.pVertexPositionList, 
				staticSolverDataGPU.pVertexPositionList, 
				size_vertex_vcs_vector, 
				cudaMemcpyDeviceToHost);
	//pVertexOldpositionList
	cudaMemcpy(staticSolverData.pVertexOldpositionList, 
				staticSolverDataGPU.pVertexOldpositionList, 
				size_vertex_vcs_vector, 
				cudaMemcpyDeviceToHost);
	//pVertexInitialpositionList
	cudaMemcpy(staticSolverData.pVertexInitialpositionList, 
				staticSolverDataGPU.pVertexInitialpositionList, 
				size_vertex_vcs_vector, 
				cudaMemcpyDeviceToHost);
	//pEdgeVertexIndexList
	cudaMemcpy(staticSolverData.pEdgeVertexIndexList, 
				staticSolverDataGPU.pEdgeVertexIndexList, 
				size_edge_vcs_vector, 
				cudaMemcpyDeviceToHost);
	//pEdgeRestlengthList
	cudaMemcpy(staticSolverData.pEdgeRestlengthList, 
				staticSolverDataGPU.pEdgeRestlengthList, 
				size_float, 
				cudaMemcpyDeviceToHost);
};

//verletIntegration
void cuda::verletIntegration(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	
	//thread_count
	dim3 thread_count(get_thread_count(dynamicSolverData));
	//grid_size_vertex
	dim3 grid_size_vertex(get_grid_size(staticSolverData, dynamicSolverData));
	
	//verletIntegration
	kernels::verletIntegration<<<grid_size_vertex, thread_count>>>(staticSolverData, dynamicSolverData);
	
	//log
	//std::cout<<"Cuda::verletIntegrationGPU"<<std::endl;

	
};

//collisionConstraintGroundplane
void cuda::collisionConstraintGroundplane(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	//thread_count
	dim3 thread_count(get_thread_count(dynamicSolverData));
	//grid_size_vertex
	dim3 grid_size_vertex(get_grid_size(staticSolverData, dynamicSolverData));

	//kernel
	kernels::collisionConstraintGroundplane<<<grid_size_vertex, thread_count>>> (staticSolverData, dynamicSolverData);
	
	//log
	//std::cout<<"Cuda::collisionConstraintGroundplane"<<std::endl;

	
};

//collisionConstraintSpheres
void cuda::collisionConstraintSpheres(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	//thread_count
	dim3 thread_count(get_thread_count(dynamicSolverData));
	//grid_size_vertex
	dim3 grid_size_vertex(get_grid_size(staticSolverData, dynamicSolverData));

	//kernel
	kernels::collisionConstraintSpheres<<<grid_size_vertex, thread_count>>> (staticSolverData, dynamicSolverData);
	
	//log
	//std::cout<<"Cuda::collisionConstraintGroundplane"<<std::endl;

	
};

//collisionConstraintConvex
void cuda::collisionConstraintConvex(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	//log
	std::cout<<"Cuda::collisionConstraintConvex Dummy"<<std::endl;
};

//stickConstraint
void cuda::stickConstraint(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	//kernel (serial since dependent on serial execution)
	kernels::stickConstraint<<<1,1>>> (staticSolverData, dynamicSolverData);
};

//positionConstraints
void cuda::positionConstraints(staticSolverData_t &staticSolverData, dynamicSolverData_t &dynamicSolverData)
{
	//kernel (serial since dependent on serial execution)
	kernels::positionConstraints<<<1,1>>> (staticSolverData, dynamicSolverData);
};