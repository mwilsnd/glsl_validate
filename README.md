# glsl_validate
This module provides a thin function wrapper around `glslang`. There is one function: `compiles`.

```ts
function compiles(shaderSource: string, version: number, stage: string)
```

Example:
```ts
import glsl_validate from 'glsl_validate';

const vertexInfo = glsl_validate.compiles('#version 100\nvoid main()...', 100, 'vert');
const fragmentInfo = glsl_validate.compiles('#version 100\nvoid main()...', 100, 'frag');

if (!vertexInfo.compiled) {
    console.log(vertexInfo.log);
}

...
```
## Building
```
git clone --recursive https://github.com/mwilsnd/glsl_validate.git && cd glsl_validate
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build```