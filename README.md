### NOODLE RT

--- 

Todo:

// Refactor ? 

Application:
- Integrator
  - Aggregate
    - Primitive
      - Shape 
      - Material
        - Texture
        - BxDF
  - Lights
  - Camera
    - Film
- Sampler

先重构Integrator 的代码
已经将Aggregate移动至Integrator中
接下来移动Camera 已完成
接下来将Film塞入 Integrator，使得主要渲染逻辑都在Integrator，Film只负责展示
