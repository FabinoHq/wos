////////////////////////////////////////////////////////////////////////////////
//   _______                               ________________________________   //
//   \\ .   \                     ________/ . . . . . . . . . . . . . .   /   //
//    \\ .   \     ____       ___/ . . . . .   __________________________/    //
//     \\ .   \   //   \   __/. . .  _________/   /    // .  _________/       //
//      \\ .   \_//     \_//     ___/.  _____    /    // .  /_____            //
//       \\ .   \/   _   \/    _/// .  /    \\   |    \\  .       \           //
//        \\ .      /\\       /  || .  |    ||   |     \\______    \          //
//         \\ .    /  \\     /   || .  \____//   |  _________//    /          //
//          \\ .  /    \\   /    //  .           / // . . . .     /           //
//           \\__/      \\_/    //______________/ //_____________/            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//   This is free and unencumbered software released into the public domain.  //
//                                                                            //
//   Anyone is free to copy, modify, publish, use, compile, sell, or          //
//   distribute this software, either in source code form or as a compiled    //
//   binary, for any purpose, commercial or non-commercial, and by any        //
//   means.                                                                   //
//                                                                            //
//   In jurisdictions that recognize copyright laws, the author or authors    //
//   of this software dedicate any and all copyright interest in the          //
//   software to the public domain. We make this dedication for the benefit   //
//   of the public at large and to the detriment of our heirs and             //
//   successors. We intend this dedication to be an overt act of              //
//   relinquishment in perpetuity of all present and future rights to this    //
//   software under copyright law.                                            //
//                                                                            //
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          //
//   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF       //
//   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   //
//   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR        //
//   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,    //
//   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR    //
//   OTHER DEALINGS IN THE SOFTWARE.                                          //
//                                                                            //
//   For more information, please refer to <http://unlicense.org>             //
////////////////////////////////////////////////////////////////////////////////
//    WOS : Web Operating System                                              //
//      renderer/shadows.js : Shadows management                              //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//  Shadows class definition                                                  //
//  param renderer : Renderer pointer                                         //
////////////////////////////////////////////////////////////////////////////////
function Shadows(renderer)
{
    // Renderer pointer
    this.renderer = renderer;

    // Size of the shadows texture
    this.width = 0;
    this.height = 0;

    // Aspect ratio of the shadows texture
    this.ratio = 1.0;

    // Shadows framebuffer
    this.framebuffer = null;
    // Shadows texture
    this.texture = null;
    // Shadows depth texture
    this.depthTexture = null;

    // Shadows projection matrix
    this.projMatrix = new Matrix4x4();
    // Shadows view matrix
    this.viewMatrix = new Matrix4x4();

    // Shadows position
    this.position = new Vector3(0.0, 0.0, 0.0);

    // Shadows angles
    this.angles = new Vector3(0.0, 0.0, 0.0);

    // Shadows fovy
    this.fovy = 90.0;

    // Shadows near and far planes
    this.nearPlane = 0.01;
    this.farPlane = 100.0;
}

