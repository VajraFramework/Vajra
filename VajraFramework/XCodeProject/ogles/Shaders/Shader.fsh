//
//  Shader.fsh
//  ogles
//
//  Created by leviathan-mac on 9/27/13.
//  Copyright (c) 2013 vajra. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
