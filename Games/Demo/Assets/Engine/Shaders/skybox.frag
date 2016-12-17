precision mediump float;
uniform samplerCube cubemap;

varying vec3 vPosition;

void main() {
	gl_FragColor = textureCube(cubemap, vPosition);
} 