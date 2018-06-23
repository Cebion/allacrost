local ns = {};
setmetatable(ns, {__index = _G});
harrvah_destruction = ns;
setfenv(1, ns);

tileset_name = "Harrvah Destruction"
image = "img/tilesets/harrvah_destruction.png"

collisions = {}
collisions[0] = { 3, 3, 3, 3, 1, 2, 1, 15, 1, 2, 1, 3, 0, 0, 3, 0 }
collisions[1] = { 12, 12, 12, 12, 4, 8, 7, 15, 15, 11, 5, 15, 2, 1, 15, 10 }
collisions[2] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[3] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[4] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[5] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[6] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[7] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[8] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

animations = {}
animations[1] = { 176, 200, 177, 200, 178, 200, 179, 200, 180, 200 }
animations[2] = { 192, 200, 193, 200, 194, 200, 195, 200, 196, 200 }
animations[3] = { 208, 200, 210, 200, 212, 200, 214, 200, 216, 200 }
animations[4] = { 209, 200, 211, 200, 213, 200, 215, 200, 217, 200 }
animations[5] = { 224, 200, 226, 200, 228, 200, 230, 200, 232, 200 }
animations[6] = { 225, 200, 227, 200, 229, 200, 231, 200, 233, 200 }
animations[7] = { 240, 200, 242, 200, 244, 200, 246, 200, 248, 200 }
animations[8] = { 241, 200, 243, 200, 245, 200, 247, 200, 249, 200 }
animations[9] = { 218, 200, 219, 200, 220, 200, 221, 200, 222, 200 }
animations[10] = { 234, 200, 235, 200, 236, 200, 237, 200, 238, 200, 239, 200 }
animations[11] = { 250, 200, 251, 200, 252, 200, 253, 200, 254, 200, 255, 200 }
