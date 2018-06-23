local ns = {};
setmetatable(ns, {__index = _G});
ship_dock = ns;
setfenv(1, ns);

tileset_name = "Ship Dock"
image = "img/tilesets/ship_dock.png"

collisions = {}
collisions[0] = { 1, 3, 3, 3, 2, 10, 15, 5, 14, 15, 13, 10, 15, 15, 13, 5 }
collisions[1] = { 5, 0, 0, 0, 10, 2, 3, 1, 10, 3, 5, 2, 3, 3, 1, 5 }
collisions[2] = { 5, 0, 0, 0, 10, 8, 12, 4, 12, 12, 12, 12, 12, 12, 12, 4 }
collisions[3] = { 5, 0, 0, 0, 10, 10, 15, 5, 5, 10, 12, 12, 12, 12, 0, 10 }
collisions[4] = { 4, 12, 12, 12, 8, 8, 12, 4, 5, 10, 12, 12, 2, 1, 0, 10 }
collisions[5] = { 5, 14, 15, 13, 15, 15, 13, 11, 7, 0, 0, 0, 12, 12, 0, 8 }
collisions[6] = { 0, 10, 5, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[7] = { 10, 15, 15, 5, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[8] = { 15, 5, 10, 15, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[9] = { 5, 10, 5, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[10] = { 0, 10, 5, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[11] = { 15, 0, 3, 7, 15, 15, 3, 7, 10, 15, 3, 3, 0, 0, 0, 0 }
collisions[12] = { 5, 10, 12, 13, 15, 12, 15, 15, 10, 15, 12, 12, 15, 0, 0, 0 }
collisions[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
collisions[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
