-- Setting the random seed to a fixed value will make the random number generator deterministic
-- The seed 66 when used with this script will always play the notes A4 and B4
-- Changing the seed or removing the call the math.randomseed() will make the script fully random
local seed = 66
math.randomseed(seed)
if tick % 128 == 0 then
  local n = math.random(69, 72)
  note(n, 1)
end
if tick % 256 == 0 then
  local n = math.random(69, 72)
  note(n, 1)
end

