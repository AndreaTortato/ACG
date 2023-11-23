uniform vec3 u_camera_pos; 
uniform vec3 u_local_camera_pos; 
uniform float  u_ray_step_lenght; 
uniform sampler3D u_volume_texture;
uniform float u_brightness;
uniform vec4 u_color;

uniform bool u_jittering;
uniform bool u_transferF;
uniform bool u_clipping;
uniform bool u_isosurface;
uniform sampler2D u_tf_texture;

uniform float u_threshold;
uniform float h_value;
uniform vec4 u_plane;

uniform vec3 light_pos;
uniform vec3 ambient_light;
uniform vec3 diffuse_light;
uniform vec3 specular_light;
uniform vec3 ka;
uniform vec3 kd;
uniform float alpha;

//this will store the color for the pixel shader
in vec3 v_position; 
in vec3 v_world_position;
in vec3 v_normal;
in vec2 v_uv;
in vec4 v_color;

float rand(vec2 co)
{
    return fract(sin(dot(co,vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 gradient(vec3 text_coords, float h_value)
{ 
	//x, y, z positions with offset
	vec3 x1 = text_coords + vec3 (h_value, 0.0, 0.0);
	vec3 x2 = text_coords + vec3 (-h_value, 0.0, 0.0);
	vec3 y1 = text_coords + vec3 (0.0, h_value, 0.0);
	vec3 y2 = text_coords + vec3 (0.0, -h_value, 0.0);
	vec3 z1 = text_coords + vec3 (0.0, 0.0, h_value);
	vec3 z2 = text_coords + vec3 (0.0, 0.0, -h_value);
	
	//Sample intensity 
	float intensity_x1 = texture3D(u_volume_texture, x1).x;
	float intensity_x2 = texture3D(u_volume_texture, x2).x;
	float intensity_y1 = texture3D(u_volume_texture, y1).y;
	float intensity_y2 = texture3D(u_volume_texture, y2).y;
	float intensity_z1 = texture3D(u_volume_texture, z1).z;
	float intensity_z2 = texture3D(u_volume_texture, z2).z;

	//Compute intensity in each direction:
	float intensity_x = intensity_x1 - intensity_x2;
	float intensity_y = intensity_y1 - intensity_y2;
	float intensity_z = intensity_z1 - intensity_z2;
	
	//Compute gradient and normalize
	vec3 gradient = vec3(intensity_x, intensity_y, intensity_z) / (2.0 * h_value);
	gradient = normalize(gradient);
	vec3 normal = 1.0 - gradient;

	return normal;
}

//implemented from GTR
vec3 phong(vec3 v_normal, vec3 text_coords) 
{  
    vec4 color = u_color;

    vec3 N = normalize(v_normal);
    vec3 L = normalize(light_pos - text_coords);
    vec3 V = normalize(u_camera_pos - text_coords);
    vec3 R = normalize(reflect(-L, N));

    float LN = dot(L, N);
    LN = clamp(LN, 0.0, 1.0);

    float RV = dot(R, V);
    RV = clamp(RV, 0.0, 1.0);

    vec3 ambient = ka * ambient_light;
    vec3 diffuse = kd * (LN) * diffuse_light;
    vec3 specular = color.w * pow(RV, alpha) * specular_light;

    vec3 illumination = color.xyz * (diffuse + specular) + ambient;

    return illumination;
}

void main() 
{
    int maxSteps = 100000;
    vec4 finalColor = vec4(0.0);
    vec3 normal = vec3(0.0);
    vec4 phong_color = vec4(0.0);

    //1. ray setup
    vec3 rayDir = normalize(v_position - u_local_camera_pos); 
    vec3 samplePos = v_position; //initialized as entry point to the volume

    if (u_jittering)
    {
        // We did the second aproach
        float offset = rand(gl_FragCoord.xy);
        samplePos = v_position + u_ray_step_lenght * rayDir * offset;
    }
    
    for (int i = 0; i < maxSteps; i++) 
    { 
        // 2. volume sampling
        vec3 text_coords = (samplePos + 1.0) / 2.0; //text_coords[0,1], samplePos[-1,1]
        float d = texture(u_volume_texture, text_coords).x; //density
        
        //3. classification
        vec4 sample_color = vec4(d, d, d, d);         

        if (u_transferF && d > 0.1)
        {
            if (d < 0.3) 
            {
                sample_color = vec4(0.5 , 0, 0, 0.1); //texture(u_tf_texture, vec2(0.5, 0)); 
            }
            else if (d < 0.6) 
            {
                sample_color = vec4(0 , 0.5, 0, 0.8); //texture(u_tf_texture, vec2(0.5, 0));
            }
            else 
            {
                sample_color = vec4(0 , 0, 0.5, 1); //texture(u_tf_texture, vec2(1, 0));
            }            
        }

        if (u_isosurface)
        {
            if (d > u_threshold) 
            {
                normal = gradient(text_coords, h_value);
				phong_color = vec4(phong(normal, text_coords), 1.0);
            }
        }

        if (u_clipping && u_plane.x * samplePos.x + u_plane.y * samplePos.y + u_plane.z * samplePos.z + u_plane.a > 0)
        {
            sample_color = vec4(0);
            phong_color = vec4(0);
        }
        
        //4. composition
        if (u_isosurface) 
        {
            finalColor += u_ray_step_lenght * (1.0 - finalColor.a) * phong_color  * u_brightness;
        }
        else 
        {
            finalColor += u_ray_step_lenght * (1.0 - finalColor.a) * sample_color  * u_brightness; 
        }
        
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
    gl_FragColor = finalColor;
}
