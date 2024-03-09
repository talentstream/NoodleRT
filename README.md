### NOODLE RT

---

> Compiled in Windows 11, C++23

！！！There are still some bugs that need to be fixed, therefore the results are not guaranteed to be correct
---

![image](https://github.com/talentstream/NoodleRT/assets/72556475/edd9ce22-605f-4c55-878c-c624f06776aa)
![image](https://github.com/talentstream/NoodleRT/assets/72556475/f3a31647-6fd6-4489-85c5-d67d6d4eb8b0)

#### Features
- Integrator
  - Normal(For Debug)
  - Whitted
  - Path
- BSDF
  - [x] Diffuse(Lambert)
  - [x] RoughDiffuse(Oren-Nayer)
  - [x] Conductor(Smooth)
  - [x] Dielectric(Smooth)
  - [x] Plastic(Smooth)
  - [ ] Conductor(Rough)
  - [ ] Dielectric(Rough)
  - [ ] Plastic(Rough)
  - [ ] Phong 
- Camera
  - [x] Perspective
- Sampler
  - [x] Independent
- Accel
  - Naive
  - BVH
      - Equal Count
      - Mid Point
      - [ ] SAH
- Shape
  - [x] Sphere
  - Triangle
      - [x] Mesh
      - [x] Rectangle
      - [x] Cube
- Light
  - [x] Point
  - [x] Area
  - [ ] Environment
 

#### 参考

- [Ray Tracing in One Weekend](https://raytracing.github.io/)

- [BRDF参考](https://digibug.ugr.es/bitstream/handle/10481/19751/rmontes_LSI-2012-001TR.Pdf)

- [mitsuba3](https://mitsuba.readthedocs.io/en/latest/)

- [pbrt-v3](https://pbr-book.org/3ed-2018/contents)
