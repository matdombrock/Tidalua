local LENGTH = 2048 * 256
local tt = sample_num % LENGTH
if tt >= 0 then
  local pos = (tt / LENGTH)
  pitch(1 + math.sin(pos * math.pi), 0)
  pitch(3 + math.sin(pos * math.pi * 2), 1)
end
