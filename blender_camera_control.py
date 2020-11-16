import bpy
import numpy as np

positions = []
dist = 7

for x in np.arange(-3, 3, 0.2):
    for z in np.arange(-1, 3, 0.2):
        y = (max(0, dist**2 - x**2 - z**2))**0.5
        positions.append([x, -y, z])

obj = bpy.data.objects["Camera"]
frame = 0

for i in positions:
    bpy.context.scene.frame_set(frame)
    obj.location = i
    obj.keyframe_insert(data_path="location", index=-1)
    bpy.context.scene.render.filepath = 'C:/Users/ASUS/Desktop/3D_Look/images/'+str(frame)+'.jpg'
    bpy.ops.render.render(write_still = True)
    frame += 1
