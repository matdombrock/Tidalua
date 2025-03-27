-- This script requires the "vis" feature to be enabled as well as "input" which is still a WIP

enable(2)
detune(0.5, 1)

local keys = {
  a = "C4",
  s = "D4",
  d = "E4",
  f = "F4",
  g = "G4",
  h = "A4",
  j = "B4",
  k = "C5",
  l = "D5",
}

env(8, 32, 16, 1)
env(8, 32, 16, 2)

if keys[keypress]  and (mem_get(1) ~= string.byte(keypress))  then
  -- Convert keypress string to a number
  local kp = string.byte(keypress)
  mem_set(kp, 1)
  amp(1, 1)
  amp(1, 2)
  note(keys[keypress], 1)
  note(keys[keypress], 2)
else
  -- amp(0, 1)
  -- amp(0, 2)
end
