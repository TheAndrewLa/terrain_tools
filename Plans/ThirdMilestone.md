## Plan for the third milestone

1. Improve hydraulic erosion algorithm (heightmap for water or water drop simulation). I need 3D view of terrain :(
1. Add texture generation algorithm
  	- Add algorithm for biom definition
  	- Add algorithm for calculate color for each biom
1. Add islands generation's algorithm
  	- The mountain regions are thickening
1. Create completely new shader system for graphics engine
	- Every material is presented by nodes, by design it's in SSA representation
	- Add some optimitzations to shader
	- Translate it into SPIR-V
1. Add Vulkan or DirectX support if it was't done in [first milestone](./FirstMilestone.md)

