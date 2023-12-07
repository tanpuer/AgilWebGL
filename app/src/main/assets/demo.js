var a = 1;
var b = 1;
let c = a + b;
console.log("c=2");
console.log(requestAnimationFrame);


// 创建顶点着色器
const vertexShaderSource = `
    attribute vec2 a_position;
    attribute vec2 a_texCoord;
    varying vec2 v_texCoord;
    attribute vec3 a_color;
    varying vec3 v_color;

    void main() {
        gl_Position = vec4(a_position, 0, 1);
        v_texCoord = a_texCoord;
        v_color = a_color;
    }
`;
const vertexShader = gl.createShader(gl.VERTEX_SHADER);
gl.shaderSource(vertexShader, vertexShaderSource);
gl.compileShader(vertexShader);

// 创建片段着色器
const fragmentShaderSource = `
    precision mediump float;
    varying vec2 v_texCoord;
    uniform sampler2D u_texture;
    varying vec3 v_color;

    void main() {
        vec2 flippedTexCoords = vec2(v_texCoord.s, 1.0 - v_texCoord.t);
        gl_FragColor = texture2D(u_texture, flippedTexCoords);
    }
`;
const fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);
gl.shaderSource(fragmentShader, fragmentShaderSource);
gl.compileShader(fragmentShader);

// 创建着色器程序
const program = gl.createProgram();
gl.attachShader(program, vertexShader);
gl.attachShader(program, fragmentShader);
gl.linkProgram(program);
gl.useProgram(program);

const positions = [
    -1.0, -1.0,
    1.0, -1.0,
    -1.0, 1.0,
    1.0, 1.0
];

const texCoords = [
    0.0, 0.0,
    1.0, 0.0,
    0.0, 1.0,
    1.0, 1.0
];

const colors = [
    1, 0, 1,
    0, 1, 0,
    0, 0, 1,
    1, 1, 0
];

// 创建顶点缓冲区
const positionBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);
gl.bindBuffer(gl.ARRAY_BUFFER, gl.NONE);

// 创建纹理坐标缓冲区
const texCoordBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, texCoordBuffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(texCoords), gl.STATIC_DRAW);
gl.bindBuffer(gl.ARRAY_BUFFER, gl.NONE);

// 创建颜色缓冲区
const colorBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);

// 创建并绑定纹理
const texture = gl.createTexture();
gl.activeTexture(gl.TEXTURE0 + texture);
gl.bindTexture(gl.TEXTURE_2D, texture);
gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, 'raining.png');
gl.bindTexture(gl.TEXTURE_2D, gl.NONE);

let mFrameCount = 0;
function setUniforms() {
    let timeMills = new Date().getTime();
    //1. iResolution
    let iResolution = gl.getUniformLocation(program, "iResolution");
    const resolution = [gl.drawingBufferWidth, gl.drawingBufferHeight];
    gl.uniform2fv(iResolution, 1, new Float32Array(resolution));
    //2. iTimeDelta
    let iTimeDelta = gl.getUniformLocation(program, "iTimeDelta");
    gl.uniform1f(iTimeDelta, 0.0);
    //3. iTime
    let iTime = gl.getUniformLocation(program, "iTime");
    gl.uniform1f(iTime, timeMills / 1000.0);
    //4. iFrame
    let iFrame = gl.getUniformLocation(program, "iFrame");
    mFrameCount++;
    gl.uniform1i(iFrame, mFrameCount);
    //5. iMouse ignore
    const mouse = [0.0, 0.0, 0.0];
    let iMouse = gl.getUniformLocation(program, "iMouse");
    gl.uniform3fv(iMouse, 1, new Float32Array(mouse));
    //6. iMouseButton ignore

    //7. iViewMatrix
//    let iViewMatrix = gl.getUniformLocation(program, "iViewMatrix");
//    setIdentityM(&matrix);
//    glUniformMatrix4fv(iViewMatrix, 1, GL_FALSE, matrix.m);
    //8. resolution
    gl.uniform2fv(gl.getUniformLocation(program, "resolution"), 1, new Float32Array(resolution));
    //9. time
    let time = gl.getUniformLocation(program, "time");
    gl.uniform1f(time, timeMills / 1000.0);
    //10. mouse ignore
    gl.uniform3fv(gl.getUniformLocation(program, "mouse"), 1, new Float32Array(mouse));
}

function setAttributes() {
    const positionAttributeLocation = gl.getAttribLocation(program, 'a_position');
    gl.enableVertexAttribArray(positionAttributeLocation);
    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
    gl.vertexAttribPointer(positionAttributeLocation, 2, gl.FLOAT, false, 0, 0);

    const texCoordAttributeLocation = gl.getAttribLocation(program, 'a_texCoord');
    gl.enableVertexAttribArray(texCoordAttributeLocation);
    gl.bindBuffer(gl.ARRAY_BUFFER, texCoordBuffer);
    gl.vertexAttribPointer(texCoordAttributeLocation, 2, gl.FLOAT, false, 0, 0);

    const colorAttributeLocation = gl.getAttribLocation(program, 'a_color');
    gl.enableVertexAttribArray(colorAttributeLocation);
    gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
    gl.vertexAttribPointer(colorAttributeLocation, 3, gl.FLOAT, false, 0, 0);
}

requestAnimationFrame(()=> {
    gl.viewport(0, 0, gl.drawingBufferWidth, gl.drawingBufferHeight);
    gl.useProgram(program);

    gl.clear(gl.COLOR_BUFFER_BIT);
    gl.clearColor(1.0, 1.0, 1.0, 1.0);

    setUniforms();
    setAttributes();

    // 获取纹理属性位置
//    gl.activeTexture(gl.TEXTURE0 + texture);
//    gl.bindTexture(gl.TEXTURE_2D, texture);
//    const textureLocation = gl.getUniformLocation(program, 'u_texture');
//    gl.uniform1i(textureLocation, texture - 1);

    gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);

    gl.bindBuffer(gl.ARRAY_BUFFER, gl.NONE);
    gl.bindTexture(gl.TEXTURE_2D, gl.NONE);
});