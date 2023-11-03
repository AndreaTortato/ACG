uniform vec3 u_camera_pos;
uniform float u_ray_step_lenght; //to control with slider
uniform sampler3D volume_texture;

uniform mat4 u_model;
uniform mat4 u_viewprojection;

//this will store the color for the pixel shader
in vec3 v_position;
in vec3 v_world_position;
in vec3 v_normal;
in vec2 v_uv;
in vec4 v_color;

void main() 
{
    vec3 rayDir = normalize(v_world_position - u_camera_pos); //ray setup
    vec3 samplePos = v_world_position;

    int maxSteps = 50;
    vec4 finalColor = vec4(0.0);

    for (int i = 0; i < maxSteps; i++) {
		
        vec3 texCoor = samplePos; 
		float d = texture(volume_texture, texCoor).x; //volume sampling

		vec4 sample_color = vec4(d, d, d, d); //classification
        //vec4 sample_color = vec4(d, 1-d, 0, d*d)

		finalColor += u_ray_step_lenght * (1.0 - finalColor.a) * sample_color; //composition

		samplePos += rayDir * u_ray_step_lenght; //next sample

		if (samplePos.x < -1.0 || samplePos.x > 1.0 || samplePos.y < -1.0 || samplePos.y > 1.0 || samplePos.z < -1.0 || samplePos.z > 1.0) 
        {
        	break; 
    	}
		if (finalColor.a >= 1) //early termination
        {
			break;
		}
    }
    gl_FragColor = finalColor;
}