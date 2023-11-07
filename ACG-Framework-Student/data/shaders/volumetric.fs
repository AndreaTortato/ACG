uniform vec3 u_camera_pos; 
uniform vec3 u_local_camera_pos; 
uniform float  u_ray_step_lenght; 
uniform sampler3D u_volume_texture;
uniform float u_brightness;

//this will store the color for the pixel shader
in vec3 v_position; 
in vec3 v_world_position;
in vec3 v_normal;
in vec2 v_uv;
in vec4 v_color;

void main() 
{
	int maxSteps = 100000;
    u_ray_step_lenght += 0.01;
    vec4 finalColor = vec4(0.0);

    //1. ray setup
    vec3 rayDir = normalize(v_position - u_local_camera_pos); //using u_camera_pos glithecs the volume when autorotating
    vec3 samplePos = v_position; //initialized as entry point to the volume

    for (int i = 0; i < maxSteps; i++) 
	{ 
		// 2. volume sampling
        vec3 text_coords = (samplePos + 1.0) / 2.0; //text_coords[0,1], samplePos[-1,1]
		float d = texture(u_volume_texture, text_coords).x; 

		//3. classification
		vec4 sample_color = vec4(d, d, d, d); 
        //vec4 sample_color = vec4(d, 1-d, 0, d*d)

		//4. composition
		finalColor += u_ray_step_lenght * (1.0 - finalColor.a) * sample_color; 

		  //5a. next sample
		samplePos += u_ray_step_lenght * rayDir; 	
	
		//5b. early termination
        //if sample is outside volume
		if (samplePos.x < -1.0 || samplePos.x > 1.0 || samplePos.y < -1.0 || samplePos.y > 1.0 || samplePos.z < -1.0 || samplePos.z > 1.0) 
        {
        	break; 
    	}
       //if alpha = 1 (totally opaque objeect)
		if (finalColor.a >= 1) 
        {
			break;
		}
    }
	gl_FragColor = finalColor * u_brightness;		
}