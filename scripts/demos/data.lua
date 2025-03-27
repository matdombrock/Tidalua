-- We can generate audio based on semi-arbitrary data sets
-- This is an example dataset of local temperatures
local highs = {
  20,
  20,
  22,
  23,
  24,
  19,
  18,
  14,
  12,
  17,
  20,
  22,
  24,
  25,
  22,
  20,
  18,
  13,
  14,
  16,
  20,
  22,
  22,
  23,
  24,
  19,
  18,
  17,
}
local lows = {
  10,
  10,
  12,
  13,
  14,
  9,
  8,
  4,
  2,
  7,
  10,
  12,
  14,
  16,
  12,
  10,
  8,
  3,
  4,
  6,
  10,
  12,
  12,
  13,
  14,
  9,
  8,
  7,
}
enable(2)
pan(-1, 1)
pan(1, 2)
-- We want to normalize the temperature to a range starting at zero
-- To make things easy we will just subtract the min value when reading a value from the table
local temp_high_min = 13
local temp_low_min = 2
local time = 32
if tick % time== 1 then
  local temp_high = highs[math.floor(tick / time) % #highs + 1] - temp_high_min
  -- Playing "random" notes sounds bad
  -- A quick "hack" is to always play fifths
  note(44 + temp_high * 5, 1)
  local temp_low = lows[math.floor(tick / time) % #lows + 1] - temp_low_min
  note(44 + temp_low * 5, 2)
end
