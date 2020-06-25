#if !defined(VX3_VOXELGROUP_H)
#define VX3_VOXELGROUP_H

#include "VX3_vector.cuh"

class VX3_VoxelyzeKernel;
class VX3_Voxel;

class VX3_VoxelGroup {
public:
    /* data */
    VX3_VoxelyzeKernel *d_kernel; // saved pointer to the whole simulation
    int dim_x, dim_y, dim_z; // dimension of this group

    VX3_Voxel **d_group_map = NULL; // store 3-dimensional voxel pointers
    VX3_dVector<VX3_Voxel *> d_surface_voxels; // all surface voxels in this group
    VX3_dVector<VX3_Voxel *> d_voxels; // all voxels in this group

    int hasNewLink = 0; // how many new links in this group.

    __device__ VX3_VoxelGroup(VX3_VoxelyzeKernel *k);
    __device__ void switchAllVoxelsTo(VX3_VoxelGroup* group);
    __device__ VX3_Vec3D<int> moveGroupPosition(VX3_Vec3D<int> from, linkDirection dir, int step = 1); // return the step next position in the group
    __device__ int to1D(VX3_Vec3D<int> groupPosition); // for generating an index(offset) for `d_group_map` from groupPosition
    __device__ VX3_Vec3D<int> to3D(int offset); // get groupPosition back from an index(offset)

    // These two methods need to consider the racing condition:
    __device__ void updateGroup(VX3_Voxel *voxel); // Update all the group info that voxel is in. BFS.
    __device__ bool isCompatible(VX3_Voxel *voxel_host, VX3_Voxel *voxel_remote, int *ret_linkdir_1, int *ret_linkdir_2); // Check host and remote group are compatible for attachment.
    // These three vairables are used to handle the racing condition.
    bool needRebuild = true;
    int buildMutex = 0; // only allow one call to update(build) this group
    int checkMutex = 0; // checking compatibility and updating group info conflict with each other.

};

#endif // VX3_VOXELGROUP_H