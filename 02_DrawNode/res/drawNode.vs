						
attribute vec4 a_position;	
attribute vec4 a_color;	
attribute vec2 a_texcoord;		
									
varying vec4 v_color;				
varying vec2 v_texcoord;			
		

uniform mat4 gWorld;						
									
void main()							
{		
    //v_color = a_color;
	v_color = vec4(a_color.rgb * a_color.a, a_color.a);	
	v_texcoord = a_texcoord;
									
	gl_Position = gWorld * a_position;
}