Shadows.prototype = {
    ////////////////////////////////////////////////////////////////////////////
    //  init : Init shadows                                                   //
    //  param width : Width of the background renderer                        //
    //  param height : Height of the background renderer                      //
    ////////////////////////////////////////////////////////////////////////////
    init: function(width, height)
    {
        // Reset shadows
        this.width = 0;
        this.height = 0;
        this.ratio = 1.0;
        this.framebuffer = null;
        this.texture = null;
        this.depthTexture = null;
        this.projMatrix.setIdentity();
        this.viewMatrix.setIdentity();
        this.position.reset();
        this.angles.reset();
        this.fovy = 90.0;
        this.nearPlane = 0.01;
        this.farPlane = 100.0;

        // Check renderer pointer
        if (!this.renderer) return false;

        // Check gl pointer
        if (!this.renderer.gl) return false;

        // Check renderer max quality
        if (this.renderer.maxQuality < WOSRendererQualityHigh) return false;

        // Set shadows caster renderer size
        if (width !== undefined) this.width = Math.round(width);
        if (height !== undefined) this.height = Math.round(height);
        if (this.width <= 1) { this.width = 1; }
        if (this.width >= WOSGLMaxTextureSize)
        {
            this.width = WOSGLMaxTextureSize;
        }
        if (this.height <= 1) { this.height = 1; }
        if (this.height >= WOSGLMaxTextureSize)
        {
            this.height = WOSGLMaxTextureSize;
        }
        if (this.height > 0.0) this.ratio = this.width/this.height;

        // Init shadows framebuffer
        this.framebuffer = this.renderer.gl.createFramebuffer();
        if (!this.framebuffer)
        {
            // Could not create framebuffer
            return false;
        }
        this.renderer.gl.bindFramebuffer(
            this.renderer.gl.FRAMEBUFFER, this.framebuffer
        );

        // Init shadows texture
        this.texture = this.renderer.gl.createTexture();
        if (!this.texture)
        {
            // Could not create texture
            return false;
        }
        this.renderer.gl.bindTexture(this.renderer.gl.TEXTURE_2D, this.texture);
        this.renderer.gl.texImage2D(
            this.renderer.gl.TEXTURE_2D, 0, this.renderer.gl.RGBA,
            this.width, this.height, 0, this.renderer.gl.RGBA,
            this.renderer.gl.UNSIGNED_BYTE, null
        );

        // Set texture wrap mode
        this.renderer.gl.texParameteri(
            this.renderer.gl.TEXTURE_2D,
            this.renderer.gl.TEXTURE_WRAP_S,
            this.renderer.gl.CLAMP_TO_EDGE
        );
        this.renderer.gl.texParameteri(
            this.renderer.gl.TEXTURE_2D,
            this.renderer.gl.TEXTURE_WRAP_T,
            this.renderer.gl.CLAMP_TO_EDGE
        );

        // Set texture min and mag filters
        this.renderer.gl.texParameteri(
            this.renderer.gl.TEXTURE_2D,
            this.renderer.gl.TEXTURE_MIN_FILTER,
            this.renderer.gl.LINEAR
        );
        this.renderer.gl.texParameteri(
            this.renderer.gl.TEXTURE_2D,
            this.renderer.gl.TEXTURE_MAG_FILTER,
            this.renderer.gl.LINEAR
        );

        // Attach texture to framebuffer
        this.renderer.gl.framebufferTexture2D(
            this.renderer.gl.FRAMEBUFFER, this.renderer.gl.COLOR_ATTACHMENT0,
            this.renderer.gl.TEXTURE_2D, this.texture, 0
        );

        // Unbind texture
        this.renderer.gl.bindTexture(this.renderer.gl.TEXTURE_2D, null);

        // Init shadows depth texture
        this.depthTexture = this.renderer.gl.createTexture();
        if (!this.depthTexture)
        {
            // Could not create depth texture
            return false;
        }
        this.renderer.gl.bindTexture(
            this.renderer.gl.TEXTURE_2D, this.depthTexture
        );
        this.renderer.gl.texImage2D(
            this.renderer.gl.TEXTURE_2D, 0, this.renderer.gl.DEPTH_COMPONENT,
            this.width, this.height, 0, this.renderer.gl.DEPTH_COMPONENT,
            this.renderer.gl.UNSIGNED_INT, null
        );

        // Set depth texture wrap mode
        this.renderer.gl.texParameteri(
            this.renderer.gl.TEXTURE_2D,
            this.renderer.gl.TEXTURE_WRAP_S,
            this.renderer.gl.CLAMP_TO_EDGE
        );
        this.renderer.gl.texParameteri(
            this.renderer.gl.TEXTURE_2D,
            this.renderer.gl.TEXTURE_WRAP_T,
            this.renderer.gl.CLAMP_TO_EDGE
        );

        // Set depth texture min and mag filters
        this.renderer.gl.texParameteri(
            this.renderer.gl.TEXTURE_2D,
            this.renderer.gl.TEXTURE_MIN_FILTER,
            this.renderer.gl.LINEAR
        );
        this.renderer.gl.texParameteri(
            this.renderer.gl.TEXTURE_2D,
            this.renderer.gl.TEXTURE_MAG_FILTER,
            this.renderer.gl.LINEAR
        );

        // Attach depth texture to framebuffer
        this.renderer.gl.framebufferTexture2D(
            this.renderer.gl.FRAMEBUFFER, this.renderer.gl.DEPTH_ATTACHMENT,
            this.renderer.gl.TEXTURE_2D, this.depthTexture, 0
        );

        // Check framebuffer status
        if (this.renderer.gl.checkFramebufferStatus(
            this.renderer.gl.FRAMEBUFFER) !=
            this.renderer.gl.FRAMEBUFFER_COMPLETE)
        {
            // Invalid framebuffer status
            return false;
        }

        // Unbind texture
        this.renderer.gl.bindTexture(this.renderer.gl.TEXTURE_2D, null);

        // Unbind framebuffer
        this.renderer.gl.bindFramebuffer(this.renderer.gl.FRAMEBUFFER, null);

        // Set projection matrix
        this.projMatrix.setIdentity();
        this.projMatrix.setPerspective(
            this.fovy, this.ratio, this.nearPlane, this.farPlane
        );

        // Set view matrix
        this.viewMatrix.setIdentity();

        // Shadows loaded
        return true;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  clear : Clear shadows buffer                                          //
    ////////////////////////////////////////////////////////////////////////////
    clear: function()
    {
        // Check renderer max quality
        if (this.renderer.maxQuality < WOSRendererQualityHigh) return;

        // Bind shadows framebuffer
        this.renderer.gl.bindFramebuffer(
            this.renderer.gl.FRAMEBUFFER, this.framebuffer
        );

        // Update viewport
        this.renderer.gl.viewport(0, 0, this.width, this.height);
        this.renderer.gl.scissor(0, 0, this.width, this.height);
        this.renderer.gl.disable(this.renderer.gl.SCISSOR_TEST);

        // Enable depth test
        this.renderer.gl.enable(this.renderer.gl.DEPTH_TEST);

        // Set shadows buffer renderer clear color
        this.renderer.gl.clearColor(0.0, 0.0, 0.0, 0.0);

        // Clear shadows buffer renderer
        this.renderer.gl.clear(
            this.renderer.gl.COLOR_BUFFER_BIT |
            this.renderer.gl.DEPTH_BUFFER_BIT
        );

        // Compute projection matrix
        this.projMatrix.setIdentity();
        this.projMatrix.setPerspective(
            this.fovy, this.ratio, this.nearPlane, this.farPlane
        );

        // Compute view matrix
        this.viewMatrix.setIdentity();
        this.viewMatrix.rotateX(this.angles.vec[0]);
        this.viewMatrix.rotateY(this.angles.vec[1]);
        this.viewMatrix.rotateY(this.angles.vec[2]);
        this.viewMatrix.translateVec3(this.position);

        // Set renderer matrices
        this.renderer.camera.projMatrix.setMatrix(this.projMatrix);
        this.renderer.camera.viewMatrix.setMatrix(this.viewMatrix);
    },

    ////////////////////////////////////////////////////////////////////////////
    //  setActive : Set shadows buffer as active renderer                     //
    ////////////////////////////////////////////////////////////////////////////
    setActive: function()
    {
        // Check renderer max quality
        if (this.renderer.maxQuality < WOSRendererQualityHigh) return;

        // Enable depth test
        this.renderer.gl.enable(this.renderer.gl.DEPTH_TEST);

        // Bind framebuffer
        this.renderer.gl.bindFramebuffer(
            this.renderer.gl.FRAMEBUFFER, this.framebuffer
        );

        // Update viewport
        this.renderer.gl.viewport(0, 0, this.width, this.height);
        this.renderer.gl.scissor(0, 0, this.width, this.height);
        this.renderer.gl.disable(this.renderer.gl.SCISSOR_TEST);

        // Compute view matrix
        this.viewMatrix.setIdentity();
        this.viewMatrix.rotateX(this.angles.vec[0]);
        this.viewMatrix.rotateY(this.angles.vec[1]);
        this.viewMatrix.rotateY(this.angles.vec[2]);
        this.viewMatrix.translateVec3(this.position);

        // Set renderer matrices
        this.renderer.camera.projMatrix.setMatrix(this.projMatrix);
        this.renderer.camera.viewMatrix.setMatrix(this.viewMatrix);

        // Set renderer matrices
        this.renderer.camera.projMatrix.setMatrix(this.projMatrix);
        this.renderer.camera.viewMatrix.setMatrix(this.viewMatrix);
    },

    ////////////////////////////////////////////////////////////////////////////
    //  setPosition : Set the shadows caster's position                       //
    //  param x : X position of the shadows caster                            //
    //  param y : Y position of the shadows caster                            //
    //  param z : Z position of the shadows caster                            //
    ////////////////////////////////////////////////////////////////////////////
    setPosition: function(x, y, z)
    {
        this.position.vec[0] = -x;
        this.position.vec[1] = -y;
        this.position.vec[2] = -z;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  setPositionVec3 : Set shadows caster's position from a vector         //
    //  param vector : 3 components vector to set shadows caster position     //
    ////////////////////////////////////////////////////////////////////////////
    setPositionVec3: function(vector)
    {
        this.position.vec[0] = -vector.vec[0];
        this.position.vec[1] = -vector.vec[1];
        this.position.vec[2] = -vector.vec[2];
    },

    ////////////////////////////////////////////////////////////////////////////
    //  setX : Set the shadows caster's X position                            //
    //  param x : X position of the shadows caster                            //
    ////////////////////////////////////////////////////////////////////////////
    setX: function(x)
    {
        this.position.vec[0] = -x;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  setY : Set the shadows caster's Y position                            //
    //  param y : Y position of the shadows caster                            //
    ////////////////////////////////////////////////////////////////////////////
    setY: function(y)
    {
        this.position.vec[1] = -y;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  setZ : Set the shadows caster's Z position                            //
    //  param z : Z position of the shadows caster                            //
    ////////////////////////////////////////////////////////////////////////////
    setZ: function(z)
    {
        this.position.vec[2] = -z;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  move : Move the shadows caster                                        //
    //  param x : Value of the translation on the X axis                      //
    //  param y : Value of the translation on the Y axis                      //
    //  param z : Value of the translation on the Z axis                      //
    ////////////////////////////////////////////////////////////////////////////
    move: function(x, y, z)
    {
        this.position.vec[0] -= x;
        this.position.vec[1] -= y;
        this.position.vec[2] -= z;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  moveVec3 : Move the shadows caster with a 3 components vector         //
    //  param vector : 3 components vector to move the shadows caster by      //
    ////////////////////////////////////////////////////////////////////////////
    moveVec3: function(vector)
    {
        this.position.vec[0] -= vector.vec[0];
        this.position.vec[1] -= vector.vec[1];
        this.position.vec[2] -= vector.vec[2];
    },

    ////////////////////////////////////////////////////////////////////////////
    //  moveX : Move the shadows caster on the X axis                         //
    //  param x : Value of the translation on the X axis                      //
    ////////////////////////////////////////////////////////////////////////////
    moveX: function(x)
    {
        this.position.vec[0] -= x;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  moveY : Move the shadows caster on the Y axis                         //
    //  param y : Value of the translation on the Y axis                      //
    ////////////////////////////////////////////////////////////////////////////
    moveY: function(y)
    {
        this.position.vec[1] -= y;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  moveZ : Move the shadows caster on the Z axis                         //
    //  param z : Value of the translation on the Z axis                      //
    ////////////////////////////////////////////////////////////////////////////
    moveZ: function(z)
    {
        this.position.vec[2] -= z;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  setAngles : Set shadows caster rotation angles                        //
    //  param angleX : Shadows caster X rotation angle to set in degrees      //
    //  param angleY : Shadows caster Y rotation angle to set in degrees      //
    //  param angleZ : Shadows caster Z rotation angle to set in degrees      //
    ////////////////////////////////////////////////////////////////////////////
    setAngles: function(angleX, angleY, angleZ)
    {
        this.angles.vec[0] = -angleX;
        this.angles.vec[1] = -angleY;
        this.angles.vec[2] = -angleZ;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  setAnglesVec3 : Set shadows caster rotation angles from a vector      //
    //  param vector : 3 components vector to set rotation angles from        //
    ////////////////////////////////////////////////////////////////////////////
    setAnglesVec3: function(vector)
    {
        this.angles.vec[0] = -vector.vec[0];
        this.angles.vec[1] = -vector.vec[1];
        this.angles.vec[2] = -vector.vec[2];
    },

    ////////////////////////////////////////////////////////////////////////////
    //  setAngleX : Set shadows caster X rotation angle                       //
    //  param angleX : Shadows caster X rotation angle to set in degrees      //
    ////////////////////////////////////////////////////////////////////////////
    setAngleX: function(angleX)
    {
        this.angles.vec[0] = -angleX;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  setAngleY : Set shadows caster Y rotation angle                       //
    //  param angleY : Shadows caster Y rotation angle to set in degrees      //
    ////////////////////////////////////////////////////////////////////////////
    setAngleY: function(angleY)
    {
        this.angles.vec[1] = -angleY;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  setAngleZ : Set shadows caster Z rotation angle                       //
    //  param angleZ : Shadows caster Z rotation angle to set in degrees      //
    ////////////////////////////////////////////////////////////////////////////
    setAngleZ: function(angleZ)
    {
        this.angles.vec[2] = -angleZ;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  rotateX : Rotate the shadows caster along the X axis                  //
    //  param angleX : Value of the X rotation in degrees                     //
    ////////////////////////////////////////////////////////////////////////////
    rotateX: function(angleX)
    {
        this.angles.vec[0] -= angleX;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  rotateY : Rotate the shadows caster along the Y axis                  //
    //  param angleY : Value of the Y rotation in degrees                     //
    ////////////////////////////////////////////////////////////////////////////
    rotateY: function(angleX)
    {
        this.angles.vec[1] -= angleY;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  rotateZ : Rotate the shadows caster along the Z axis                  //
    //  param angleZ : Value of the Z rotation in degrees                     //
    ////////////////////////////////////////////////////////////////////////////
    rotateZ: function(angleZ)
    {
        this.angles.vec[2] -= angleZ;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  setFovy : Set the shadows caster fovy angle                           //
    //  param fovy : Value of the shadows caster fovy in degrees              //
    ////////////////////////////////////////////////////////////////////////////
    setFovy: function(fovy)
    {
        this.fovy = fovy;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  setNearPlane : Set the shadows caster near plane                      //
    //  param nearPlane : Value of the shadows caster near plane              //
    ////////////////////////////////////////////////////////////////////////////
    setNearPlane: function(nearPlane)
    {
        this.nearPlane = nearPlane;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  setFarPlane : Set the shadows caster far plane                        //
    //  param farPlane : Value of the shadows caster far plane                //
    ////////////////////////////////////////////////////////////////////////////
    setFarPlane: function(farPlane)
    {
        this.farPlane = farPlane;
    },

    ////////////////////////////////////////////////////////////////////////////
    //  getX : Get the shadows caster's X position                            //
    //  return : X position of the shadows caster                             //
    ////////////////////////////////////////////////////////////////////////////
    getX: function()
    {
        return this.position.vec[0];
    },

    ////////////////////////////////////////////////////////////////////////////
    //  getY : Get the shadows caster's Y position                            //
    //  return : Y position of the shadows caster                             //
    ////////////////////////////////////////////////////////////////////////////
    getY: function()
    {
        return this.position.vec[1];
    },

    ////////////////////////////////////////////////////////////////////////////
    //  getZ : Get the shadows caster's Z position                            //
    //  return : Z position of the shadows caster                             //
    ////////////////////////////////////////////////////////////////////////////
    getZ: function()
    {
        return this.position.vec[2];
    },

    ////////////////////////////////////////////////////////////////////////////
    //  getAngle : Get the shadows caster's X rotation angle                  //
    //  return : X rotation angle of the shadows caster                       //
    ////////////////////////////////////////////////////////////////////////////
    getAngleX: function()
    {
        return this.angles.vec[0];
    },

    ////////////////////////////////////////////////////////////////////////////
    //  getAngleY : Get the shadows caster's Y rotation angle                 //
    //  return : Y rotation angle of the shadows caster                       //
    ////////////////////////////////////////////////////////////////////////////
    getAngleY: function()
    {
        return this.angles.vec[1];
    },

    ////////////////////////////////////////////////////////////////////////////
    //  getAngleZ : Get the shadows caster's Z rotation angle                 //
    //  return : Z rotation angle of the shadows caster                       //
    ////////////////////////////////////////////////////////////////////////////
    getAngleZ: function()
    {
        return this.angles.vec[2];
    }
};
