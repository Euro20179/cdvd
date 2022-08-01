print("starting...")

sdl.resize(700, 700);


for i = 0,100,1 do
    dvd.add(math.random(0, sdl.width - 98), math.random(0, sdl.height - 43), "DVD_Blue.png")
end
print("Added dvds")

function bounce(id, _)
    sdl.background(math.random(0, 255), math.random(0, 255), math.random(0, 255), 255)
end

function dvd_bounce_x(id, _)
    bounce(id, _)
end
function dvd_bounce_y(id, _)
    bounce(id, _)
end


function dump(o)
   if type(o) == 'table' then
      local s = '{ '
      for k,v in pairs(o) do
         if type(k) ~= 'number' then k = '"'..k..'"' end
         s = s .. '['..k..'] = ' .. dump(v) .. ','
      end
      return s .. '} '
   else
      return tostring(o)
   end
end

function on_click(x, y)
    --sdl.set_fps(sdl.fps + 10)
    print(x, y)
end

