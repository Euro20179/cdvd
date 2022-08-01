print("starting...")

local size = sdl.size()
local WIDTH = size.width
local HEIGHT = size.height


print(WIDTH)

for i = 0,10,1 do
    dvd.add(math.random(0, WIDTH - 98), math.random(0, HEIGHT - 43), "DVD_Blue.png")
    print("success" .. i)
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

local randId = dvd.get_all()[1].id

print(dump(dvd.get_by_id(randId)))
