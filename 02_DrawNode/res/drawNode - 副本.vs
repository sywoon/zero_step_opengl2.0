#ifdef GL_ES						
attribute mediump vec4 a_position;	
attribute mediump vec2 a_texcoord;	
attribute mediump vec4 a_color;		
									
varying mediump vec4 v_color;		
varying mediump vec2 v_texcoord;	
									
#else								
attribute vec4 a_position;			
attribute vec2 a_texcoord;			
attribute vec4 a_color;				
									
varying vec4 v_color;				
varying vec2 v_texcoord;			
#endif		

uniform mat4 gWorld;						
									
void main()							
{									
	v_color = vec4(a_color.rgb * a_color.a, a_color.a);	
	v_texcoord = a_texcoord;
									
	gl_Position = gWorld * a_position;
}