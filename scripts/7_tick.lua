-- There are 128 ticks in a second
-- Quarter note ticks
local qnt = 128 / 4

-- Current position in the song
local tt = tick % (qnt * 8)

-- On quarter note
local function oqn(_tt, n)
  if n == 1 then
    return _tt == 1
  end
  return _tt == n * qnt
end

-- Set the tick speed
tick_speed(0.8) -- max 1.0

-- Apply some attack and release
atk_rel(8, 0, 12)

-- Trigger notes on quarter notes
if oqn(tt, 1) then
  note("A4")
elseif oqn(tt, 2) then
  note("C5")
elseif oqn(tt, 3) then
  note("D5")
elseif oqn(tt, 4) then
  note("E5")
elseif oqn(tt, 5) then
  note("F5")
elseif oqn(tt, 6) then
  note("G5")
elseif oqn(tt, 7) then
  note("A5")
end

