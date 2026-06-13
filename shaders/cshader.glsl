	#version 430 core

	layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;

	layout(std430, binding = 0) buffer Bounds{
		vec4 center_radius[];
	};

	layout(std430, binding = 1) buffer Visibility{
		uint visible[];
	};

	layout(std140, binding = 2) uniform CamData{
		vec4 planes[6];
	};
	
	uniform vec3 playerpos;

	void main(){
		//uvec3 gid = gl_GlobalInvocationID;
		//uvec3 grid    = gl_NumWorkGroups * gl_WorkGroupSize;
		//
		//uint i = gid.x + 
		//         gid.y * grid.x + 
		//		 gid.z * grid.x * grid.y;
		
		uint i = gl_GlobalInvocationID.x;
		
		vec3 center  = vec3(center_radius[i].x, playerpos.y, center_radius[i].z);
		float radius = center_radius[i].w;
		
		for(int p = 0; p < 6; p++){
			vec4 plane = planes[p];
			float distance = dot(plane.xyz, center) + plane.w;
			
			if(distance < -(radius + 1.0)) {
				visible[i] = 0u;
				return;
			}
		}
		
		visible[i] = 1u;
	}