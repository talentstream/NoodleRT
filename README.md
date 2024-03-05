### NOODLE RT

--- 

Todo:

// Refactor ? 

Application:
- Integrator
  - aggregate1
    - Primitive
      - Shape 
      - Material
        - Texture
        - BxDF
  - Lights
  - Camera
    - Film
  - Sampler

- Todo : environment light?
- 
接下来
phong brdf ?
blinn-phong brdf? 
cook-torrance brdf?

path tracing & light path tracing? 

重构代码
先从aggregate开始
删除不需要的primitive

[BRDF参考](https://digibug.ugr.es/bitstream/handle/10481/19751/rmontes_LSI-2012-001TR.pdf)